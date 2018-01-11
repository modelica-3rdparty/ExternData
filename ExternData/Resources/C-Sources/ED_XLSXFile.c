/* ED_XLSXFile.c - Excel XLSX functions
 *
 * Copyright (C) 2015-2018, tbeu
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

#include <stdint.h>
#include <string.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include <ctype.h>
#include "ED_locale.h"
#include "ED_ptrtrack.h"
#include "bsxml.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_XLSXFile.h"
#include "unzip.h"
#define HASH_NONFATAL_OOM 1
#include "uthash.h"

#define E_NO_MEMORY (11)
#define E_BAD_DATA (12)
#define E_EOPEN (15)
#define E_ECLOSE (17)
#define E_EREAD (18)
#define E_ELOCATE (30)
#define E_EGETFILEINFO (31)

#define WB_XML "xl/workbook.xml"
#define STR_XML "xl/sharedStrings.xml"

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

void* ED_createXLSX(const char* fileName, int verbose)
{
	size_t i;
	int rc;
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

	if (verbose == 1) {
		/* Print info message, that file is loading */
		ModelicaFormatMessage("... loading \"%s\"\n", fileName);
	}

	xlsx->zfile = unzOpen(fileName);
	if (xlsx->zfile == NULL) {
		free(xlsx->fileName);
		free(xlsx);
		ModelicaFormatError("Cannot open file \"%s\"\n", fileName);
		return NULL;
	}
	rc = parseXML(xlsx->zfile, WB_XML, &root);
	if (rc != 0) {
		unzClose(xlsx->zfile);
		free(xlsx->fileName);
		free(xlsx);
		switch (rc) {
			case E_NO_MEMORY:
				ModelicaError("Memory allocation error\n");
				break;
			case E_ELOCATE:
				ModelicaFormatError("Cannot locate %s in file \"%s\"\n", WB_XML, fileName);
				break;
			case E_EOPEN:
				ModelicaFormatError("Cannot open %s in file \"%s\"\n", WB_XML, fileName);
				break;
			case E_EGETFILEINFO:
				ModelicaFormatError("Cannot get file info of %s in file \"%s\"\n", WB_XML, fileName);
				break;
			case E_EREAD:
				ModelicaFormatError("Cannot read file %s in file \"%s\"\n", WB_XML, fileName);
				break;
			case E_BAD_DATA:
				ModelicaFormatError("Cannot parse file %s of file \"%s\"\n", WB_XML, fileName);
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
			char* sheetName = XmlNode_getAttributeValue(child, "name");
			char* sheetId = XmlNode_getAttributeValue(child, "sheetId");
			if (sheetName != NULL && sheetId != NULL) {
				SheetShare* iter = malloc(sizeof(SheetShare));
				if (iter != NULL) {
					iter->sheetName = strdup(sheetName);
					iter->sheetId = strdup(sheetId);
					iter->root = NULL;
					HASH_ADD_KEYPTR(hh, xlsx->sheets, iter->sheetName, strlen(iter->sheetName), iter);
					if (NULL == iter->hh.tbl) {
						free(iter->sheetName);
						free(iter->sheetId);
						free(iter);
					}
				}
			}
		}
	}

	XmlNode_deleteTree(root);
	parseXML(xlsx->zfile, STR_XML, &xlsx->sroot);

	xlsx->loc = ED_INIT_LOCALE;
	ED_PTR_ADD(xlsx);
	return xlsx;
}

void ED_destroyXLSX(void* _xlsx)
{
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	ED_PTR_CHECK(xlsx);
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
		XmlNode_deleteTree(xlsx->sroot);
		free(xlsx);
		ED_PTR_DEL(xlsx);
	}
}

static void rc1(const char* cellAddress, WORD* row, WORD* col)
{
	WORD i = 0, j;
	while (cellAddress[i++] >= 'A');
	i--;
	/* i now points to first character of row address */
	*col = 0;
	for (j = 0; j < i; j++) {
		*col *= 26;
		*col += toupper(cellAddress[j]) - 'A' + 1;
	}
	*row = (WORD)atoi(cellAddress + i);
}

static void rc(const char* cellAddress, WORD* row, WORD* col)
{
	rc1(cellAddress, row, col);
	if (*col > 0)
		(*col)--;
	if (*row > 0)
		(*row)--;
}

static void ca(char* colAddress, WORD idx)
{
	char c = (idx % 26) + 'A';
	WORD quotient = idx/26;
	if (quotient > 0) {
		ca(colAddress, quotient - 1);
	}
	strncat(colAddress, &c, 1);
}

