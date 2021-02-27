/* ED_SSVFile.c - SSV functions
 *
 * Copyright (C) 2015-2021, tbeu
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if defined(__gnu_linux__)
#define _GNU_SOURCE 1
#endif

#include <string.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include "ED_locale.h"
#include "ED_logging.h"
#include "ED_ptrtrack.h"
#include "bsxml.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_SSVFile.h"

typedef struct {
	char* fileName;
	char* ns;
	XmlNodeRef root;
	ED_LOCALE_TYPE loc;
	ED_LOGGING_FUNC log;
	char ns_sep;
} SSVFile;

void* ED_createSSV(const char* fileName, const char* ns, int verbose, int detectMissingData)
{
	XmlParser xmlParser;
	SSVFile* ssv = (SSVFile*)malloc(sizeof(SSVFile));
	if (ssv == NULL) {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	ssv->fileName = strdup(fileName);
	if (ssv->fileName == NULL) {
		free(ssv);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	if ('\0' == ns[0]) {
		ssv->ns = NULL;
		ssv->ns_sep = '\0';
	}
	else {
		ssv->ns = strdup(ns);
		if (ssv->ns == NULL) {
			free(ssv->fileName);
			free(ssv);
			ModelicaError("Memory allocation error\n");
			return NULL;
		}
		ssv->ns_sep = ':';
	}

	if (verbose == 1) {
		/* Print info message, that file is loading */
		ModelicaFormatMessage("... loading \"%s\"\n", fileName);
	}

	ssv->root = XmlParser_parse_file_ns(&xmlParser, fileName, ssv->ns_sep);
	if (ssv->root == NULL) {
		free(ssv->fileName);
		free(ssv);
		if (XmlParser_getErrorLineSet(&xmlParser) != 0) {
			ModelicaFormatError("Error \"%s\" in line %lu: Cannot parse file \"%s\"\n",
				XmlParser_getErrorString(&xmlParser), XmlParser_getErrorLine(&xmlParser), fileName);
		}
		else {
			ModelicaFormatError("Cannot read \"%s\": %s\n", fileName, XmlParser_getErrorString(&xmlParser));
		}
		return NULL;
	}
	ssv->loc = ED_INIT_LOCALE;
	switch (detectMissingData) {
		case ED_LOG_NONE:
			ssv->log = ED_LogNone;
			break;
		case ED_LOG_DEBUG:
			ssv->log = ED_LogDebug;
			break;
		case ED_LOG_ERROR:
			ssv->log = ED_LogError;
			break;
		default:
			ssv->log = ED_LogWarning;
			break;
	}
	ED_PTR_ADD(ssv);
	return ssv;
}

void ED_destroySSV(void* _ssv)
{
	SSVFile* ssv = (SSVFile*)_ssv;
	ED_PTR_CHECK(ssv);
	if (ssv != NULL) {
		if (ssv->fileName != NULL) {
			free(ssv->fileName);
		}
		if (ssv->ns != NULL) {
			free(ssv->ns);
		}
		XmlNode_deleteTree(ssv->root);
		ED_FREE_LOCALE(ssv->loc);
		free(ssv);
		ED_PTR_DEL(ssv);
	}
}

static int XmlNode_Paramcomparer(const void* a, const void* b)
{
	const char* attr_a = XmlNode_getAttributeValue((XmlNode *)a, "name");
	const char* attr_b = XmlNode_getAttributeValue((XmlNode *)b, "name");
	if (attr_a != NULL && attr_b != NULL)
		return strcmp(attr_a, attr_b);
	else if (attr_a != NULL)
		return 1;
	else if (attr_b != NULL)
		return -1;
	return 0;
}

static XmlNodeRef XmlNode_findParam(XmlNodeRef node, const char* param)
{
	XmlNodeRef ret;
	XmlNode tmpNode = {0};
	tmpNode.m_type = NODE_CHILD;
	tmpNode.m_attributes = cpo_array_create(1, sizeof(struct XmlAttribute));
	XmlNode_setAttribute(&tmpNode, "name", param);
	ret = (XmlNodeRef)cpo_array_bsearch(node->m_childs, &tmpNode, XmlNode_Paramcomparer);
	XmlNode_delete(&tmpNode);
	return ret;
}

