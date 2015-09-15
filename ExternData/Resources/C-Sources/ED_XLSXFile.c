#if !defined(ED_XLSXFILE_C)
#define ED_XLSXFILE_C

#if defined(__gnu_linux__)
#define _GNU_SOURCE 1
#endif

#include <stdint.h>
#include <string.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include <ctype.h>
#include "ED_locale.h"
#include "bsxml.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_XLSXFile.h"
#include "unzip.h"
#define uthash_fatal(msg) ModelicaFormatMessage("Error: %s\n", msg); break
#include "uthash.h"

#define E_NO_MEMORY (11)
#define E_BAD_DATA (12)
#define E_EOPEN (15)
#define E_ECLOSE (17)
#define E_EREAD (18)
#define E_ELOCATE (30)
#define E_EGETFILEINFO (31)

#define WB "xl/workbook.xml"

typedef uint16_t WORD;

typedef struct {
	char* sheetName;
	char* sheetId;
	XmlNodeRef root;
	UT_hash_handle hh; /* Hashable structure */
} SheetShare;

typedef struct {
	char* fileName;
	ED_LOCALE_TYPE loc;
	unzFile zfile;
	XmlNodeRef sroot; /* Shared strings */
	SheetShare* sheets;
} XLSXFile;

static int parseXML(unzFile zfile, const char* fileName, XmlNodeRef* root)
{
	unz_file_info info;
	char* buf;
	int rc;
	XmlParser xmlParser;
	rc = unzLocateFile(zfile, fileName, 1);
	if (rc != UNZ_OK) {
		return E_ELOCATE;
	}
	rc = unzOpenCurrentFile(zfile);
	if (rc != UNZ_OK) {
		return E_EOPEN;
	}
	rc = unzGetCurrentFileInfo(zfile, &info, NULL, 0, NULL, 0, NULL, 0);
	if (rc != UNZ_OK) {
		return E_EGETFILEINFO;
	}
	buf = malloc(info.uncompressed_size + 1);
	if (buf == NULL) {
		return E_NO_MEMORY;
	}
	rc = unzReadCurrentFile(zfile, buf, info.uncompressed_size);
	if (rc < 0) {
		free(buf);
		return E_EREAD;
	}
	buf[info.uncompressed_size] = '\0';
	*root = XmlParser_parse(&xmlParser, buf);
	free(buf);
	if (*root == NULL) {
		return E_BAD_DATA;
	}

	return 0;
}

