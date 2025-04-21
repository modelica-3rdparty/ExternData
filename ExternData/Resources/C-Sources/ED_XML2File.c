/* ED_XML2File.c - XML2 functions
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include "ModelicaUtilities.h"
#include "ED_XML2File.h"

/* The standard way to detect posix is to check _POSIX_VERSION,
 * which is defined in <unistd.h>
 */
#if defined(__unix__) || defined(__linux__) || defined(__APPLE_CC__)
#include <unistd.h>
#endif
#if !defined(_POSIX_) && defined(_POSIX_VERSION)
#define _POSIX_ 1
#endif

/* Use re-entrant string tokenize function if available */
#if defined(_POSIX_)
#elif defined(_MSC_VER) && _MSC_VER >= 1400
#define strtok_r(str, delim, saveptr) strtok_s((str), (delim), (saveptr))
#else
#define strtok_r(str, delim, saveptr) strtok((str), (delim))
#endif

typedef struct {
	char* fileName;
	xmlDocPtr root;
	xmlXPathContextPtr xpathCtx;
	ED_LOCALE_TYPE loc;
	ED_LOGGING_FUNC log;
} XML2File;

static void errorFunc(void* ctx, const char* string, ...)
{
	va_list args;
	va_start(args, string);
	ModelicaVFormatMessage(string, args);
	va_end(args);
}

void* ED_createXML2(const char* fileName, const char** ns, size_t sizeNS, int verbose, int detectMissingData)
{
	size_t i;
	XML2File* xml = (XML2File*)malloc(sizeof(XML2File));
	if (xml == NULL) {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	xml->fileName = strdup(fileName);
	if (xml->fileName == NULL) {
		free(xml);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}

	/* Init libxml */
	xmlInitParser();

	xmlSetGenericErrorFunc(NULL, errorFunc);

	if (verbose == 1) {
		/* Print info message, that file is loading */
		ModelicaFormatMessage("... loading \"%s\"\n", fileName);
	}

	/* Load XML document */
	xml->root = xmlReadFile(fileName, NULL, 0);
	if (xml->root == NULL) {
		free(xml->fileName);
		free(xml);
		xmlCleanupParser();
		ModelicaFormatError("Error: Cannot parse file \"%s\"\n", fileName);
		return NULL;
	}

	/* Create XPath evaluation context */
	xml->xpathCtx = xmlXPathNewContext(xml->root);
	if (xml->xpathCtx == NULL) {
		xmlFreeDoc(xml->root);
		free(xml->fileName);
		free(xml);
		xmlCleanupParser();
		ModelicaFormatError("Error: Unable to create new XPath context for file \"%s\"\n", fileName);
		return NULL;
	}

	/* Register namespaces */
	for (i = 0; i < sizeNS; i++) {
		const char* prefix = ns[2*i];
		const char* href = ns[2*i + 1];
		if (xmlXPathRegisterNs(xml->xpathCtx, (const xmlChar*)prefix, (const xmlChar*)href) != 0) {
			xmlXPathFreeContext(xml->xpathCtx);
			xmlFreeDoc(xml->root);
			free(xml->fileName);
			free(xml);
			xmlCleanupParser();
			ModelicaFormatError("Error: Unable to register name space with prefix=\"%s\" and href=\"%s\"\n",
				prefix, href);
		}
	}

	xml->loc = ED_INIT_LOCALE;
	switch (detectMissingData) {
		case ED_LOG_NONE:
			xml->log = ED_LogNone;
			break;
		case ED_LOG_DEBUG:
			xml->log = ED_LogDebug;
			break;
		case ED_LOG_ERROR:
			xml->log = ED_LogError;
			break;
		default:
			xml->log = ED_LogWarning;
			break;
	}
	ED_PTR_ADD(xml);
	return xml;
}

void ED_destroyXML2(void* _xml)
{
	XML2File* xml = (XML2File*)_xml;
	ED_PTR_CHECK(xml);
	if (xml != NULL) {
		xmlXPathFreeContext(xml->xpathCtx);
		xmlFreeDoc(xml->root);
		if (xml->fileName != NULL) {
			free(xml->fileName);
		}
		ED_FREE_LOCALE(xml->loc);
		free(xml);
		ED_PTR_DEL(xml);
	}

	/* Shutdown libxml */
	xmlCleanupParser();
}

double ED_getDoubleFromXML2(void* _xml, const char* varName, int* exist)
{
	double ret = 0.;
	XML2File* xml = (XML2File*)_xml;
	ED_PTR_CHECK(xml);
	if (xml != NULL) {
		/* Evaluate XPath expression */
		xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)varName, xml->xpathCtx);
		*exist = 1;
		if (xpathObj == NULL) {
			ModelicaFormatError("Error: Unable to evaluate XPath expression \"%s\" from file \"%s\"\n",
				varName, xml->fileName);
		}
		else if (xpathObj->nodesetval != NULL && xpathObj->nodesetval->nodeNr == 1) {
			xmlNodePtr cur = xpathObj->nodesetval->nodeTab[0];
			xmlElementType type = cur->type;
			if (type == XML_ATTRIBUTE_NODE || type == XML_ELEMENT_NODE) {
				cur = cur->children;
				type = cur->type;
			}
			if (type == XML_TEXT_NODE) {
				char* token = (char*)cur->content;
				if (token != NULL) {
					if (ED_strtod(token, xml->loc, &ret, ED_STRICT)) {
						int line = (int)cur->line;
						char buf[128];
						memset(buf, 0, 128);
						strncpy(buf, token, 127);
						xmlXPathFreeObject(xpathObj);
						ModelicaFormatError("Error in line %i: Cannot read double value \"%s\" from file \"%s\"\n",
							line, buf, xml->fileName);
					}
					else {
						xmlXPathFreeObject(xpathObj);
					}
				}
				else {
					int line = (int)cur->line;
					*exist = 0;
					xmlXPathFreeObject(xpathObj);
					xml->log("Line %i: Cannot read double value from file \"%s\"\n",
						line, xml->fileName);
				}
			}
			else {
				int line = (int)cur->line;
				*exist = 0;
				xmlXPathFreeObject(xpathObj);
				xml->log("Line %i: No text node found for \"%s\" in file \"%s\"\n",
					line, varName, xml->fileName);
			}
		}
		else if (xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeNr == 0) {
			*exist = 0;
			xmlXPathFreeObject(xpathObj);
			xml->log("No node found for \"%s\" in file \"%s\"\n",
				varName, xml->fileName);
		}
		else if (xpathObj->nodesetval != NULL && xpathObj->nodesetval->nodeNr > 1) {
			xmlXPathFreeObject(xpathObj);
			ModelicaFormatError("Error: More than one node found for \"%s\" in file \"%s\"\n",
				varName, xml->fileName);
		}
		else {
			*exist = 0;
			xmlXPathFreeObject(xpathObj);
		}
	}
	else {
		*exist = 0;
	}
	return ret;
}

