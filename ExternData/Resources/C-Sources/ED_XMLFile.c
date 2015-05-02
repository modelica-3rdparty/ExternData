#if !defined(ED_XMLFILE_C)
#define ED_XMLFILE_C

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "bsxml.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_XMLFile.h"

typedef struct {
	char* fileName;
	XmlNodeRef root;
	_locale_t loc;
} XMLFile;

void* ED_createXML(const char* fileName) {
	XMLFile* xml = NULL;
	XmlParser xmlParser;
	XmlNodeRef root = XmlParser_parse_file(&xmlParser, fileName);
	if (root == NULL) {
		ModelicaFormatError("Cannot parse file \"%s\"\n", fileName);
	}
	xml = (XMLFile*)malloc(sizeof(XMLFile));
	if (xml) {
		xml->fileName = _strdup(fileName);
		if (xml->fileName == NULL) {
			free(xml);
			xml = NULL;
			ModelicaError("Memory allocation error\n");
		}
		xml->loc = _create_locale(LC_NUMERIC, "C");
		xml->root = root;
	}
	else {
		ModelicaError("Memory allocation error\n");
	}
	return xml;
}

void ED_destroyXML(void* _xml)
{
	XMLFile* xml = (XMLFile*)_xml;
	if (xml) {
		if (xml->fileName) {
			free(xml->fileName);
		}
		XmlNode_deleteTree(xml->root);
		_free_locale(xml->loc);
		free(xml);
	}
}

static char* findValue(XmlNodeRef* root, const char* varName, const char* fileName)
{
	char* token = NULL;
	char* buf = _strdup(varName);
	if (buf) {
		int elementError = 0;
		strcpy(buf, varName);
		token = strtok(buf, ".");
		if (token == NULL) {
			elementError = 1;
		}
		while (token && elementError == 0) {
			int i;
			int foundToken = 0;
			for (i = 0; i < XmlNode_getChildCount(*root); i++) {
				XmlNodeRef child = XmlNode_getChild(*root, i);
				if (XmlNode_isTag(child, token)) {
					*root = child;
					token = strtok(NULL, ".");
					foundToken = 1;
					break;
				}
			}
			if (foundToken == 0) {
				elementError = 1;
			}
		}
		free(buf);
		if (elementError == 1) {
			ModelicaFormatError("Error in line %i when reading element %s from file \"%s\"\n",
				XmlNode_getLine(*root), varName, fileName);
		}
		XmlNode_getValue(*root, &token);
	}
	else {
		ModelicaError("Memory allocation error\n");
	}
	return token;
}

double ED_getDoubleFromXML(void* _xml, const char* varName)
{
	double ret = 0.;
	XMLFile* xml = (XMLFile*)_xml;
	if (xml) {
		XmlNodeRef root = xml->root;
		char* token = findValue(&root, varName, xml->fileName);
		if (token) {
			char* endptr;
			ret = _strtod_l(token, &endptr, xml->loc);
			if (*endptr != 0) {
				ret = 0.;
				ModelicaFormatError("Error in line %i when reading double value %s from file \"%s\"\n",
					XmlNode_getLine(root), token, xml->fileName);
			}
		}
		else {
			ModelicaFormatError("Error in line %i when reading double value from file \"%s\"\n",
				XmlNode_getLine(root), xml->fileName);
		}
	}
	return ret;
}

const char* ED_getStringFromXML(void* _xml, const char* varName)
{
	XMLFile* xml = (XMLFile*)_xml;
	if (xml) {
		XmlNodeRef root = xml->root;
		char* token = findValue(&root, varName, xml->fileName);
		if (token) {
			char* ret = ModelicaAllocateString(strlen(token));
			strcpy(ret, token);
			return (const char*)ret;
		}
		else {
			ModelicaFormatError("Error in line %i when reading value from file \"%s\"\n",
				XmlNode_getLine(root), xml->fileName);
		}
	}
	return "";
}

int ED_getIntFromXML(void* _xml, const char* varName)
{
	int ret = 0;
	XMLFile* xml = (XMLFile*)_xml;
	if (xml) {
		XmlNodeRef root = xml->root;
		char* token = findValue(&root, varName, xml->fileName);
		if (token) {
			char* endptr;
			ret = (int)_strtol_l(token, &endptr, 10, xml->loc);
			if (*endptr != 0) {
				ret = 0;
				ModelicaFormatError("Error in line %i when reading int value %s from file \"%s\"\n",
					XmlNode_getLine(root), token, xml->fileName);
			}
		}
		else {
			ModelicaFormatError("Error in line %i when reading int value from file \"%s\"\n",
				XmlNode_getLine(root), xml->fileName);
		}
	}
	return ret;
}

void ED_getDoubleArray1DFromXML(void* _xml, const char* varName, double* a, size_t n)
{
	XMLFile* xml = (XMLFile*)_xml;
	if (xml) {
		XmlNodeRef root = xml->root;
		char* token = findValue(&root, varName, xml->fileName);
		if (token) {
			char* buf = _strdup(token);
			if (buf) {
				size_t i;
				strcpy(buf, token);
				token = strtok(buf, "[]{},; \t");
				for (i = 0; i < n; i++) {
					char* endptr;
					a[i] = _strtod_l(token, &endptr, xml->loc);
					if (*endptr != 0) {
						a[i] = 0.;
						ModelicaFormatError("Error in line %i when reading double value %s from file \"%s\"\n",
							XmlNode_getLine(root), token, xml->fileName);
					}
					token = strtok(NULL, "[]{},; \t");
				}
				free(buf);
			}
		}
		else {
			ModelicaFormatError("Error in line %i when reading double value from file \"%s\"\n",
				XmlNode_getLine(root), xml->fileName);
		}
	}
}

void ED_getDoubleArray2DFromXML(void* _xml, const char* varName, double* a, size_t m, size_t n)
{
	ED_getDoubleArray1DFromXML(_xml, varName, a, m*n);
}

#endif