static char* findValue(XmlNodeRef* root, const char* varName, const char* fileName, const char* ns, char ns_sep, const char* typeName)
{
	char* token = NULL;
	int elementError = 1;
	XmlNodeRef paramsNode;
	size_t ns_len = ns != NULL ? strlen(ns) + 1 : 0;
	char* tag = (char*)malloc(ns_len + 11);
	if (tag == NULL) {
		*root = NULL;
		return token;
	}
	if (ns != NULL) {
		char tmp[2];
		*tmp = ns_sep;
		strcpy(tag, ns);
		strncat(tag, tmp, 1);
	}
	strcpy(tag + ns_len, "Parameters");
	paramsNode = XmlNode_findChild(*root, tag);
	if (paramsNode != NULL) {
		XmlNodeRef iter = XmlNode_findParam(paramsNode, varName);
		*root = paramsNode;
		if (iter != NULL) {
			*root = iter;
			strcpy(tag + ns_len, "Parameter");
			if (XmlNode_isTag(iter, tag)) {
				elementError = 0;
				if (1 == XmlNode_getChildCount(iter)) {
					iter = XmlNode_getChild(iter, 0);
					if (iter != NULL) {
						*root = iter;
						strcpy(tag + ns_len, typeName);
						if (XmlNode_isTag(iter, tag)) {
							token = XmlNode_getAttributeValue(*root, "value");
							free(tag);
							return token;
						}
						else {
							ModelicaFormatMessage("Error in line %i: Cannot find element \"%s%c%s\" for parameter \"%s\" in file \"%s\"\n",
								XmlNode_getLine(*root), ns != NULL ? "ssv" : "", ns_sep, typeName, varName, fileName);
							*root = NULL;
							free(tag);
							return token;
						}
					}
				}
				else {
					ModelicaFormatMessage("Error in line %i: Exactly one child element for parameter \"%s\" must be present in file \"%s\"\n",
						XmlNode_getLine(*root), varName, fileName);
					*root = NULL;
					free(tag);
					return token;
				}
			}
			else {
				ModelicaFormatMessage("Error in line %i: Invalid element \"%s\" in file \"%s\"\n",
					XmlNode_getLine(*root), XmlNode_getTag(*root), fileName);
				*root = NULL;
				free(tag);
				return token;
			}
		}
	}
	if (1 == elementError && *root != NULL) {
		ModelicaFormatMessage("Error in line %i: Cannot find element \"%s%cParameter\" for parameter \"%s\" in file \"%s\"\n",
			XmlNode_getLine(*root), ns != NULL ? "ssv" : "", ns_sep, varName, fileName);
		*root = NULL;
		token = NULL;
	}
	free(tag);
	return token;
}

double ED_getDoubleFromSSV(void* _ssv, const char* varName, int* exist)
{
	double ret = 0.;
	SSVFile* ssv = (SSVFile*)_ssv;
	ED_PTR_CHECK(ssv);
	if (ssv != NULL) {
		XmlNodeRef root = ssv->root;
		char* token = findValue(&root, varName, ssv->fileName, ssv->ns, ssv->ns_sep, "Real");
		*exist = 1;
		if (token != NULL) {
			if (ED_strtod(token, ssv->loc, &ret, ED_STRICT)) {
				ModelicaFormatError("Error in line %i: Cannot read double value \"%s\" from file \"%s\"\n",
					XmlNode_getLine(root), token, ssv->fileName);
			}
		}
		else if (NULL != root) {
			ssv->log("Line %i: Cannot read double value from file \"%s\"\n",
				XmlNode_getLine(root), ssv->fileName);
			*exist = 0;
		}
		else {
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return ret;
}

const char* ED_getStringFromSSV(void* _ssv, const char* varName, int* exist)
{
	SSVFile* ssv = (SSVFile*)_ssv;
	ED_PTR_CHECK(ssv);
	if (ssv != NULL) {
		XmlNodeRef root = ssv->root;
		char* token = findValue(&root, varName, ssv->fileName, ssv->ns, ssv->ns_sep, "String");
		*exist = 1;
		if (token != NULL) {
			char* ret = ModelicaAllocateString(strlen(token));
			strcpy(ret, token);
			return (const char*)ret;
		}
		else if (NULL != root) {
			ssv->log("Line %i: Cannot read value from file \"%s\"\n",
				XmlNode_getLine(root), ssv->fileName);
			*exist = 0;
		}
		else {
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return "";
}

int ED_getIntFromSSV(void* _ssv, const char* varName, int* exist)
{
	long ret = 0;
	SSVFile* ssv = (SSVFile*)_ssv;
	ED_PTR_CHECK(ssv);
	if (ssv != NULL) {
		XmlNodeRef root = ssv->root;
		char* token = findValue(&root, varName, ssv->fileName, ssv->ns, ssv->ns_sep, "Integer");
		*exist = 1;
		if (token != NULL) {
			if (ED_strtol(token, ssv->loc, &ret, ED_STRICT)) {
				ModelicaFormatError("Error in line %i: Cannot read int value \"%s\" from file \"%s\"\n",
					XmlNode_getLine(root), token, ssv->fileName);
			}
		}
		else if (NULL != root) {
			ssv->log("Line %i: Cannot read int value from file \"%s\"\n",
				XmlNode_getLine(root), ssv->fileName);
			*exist = 0;
		}
		else {
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return (int)ret;
}

int ED_getBooleanFromSSV(void* _ssv, const char* varName, int* exist)
{
	int ret = 0;
	SSVFile* ssv = (SSVFile*)_ssv;
	ED_PTR_CHECK(ssv);
	if (ssv != NULL) {
		XmlNodeRef root = ssv->root;
		char* token = findValue(&root, varName, ssv->fileName, ssv->ns, ssv->ns_sep, "Boolean");
		*exist = 1;
		if (token != NULL) {
			if (0 == strcmp(token, "true") || 0 == strcmp(token, "1")) {
				ret = 1;
			}
			else if (0 == strcmp(token, "false") || 0 == strcmp(token, "0")) {
				ret = 0;
			}
			else {
				ModelicaFormatError("Error in line %i: Cannot read boolean value \"%s\" from file \"%s\"\n",
					XmlNode_getLine(root), token, ssv->fileName);
			}
		}
		else if (NULL != root) {
			ssv->log("Line %i: Cannot read boolean value from file \"%s\"\n",
				XmlNode_getLine(root), ssv->fileName);
			*exist = 0;
		}
		else {
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return ret;
}