const char* ED_getStringFromXML2(void* _xml, const char* varName, int* exist)
{
	XML2File* xml = (XML2File*)_xml;
	ED_PTR_CHECK(xml);
	if (xml != NULL) {
		/* Evaluate XPath expression */
		xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)varName, xml->xpathCtx);
		*exist = 1;
		if (xpathObj == NULL) {
			ModelicaFormatError("Error: Unable to evaluate XPath expression \"%s\" from file \"%s\"\n",
				varName, xml->fileName);
		}
		else if (xpathObj->nodesetval != NULL && xpathObj->nodesetval->nodeNr == 1) {
			xmlNodePtr cur = xpathObj->nodesetval->nodeTab[0];
			xmlElementType type = cur->type;
			if (type == XML_ATTRIBUTE_NODE || type == XML_ELEMENT_NODE) {
				cur = cur->children;
				type = cur->type;
			}
			if (type == XML_TEXT_NODE) {
				char* token = (char*)cur->content;
				if (token != NULL) {
					char* ret = ModelicaAllocateStringWithErrorReturn(strlen(token));
					if (ret != NULL) {
						strcpy(ret, token);
						xmlXPathFreeObject(xpathObj);
						return (const char*)ret;
					}
					else {
						xmlXPathFreeObject(xpathObj);
						ModelicaError("Memory allocation error\n");
					}
				}
				else {
					int line = (int)cur->line;
					*exist = 0;
					xmlXPathFreeObject(xpathObj);
					xml->log("Line %i: Cannot read value from file \"%s\"\n",
						line, xml->fileName);
				}
			}
			else {
				int line = (int)cur->line;
				*exist = 0;
				xmlXPathFreeObject(xpathObj);
				xml->log("Line %i: No text node found for \"%s\" in file \"%s\"\n",
					line, varName, xml->fileName);
			}
		}
		else if (xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeNr == 0) {
			*exist = 0;
			xmlXPathFreeObject(xpathObj);
			xml->log("No node found for \"%s\" in file \"%s\"\n",
				varName, xml->fileName);
		}
		else if (xpathObj->nodesetval != NULL && xpathObj->nodesetval->nodeNr > 1) {
			xmlXPathFreeObject(xpathObj);
			ModelicaFormatError("Error: More than one node found for \"%s\" in file \"%s\"\n",
				varName, xml->fileName);
		}
		else {
			*exist = 0;
			xmlXPathFreeObject(xpathObj);
		}
	}
	else {
		*exist = 0;
	}
	return "";
}