void* ED_createXLSX(const char* fileName)
{
	int i, rc;
	XmlNodeRef root;
	XmlNodeRef sheets;
	XLSXFile* xlsx = (XLSXFile*)malloc(sizeof(XLSXFile));
	if (xlsx == NULL) {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	xlsx->fileName = strdup(fileName);
	if (xlsx->fileName == NULL) {
		free(xlsx);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	
	xlsx->zfile = unzOpen(fileName);
	if (xlsx->zfile == NULL) {
		free(xlsx->fileName);
		free(xlsx);
		ModelicaFormatError("Cannot open file \"%s\"\n", fileName);
		return NULL;
	}
	rc = parseXML(xlsx->zfile, WB, &root);
	if (rc != 0) {
		unzClose(xlsx->zfile);
		free(xlsx->fileName);
		free(xlsx);
		switch (rc) {
			case E_NO_MEMORY:
				ModelicaError("Memory allocation error\n");
				break;
			case E_ELOCATE:
				ModelicaFormatError("Cannot locate %s in file \"%s\"\n", WB, fileName);
				break;
			case E_EOPEN:
				ModelicaFormatError("Cannot open %s in file \"%s\"\n", WB, fileName);
				break;
			case E_EGETFILEINFO:
				ModelicaFormatError("Cannot get file info of %s in file \"%s\"\n", WB, fileName);
				break;
			case E_EREAD:
				ModelicaFormatError("Cannot read file %s in file \"%s\"\n", WB, fileName);
				break;
			case E_BAD_DATA:
				ModelicaFormatError("Cannot parse file %s of file \"%s\"\n", WB, fileName);
				break;
			default:
				break;
		}
		return NULL;
	}

	sheets = XmlNode_findChild(root, "sheets");
	if (sheets == NULL) {
		unzClose(xlsx->zfile);
		XmlNode_deleteTree(root);
		free(xlsx->fileName);
		free(xlsx);
		ModelicaFormatError("Cannot find any sheet in file \"%s\"\n", fileName);
		return NULL;
	}
	xlsx->sheets = NULL;
	for (i = 0; i < XmlNode_getChildCount(sheets); i++) {
		XmlNodeRef child = XmlNode_getChild(sheets, i);
		if (XmlNode_isTag(child, "sheet")) {
			SheetShare* iter;
			char* sheetName = XmlNode_getAttributeValue(child, "name");
			char* sheetId = XmlNode_getAttributeValue(child, "sheetId");
			HASH_FIND_STR(xlsx->sheets, sheetName, iter);
			if (iter == NULL) {
				iter = malloc(sizeof(SheetShare));
				if (iter != NULL) {
					iter->sheetName = strdup(sheetName);
					iter->sheetId = strdup(sheetId);
					iter->root = NULL;
					HASH_ADD_KEYPTR(hh, xlsx->sheets, sheetName, strlen(sheetName), iter);
				}
			}
		}
	}

	XmlNode_deleteTree(root);
	parseXML(xlsx->zfile, "xl/sharedStrings.xml", &xlsx->sroot);
	
	xlsx->loc = ED_INIT_LOCALE;
	return xlsx;
}

void ED_destroyXLSX(void* _xlsx)
{
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	if (xlsx != NULL) {
		SheetShare* iter;
		SheetShare* tmp;
		if (xlsx->fileName != NULL) {
			free(xlsx->fileName);
		}
		ED_FREE_LOCALE(xlsx->loc);
		unzClose(xlsx->zfile);
		HASH_ITER(hh, xlsx->sheets, iter, tmp) {
			free(iter->sheetName);
			free(iter->sheetId);
			XmlNode_deleteTree(iter->root);
			HASH_DEL(xlsx->sheets, iter);
			free(iter);
		}
		free(xlsx);
	}
}

static void rc(const char* cellAddr, WORD* row, WORD* col)
{
	WORD i = 0, j, colVal = 0, rowVal;
	while (cellAddr[i++] >= 'A');
	i--;
	/* i now points to first character of row address */
	for (j = 0; j < i; j++) {
		colVal = 26*colVal + toupper(cellAddr[j]) - 'A' + 1;
	}
	*col = colVal > 0 ? (colVal - 1) : 0;
	rowVal = (WORD)atoi(cellAddr + i);
	*row =  rowVal > 0 ? (rowVal - 1) : 0;
}

static XmlNodeRef findSheet(XLSXFile* xlsx, char** sheetName)
{
	SheetShare* iter;
	SheetShare* tmp;
	XmlNodeRef root;

	if (strlen(*sheetName) == 0) {
		/* Resolve default sheet name */
		HASH_ITER(hh, xlsx->sheets, iter, tmp) {
			if (0 == strcmp(iter->sheetId, "1")) {
				*sheetName = iter->sheetName;
				break;
			}
		}
	}
		
	HASH_FIND_STR(xlsx->sheets, *sheetName, iter);
	if (iter == NULL) {
		ModelicaFormatError("Cannot find sheet name \"%s\" in file \"%s\" of file \"%s\"\n",
			*sheetName, WB, xlsx->fileName);
		return NULL;
	}

	if (iter->root != NULL) {
		root = iter->root;
	}	
	else {
		const char* sp = "xl/worksheets/sheet";
		char* s = malloc((strlen(sp) + strlen(iter->sheetId) + strlen(".xml") + 1)*sizeof(char));
		if (s == NULL) {
			ModelicaError("Memory allocation error\n");
			return NULL;
		}
		strcpy(s, sp);
		strcat(s, iter->sheetId);
		strcat(s, ".xml");
		parseXML(xlsx->zfile, s, &root);
		free(s);
		iter->root = root;
	}

	return root;
}

double ED_getDoubleFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName)
{
	double ret = 0.;
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	if (xlsx != NULL) {
		char* _sheetName = (char*)sheetName;
		XmlNodeRef root;
		XmlNodeRef iter;
		WORD row = 0, col = 0;

		root = findSheet(xlsx, &_sheetName);
		rc(cellAddress, &row, &col);
		iter = XmlNode_findChild(root, "sheetData");
		if (row < XmlNode_getChildCount(iter)) {
			iter = XmlNode_getChild(iter, row);
			if (col < XmlNode_getChildCount(iter)) {
				char* t;
				iter = XmlNode_getChild(iter, col);
				t = XmlNode_getAttributeValue(iter, "t");
				if (0 == strcmp(t, "s")) {
					/* Shared string */
				}
				else {
					char* token = NULL;
					iter = XmlNode_getChild(iter, 0);
					XmlNode_getValue(iter, &token);
					if (token != NULL) {
						if (ED_strtod(token, xlsx->loc, &ret)) {
							ModelicaFormatError("Error\n");
						}
					}
					else {
						ModelicaFormatError("Error\n");
					}
				}
			}
		}

	}
	return ret;
}

const char* ED_getStringFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName)
{
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	if (xlsx != NULL) {
	}
	return "";
}

int ED_getIntFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName)
{
	int ret = 0;
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	if (xlsx != NULL) {
	}
	return ret;
}

#endif