static XmlNodeRef findSheet(XLSXFile* xlsx, char** sheetName)
{
	SheetShare* iter;
	XmlNodeRef root;

	if (strlen(*sheetName) == 0) {
		SheetShare* tmp;
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
		ModelicaFormatMessage("Cannot find sheet name \"%s\" in file \"%s\" of file \"%s\"\n",
			*sheetName, WB_XML, xlsx->fileName);
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

static int XmlNode_Rowcomparer(const void* a, const void* b)
{
	return strcmp(
		(XmlNode_getAttribute((XmlNode *)a, "r"))->value,
		(XmlNode_getAttribute((XmlNode *)b, "r"))->value);
}

static XmlNodeRef XmlNode_findRow(XmlNodeRef node, const char* row)
{
	XmlNodeRef ret;
	XmlNode tmpNode = {0};
	tmpNode.m_type = NODE_CHILD;
	tmpNode.m_attributes = cpo_array_create(1, sizeof(struct XmlAttribute));
	XmlNode_setAttribute(&tmpNode, "r", row);
	ret = (XmlNodeRef)cpo_array_bsearch(node->m_childs, &tmpNode, XmlNode_Rowcomparer);
	XmlNode_delete(&tmpNode);
	return ret;
}

static char* findCellValueFromRow(XLSXFile* xlsx, const char* cellAddress, XmlNodeRef root, const char* sheetName)
{
	char* token = NULL;
	XmlNodeRef iter = XmlNode_findRow(root, cellAddress);
	if (iter != NULL) {
		char* t = XmlNode_getAttributeValue(iter, "t");
		if (t != NULL && 0 == strncmp(t, "s", 1)) {
			/* Shared string */
			XmlNodeRef ites = XmlNode_getChild(iter, 0);
			iter = NULL;
			if (ites != NULL) {
				XmlNode_getValue(ites, &token);
				if (token != NULL) {
					long idx = 0;
					if (!ED_strtol(token, xlsx->loc, &idx, ED_STRICT)) {
						if (xlsx->sroot != NULL && (size_t)idx < XmlNode_getChildCount(xlsx->sroot)) {
							iter = XmlNode_getChild(xlsx->sroot, (int)idx);
						}
					}
					token = NULL;
				}
			}
		}
		if (iter != NULL) {
			iter = XmlNode_getChild(iter, 0);
			if (iter != NULL) {
				XmlNode_getValue(iter, &token);
			}
		}
	}
	return token;
}

static XmlNodeRef findRow(XLSXFile* xlsx, const char* cellAddress, XmlNodeRef root, const char* sheetName)
{
	XmlNodeRef iter = XmlNode_findChild(root, "sheetData");
	if (iter != NULL) {
		WORD i = 0;
		while (cellAddress[i++] >= 'A');
		iter = XmlNode_findRow(iter, &cellAddress[--i]);
	}
	else {
		ModelicaFormatError("Cannot find \"sheetData\" in sheet \"%s\" from file \"%s\"\n",
			sheetName, xlsx->fileName);
	}
	return iter;
}

static char* findCellValue(XLSXFile* xlsx, const char* cellAddress, XmlNodeRef root, const char* sheetName)
{
	char* token = NULL;
	XmlNodeRef iter = findRow(xlsx, cellAddress, root, sheetName);
	if (iter != NULL) {
		token = findCellValueFromRow(xlsx, cellAddress, iter, sheetName);
	}
	return token;
}

static void findBlankCell(WORD row, WORD col, XmlNodeRef root, int* isBlank)
{
	/* Check if blank cell by dimension ref */
	const XmlNodeRef dim = XmlNode_findChild(root, "dimension");
	*isBlank = 0;
	if (NULL != dim) {
		char* ref = XmlNode_getAttributeValue(dim, "ref");
		if (NULL != ref) {
			char* colon = strchr(ref, ':');
			if (NULL != colon) {
				WORD refRow = 0, refCol = 0;
				rc(ref, &refRow, &refCol);
				if (row >= refRow && col >= refCol) {
					rc(++colon, &refRow, &refCol);
					if (row <= refRow && col <= refCol) {
						*isBlank = 1;
					}
				}
			}
		}
	}
}

double ED_getDoubleFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist)
{
	double ret = 0.;
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	ED_PTR_CHECK(xlsx);
	if (xlsx != NULL) {
		char* _sheetName = (char*)sheetName;
		const XmlNodeRef root = findSheet(xlsx, &_sheetName);
		if (root != NULL) {
			char* token = findCellValue(xlsx, cellAddress, root, _sheetName);
			*exist = 1;
			if (token != NULL) {
				if (ED_strtod(token, xlsx->loc, &ret, ED_STRICT)) {
					ModelicaFormatError("Cannot read double value \"%s\" from file \"%s\"\n",
						token, xlsx->fileName);
				}
			}
			else {
				WORD row = 0, col = 0;
				rc(cellAddress, &row, &col);
				findBlankCell(row, col, root, exist);
				if (*exist == 1) {
					ModelicaFormatMessage("Found blank cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xlsx->fileName);
				}
				else {
					ModelicaFormatMessage("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xlsx->fileName);
				}
			}
		}
		else {
			ModelicaFormatMessage("Cannot find \"sheetData\" in sheet \"%s\" from file \"%s\"\n",
				_sheetName, xlsx->fileName);
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return ret;
}

const char* ED_getStringFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist)
{
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	ED_PTR_CHECK(xlsx);
	if (xlsx != NULL) {
		char* _sheetName = (char*)sheetName;
		const XmlNodeRef root = findSheet(xlsx, &_sheetName);
		if (root != NULL) {
			char* token = findCellValue(xlsx, cellAddress, root, _sheetName);
			*exist = 1;
			if (token != NULL) {
				char* ret = ModelicaAllocateString(strlen(token));
				strcpy(ret, token);
				return (const char*)ret;
			}
			else {
				WORD row = 0, col = 0;
				rc(cellAddress, &row, &col);
				findBlankCell(row, col, root, exist);
				if (*exist == 1) {
					ModelicaFormatMessage("Found blank cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xlsx->fileName);
				}
				else {
					ModelicaFormatMessage("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xlsx->fileName);
				}
			}
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

int ED_getIntFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist)
{
	long ret = 0;
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	ED_PTR_CHECK(xlsx);
	if (xlsx != NULL) {
		char* _sheetName = (char*)sheetName;
		const XmlNodeRef root = findSheet(xlsx, &_sheetName);
		if (root != NULL) {
			char* token = findCellValue(xlsx, cellAddress, root, _sheetName);
			*exist = 1;
			if (token != NULL) {
				if (ED_strtol(token, xlsx->loc, &ret, ED_STRICT)) {
					ModelicaFormatError("Cannot read int value \"%s\" from file \"%s\"\n",
						token, xlsx->fileName);
				}
			}
			else {
				WORD row = 0, col = 0;
				rc(cellAddress, &row, &col);
				findBlankCell(row, col, root, exist);
				if (*exist == 1) {
					ModelicaFormatMessage("Found blank cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xlsx->fileName);
				}
				else {
					ModelicaFormatMessage("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xlsx->fileName);
				}
			}
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

void ED_getDoubleArray2DFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, double* a, size_t m, size_t n)
{
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	ED_PTR_CHECK(xlsx);
	if (xlsx != NULL) {
		char* _sheetName = (char*)sheetName;
		const XmlNodeRef root = findSheet(xlsx, &_sheetName);
		if (root != NULL) {
			WORD row = 0, col = 0;
			WORD i, j;
			char cell[63];
			char tmp[63];
			rc(cellAddress, &row, &col);
			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {
					char* token;
					tmp[0] = '\0';
					ca(tmp, col + j);
					sprintf(cell, "%s%u", tmp, (unsigned int)(row + i + 1));
					token = findCellValue(xlsx, cell, root, _sheetName);
					if (token != NULL) {
						if (ED_strtod(token, xlsx->loc, &a[i*n + j], ED_STRICT)) {
							ModelicaFormatError("Error in cell (%u,%u) when reading double value \"%s\" from sheet \"%s\" of file \"%s\"\n",
								(unsigned int)(row + i), (unsigned int)(col + j), token, _sheetName, xlsx->fileName);
						}
					}
					else {
						int exist;
						findBlankCell(row + i, col + j, root, &exist);
						a[i*n + j] = 0.;
						if (exist == 1) {
							ModelicaFormatMessage("Found blank cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
								(unsigned int)(row +i), (unsigned int)(col + j), _sheetName, xlsx->fileName);
						}
						else {
							ModelicaFormatMessage("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
								(unsigned int)(row +i), (unsigned int)(col + j), _sheetName, xlsx->fileName);
						}
					}
				}
			}
		}
	}
}

void ED_getArray2DDimensionsFromXLSX(void* _xlsx, const char* sheetName, int* m, int* n)
{
	XLSXFile* xlsx = (XLSXFile*)_xlsx;
	int _m = 0;
	int _n = 0;
	if (NULL != m)
		*m = 0;
	if (NULL != n)
		*n = 0;
	ED_PTR_CHECK(xlsx);
	if (xlsx != NULL) {
		char* _sheetName = (char*)sheetName;
		const XmlNodeRef root = findSheet(xlsx, &_sheetName);
		if (root != NULL) {
			const XmlNodeRef dim = XmlNode_findChild(root, "dimension");
			if (NULL != dim) {
				char* ref = XmlNode_getAttributeValue(dim, "ref");
				if (NULL != ref) {
					char* colon = strchr(ref, ':');
					if (NULL != colon) {
						WORD row = 0, col = 0;
						rc1(++colon, &row, &col);
						_m = (int)row;
						_n = (int)col;
					}
				}
			}
		}
	}
	if (NULL != m)
		*m = _m;
	if (NULL != n)
		*n = _n;
}