int ED_getIntFromXML2(void* _xml, const char* varName, int* exist)
{
	long ret = 0;
	XML2File* xml = (XML2File*)_xml;
	ED_PTR_CHECK(xml);
	if (xml != NULL) {
		/* Evaluate XPath expression */
		xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)varName, xml->xpathCtx);
		*exist = 1;
		if (xpathObj == NULL) {
			ModelicaFormatError("Error: Unable to evaluate XPath expression \"%s\" from file \"%s\"\n",
				varName, xml->fileName);
		}
		else if (xpathObj->nodesetval != NULL && xpathObj->nodesetval->nodeNr == 1) {
			xmlNodePtr cur = xpathObj->nodesetval->nodeTab[0];
			xmlElementType type = cur->type;
			if (type == XML_ATTRIBUTE_NODE || type == XML_ELEMENT_NODE) {
				cur = cur->children;
				type = cur->type;
			}
			if (type == XML_TEXT_NODE) {
				char* token = (char*)cur->content;
				if (token != NULL) {
					if (ED_strtol(token, xml->loc, &ret, ED_STRICT)) {
						int line = (int)cur->line;
						char buf[128];
						memset(buf, 0, 128);
						strncpy(buf, token, 127);
						xmlXPathFreeObject(xpathObj);
						ModelicaFormatError("Error in line %i: Cannot read int value \"%s\" from file \"%s\"\n",
							line, buf, xml->fileName);
					}
					else {
						xmlXPathFreeObject(xpathObj);
					}
				}
				else {
					int line = (int)cur->line;
					*exist = 0;
					xmlXPathFreeObject(xpathObj);
					xml->log("Line %i: Cannot read int value from file \"%s\"\n",
						line, xml->fileName);
				}
			}
			else {
				int line = (int)cur->line;
				*exist = 0;
				xmlXPathFreeObject(xpathObj);
				xml->log("Line %i: No text node found for \"%s\" in file \"%s\"\n",
					line, varName, xml->fileName);
			}
		}
		else if (xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeNr == 0) {
			*exist = 0;
			xmlXPathFreeObject(xpathObj);
			xml->log("No node found for \"%s\" in file \"%s\"\n",
				varName, xml->fileName);
		}
		else if (xpathObj->nodesetval != NULL && xpathObj->nodesetval->nodeNr > 1) {
			xmlXPathFreeObject(xpathObj);
			ModelicaFormatError("Error: More than one node found for \"%s\" in file \"%s\"\n",
				varName, xml->fileName);
		}
		else {
			*exist = 0;
			xmlXPathFreeObject(xpathObj);
		}
	}
	else {
		*exist = 0;
	}
	return (int)ret;
}

void ED_getDoubleArray1DFromXML2(void* _xml, const char* varName, double* a, size_t n)
{
	XML2File* xml = (XML2File*)_xml;
	ED_PTR_CHECK(xml);
	if (xml != NULL) {
		/* Evaluate XPath expression */
		xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)varName, xml->xpathCtx);
		if (xpathObj == NULL) {
			ModelicaFormatError("Error: Unable to evaluate XPath expression \"%s\" from file \"%s\"\n",
				varName, xml->fileName);
		}
		else if (xpathObj->nodesetval != NULL && xpathObj->nodesetval->nodeNr >= 1) {
			size_t i = 0;
			int j;
			for (j = 0; j < xpathObj->nodesetval->nodeNr; j++) {
				xmlNodePtr cur = xpathObj->nodesetval->nodeTab[j];
				xmlElementType type = cur->type;
				if (type == XML_ATTRIBUTE_NODE || type == XML_ELEMENT_NODE) {
					cur = cur->children;
					type = cur->type;
				}
				if (type == XML_TEXT_NODE) {
					char* token = (char*)cur->content;
					if (token != NULL) {
						char* tokenCopy = strdup(token);
						if (tokenCopy != NULL) {
							char* nextToken = NULL;
							token = strtok_r(tokenCopy, "[]{},; \t", &nextToken);
							while (i < n) {
								if (token != NULL) {
									if (ED_strtod(token, xml->loc, &a[i++], ED_STRICT)) {
										int line = (int)cur->line;
										char buf[128];
										memset(buf, 0, 128);
										strncpy(buf, token, 127);
										xmlXPathFreeObject(xpathObj);
										free(tokenCopy);
										ModelicaFormatError("Error in line %i: Cannot read double value \"%s\" from file \"%s\"\n",
											line, token, xml->fileName);
										return;
									}
									token = strtok_r(NULL, "[]{},; \t", &nextToken);
								}
								else {
									break;
								}
							}
							free(tokenCopy);
						}
						else {
							xmlXPathFreeObject(xpathObj);
							ModelicaError("Memory allocation error\n");
						}
					}
					else {
						int line = (int)cur->line;
						xmlXPathFreeObject(xpathObj);
						ModelicaFormatError("Error in line %i: Cannot read empty element \"%s\" in file \"%s\"\n",
							line, varName, xml->fileName);
					}
				}
				else {
					int line = (int)cur->line;
					xmlXPathFreeObject(xpathObj);
					ModelicaFormatError("Error in line %i: No text node found for \"%s\" in file \"%s\"\n",
						line, varName, xml->fileName);
				}
			}
			if (i + 1 < n) {
				xmlNodePtr cur = xpathObj->nodesetval->nodeTab[xpathObj->nodesetval->nodeNr - 1];
				int line = (int)cur->line;
				xmlXPathFreeObject(xpathObj);
				ModelicaFormatError("Error in line %i: Cannot read empty (%lu.) element \"%s\" in file \"%s\"\n",
					line, (unsigned long)(i + 1), varName, xml->fileName);
				return;
			}
			xmlXPathFreeObject(xpathObj);
		}
		else if (xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeNr == 0) {
			xmlXPathFreeObject(xpathObj);
			ModelicaFormatError("No node found for \"%s\" in file \"%s\"\n",
				varName, xml->fileName);
		}
		else {
			xmlXPathFreeObject(xpathObj);
		}
	}
}

void ED_getDoubleArray2DFromXML2(void* _xml, const char* varName, double* a, size_t m, size_t n)
{
	ED_getDoubleArray1DFromXML2(_xml, varName, a, m*n);
}

void ED_getArray1DDimensionFromXML2(void* _xml, const char* varName, int* n)
{
	int m;
	ED_getArray2DDimensionsFromXML2(_xml, varName, &m, n);
	*n *= m;
}

void ED_getArray2DDimensionsFromXML2(void* _xml, const char* varName, int* m, int* n)
{
	XML2File* xml = (XML2File*)_xml;
	int _m = 0;
	int _n = 0;
	if (NULL != m)
		*m = 0;
	if (NULL != n)
		*n = 0;
	ED_PTR_CHECK(xml);
	if (xml != NULL) {
		/* Evaluate XPath expression */
		xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression((const xmlChar*)varName, xml->xpathCtx);
		if (xpathObj == NULL) {
			ModelicaFormatError("Error: Unable to evaluate XPath expression \"%s\" from file \"%s\"\n",
				varName, xml->fileName);
		}
		else if (xpathObj->nodesetval != NULL && xpathObj->nodesetval->nodeNr >= 1) {
			xmlNodePtr cur = xpathObj->nodesetval->nodeTab[0];
			xmlElementType type = cur->type;
			if (type == XML_ATTRIBUTE_NODE || type == XML_ELEMENT_NODE) {
				cur = cur->children;
				type = cur->type;
			}
			if (type == XML_TEXT_NODE) {
				char* token = (char*)cur->content;
				if (token != NULL) {
					char* tokenCopy = strdup(token);
					if (tokenCopy != NULL) {
						char* nextToken = NULL;
						char* sep = strchr(tokenCopy, ';');
						_m = 1;
						if (NULL != sep) {
							do {
								_m++;
								sep = strchr(sep + 1, ';');
							} while (NULL != sep);
						}
						else {
							sep = strchr(tokenCopy, '}');
							while (NULL != sep) {
								sep = strchr(sep + 1, ',');
								if (NULL != sep) {
									_m++;
									sep = strchr(sep + 1, '}');
								}
							}
						}
						token = strtok_r(tokenCopy, "[]{},; \t", &nextToken);
						if (NULL != token) {
							do {
								_n++;
								token = strtok_r(NULL, "[]{},; \t", &nextToken);
							} while (NULL != token);
							if (0 == _n%_m) {
								_n /= _m;
							}
							else {
								_m = 1;
							}
						}
						_m *= xpathObj->nodesetval->nodeNr;
						free(tokenCopy);
					}
				}
			}
		}
		else if (xpathObj->nodesetval == NULL || xpathObj->nodesetval->nodeNr == 0) {
			xmlXPathFreeObject(xpathObj);
			ModelicaFormatError("No node found for \"%s\" in file \"%s\"\n",
				varName, xml->fileName);
		}
		else {
			xmlXPathFreeObject(xpathObj);
		}
	}
	if (NULL != m)
		*m = _m;
	if (NULL != n)
		*n = _n;
}
