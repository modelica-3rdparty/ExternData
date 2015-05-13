#if !defined(ED_XLSFILE_C)
#define ED_XLSFILE_C

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "ModelicaUtilities.h"
#include "libxls/xls.h"
#include "../Include/ED_XLSFile.h"
#define uthash_fatal(msg) ModelicaFormatMessage("Error: %s\n", msg); break
#include "uthash.h"

typedef struct SheetShare {
	char* sheetName;
	xlsWorkSheet* pWS;
	UT_hash_handle hh; /* Hashable structure */
} SheetShare;

typedef struct {
	char* fileName;
	_locale_t loc;
	xlsWorkBook* pWB;
	SheetShare* sheets;
} XLSFile;

void* ED_createXLS(const char* fileName, const char* encoding)
{
	XLSFile* xls = NULL;
	xlsWorkBook* pWB = xls_open(fileName, encoding);
	if (!pWB) {
		ModelicaFormatError("Cannot open file \"%s\"\n", fileName);
		return xls;
	}
	xls = (XLSFile*)malloc(sizeof(XLSFile));
	if (xls) {
		xls->fileName = _strdup(fileName);
		if (xls->fileName == NULL) {
			free(xls);
			xls = NULL;
			ModelicaError("Memory allocation error\n");
		}
		xls->loc = _create_locale(LC_NUMERIC, "C");
		xls->pWB = pWB;
		xls->sheets = NULL;
	}
	else {
		ModelicaError("Memory allocation error\n");
	}
	return xls;
}

void ED_destroyXLS(void* _xls)
{
	XLSFile* xls = (XLSFile*)_xls;
	if (xls) {
		SheetShare* iter;
		SheetShare* tmp;
		if (xls->fileName) {
			free(xls->fileName);
		}
		_free_locale(xls->loc);
		HASH_ITER(hh, xls->sheets, iter, tmp) {
			free(iter->sheetName);
			xls_close_WS(iter->pWS);
			HASH_DEL(xls->sheets, iter);
			free(iter);
		}
		xls_close(xls->pWB);
		free(xls);
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

static xlsWorkSheet* findSheet(XLSFile* xls, const char** sheetName)
{
	SheetShare* iter;
	xlsWorkSheet* pWS = NULL;

	if (xls->pWB->sheets.count == 0) {
		ModelicaFormatError("Cannot find any sheet in file \"%s\"\n",
			xls->fileName);
		return pWS;
	}

	if (strlen(*sheetName) == 0) {
		/* Resolve default sheet name */
		*sheetName = (char*)xls->pWB->sheets.sheet[0].name;
	}

	HASH_FIND_STR(xls->sheets, *sheetName, iter);
	if (iter) {
		pWS = iter->pWS;
	}
	else {
		int sheet = -1;
		DWORD i;
		/* Process all sheets */
		for (i = 0; i < xls->pWB->sheets.count; i++) {
			if (0 == strcmp(*sheetName, (char*)xls->pWB->sheets.sheet[i].name)) {
				sheet = (int)i;
				break;
			}
		}
		if (sheet < 0) {
			ModelicaFormatError("Cannot find sheet \"%s\" in file \"%s\"\n",
				*sheetName, xls->fileName);
			return NULL;
		}
		/* Open and parse the sheet */
		pWS = xls_getWorkSheet(xls->pWB, sheet);
		xls_parseWorkSheet(pWS);
		iter = malloc(sizeof(SheetShare));
		if (iter) {
			iter->sheetName = _strdup(*sheetName);
			iter->pWS = pWS;
			HASH_ADD_KEYPTR(hh, xls->sheets, *sheetName, strlen(*sheetName), iter);
		}
	}
	return pWS;
}

double ED_getDoubleFromXLS(void* _xls, const char* cellAddress, const char* sheetName)
{
	double ret = 0.;
	XLSFile* xls = (XLSFile*)_xls;
	if (xls) {
		char* _sheetName = (char*)sheetName;
		xlsWorkSheet* pWS = findSheet(xls, &_sheetName);
		xlsCell* cell;
		WORD row = 0, col = 0;

		rc(cellAddress, &row, &col);
		cell = xls_cell(pWS, row, col);
		if (cell && !cell->isHidden) {
			/* Get the value of the cell (either numeric or string) */
			if (cell->id == XLS_RECORD_RK || cell->id == XLS_RECORD_MULRK || cell->id == XLS_RECORD_NUMBER) {
				ret = cell->d;
			}
			else if (cell->id == XLS_RECORD_FORMULA) {
				if (cell->l == 0) { /* It is a number */
					ret = cell->d;
				}
				else {
					if (0 == strcmp((char*)cell->str, "bool")) { /* It is boolean */
						ret = (int)cell->d ? 1. : 0.;
					}
					else if (0 == strcmp((char*)cell->str, "error")) { /* Formula is in error */
						ModelicaFormatError("Error in formula of cell (%u,%u) in sheet \"%s\" of file \"%s\"\n",
							(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
					}
					else { /* Valid formula result */
						char* endptr;
						ret = _strtod_l((char*)cell->str, &endptr, xls->loc);
						if (*endptr != 0) {
							ret = 0.;
							ModelicaFormatError("Error in cell (%u,%u) when reading double value \"%s\" from sheet \"%s\" of file \"%s\"\n",
								(unsigned int)row, (unsigned int)col, (char*)cell->str, _sheetName, xls->fileName);
						}
					}
				}
			}
			else if (cell->str != NULL) {
				char* endptr;
				ret = _strtod_l((char *)cell->str, &endptr, xls->loc);
				if (*endptr != 0) {
					ret = 0.;
					ModelicaFormatError("Error in cell (%u,%u) when reading double value \"%s\" from sheet \"%s\" of file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, (char*)cell->str, _sheetName, xls->fileName);
				}
			}
		}
		else {
			ModelicaFormatError("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
				(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
		}
	}
	return ret;
}

const char* ED_getStringFromXLS(void* _xls, const char* cellAddress, const char* sheetName)
{
	XLSFile* xls = (XLSFile*)_xls;
	if (xls) {
		char* _sheetName = (char*)sheetName;
		xlsWorkSheet* pWS = findSheet(xls, &_sheetName);
		xlsCell* cell;
		WORD row = 0, col = 0;

		rc(cellAddress, &row, &col);
		cell = xls_cell(pWS, row, col);
		if (cell && !cell->isHidden) {
			/* Get the string value of the cell */
			if (cell->id == XLS_RECORD_FORMULA) {
				if (cell->l != 0) { /* It is not a number */
					if ((0 != strcmp((char*)cell->str, "bool")) && /* It is not boolean and */
						(0 != strcmp((char*)cell->str, "error"))) { /* formula is not in error */
						char* ret = ModelicaAllocateString(strlen((char*)cell->str));
						strcpy(ret, (char*)cell->str);
						return (const char*)ret;
					}
				}
			}
			else if (cell->str != NULL) {
				char* ret = ModelicaAllocateString(strlen((char*)cell->str));
				strcpy(ret, (char*)cell->str);
				return (const char*)ret;
			}
		}
		else {
			ModelicaFormatError("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
				(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
		}
	}
	return "";
}

int ED_getIntFromXLS(void* _xls, const char* cellAddress, const char* sheetName)
{
	int ret = 0;
	XLSFile* xls = (XLSFile*)_xls;
	if (xls) {
		char* _sheetName = (char*)sheetName;
		xlsWorkSheet* pWS = findSheet(xls, &_sheetName);
		xlsCell* cell;
		WORD row = 0, col = 0;

		rc(cellAddress, &row, &col);
		cell = xls_cell(pWS, row, col);
		if (cell && !cell->isHidden) {
			/* Get the value of the cell (either numeric or string) */
			if (cell->id == XLS_RECORD_RK || cell->id == XLS_RECORD_MULRK || cell->id == XLS_RECORD_NUMBER) {
				ret = (int)cell->d;
			}
			else if (cell->id == XLS_RECORD_FORMULA) {
				if (cell->l == 0) { /* It is a number */
					ret = (int)cell->d;
				}
				else {
					if (0 == strcmp((char*)cell->str, "bool")) { /* It is boolean */
						ret = (int)cell->d ? 1 : 0;
					}
					else if (0 == strcmp((char*)cell->str, "error")) { /* Formula is in error */
						ModelicaFormatError("Error in formula of cell (%u,%u) in sheet \"%s\" of file \"%s\"\n",
							(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
					}
					else { /* Valid formula result */
						char* endptr;
						ret = (int)_strtol_l((char*)cell->str, &endptr, 10, xls->loc);
						if (*endptr != 0) {
							ret = 0;
							ModelicaFormatError("Error in cell (%u,%u) when reading int value \"%s\" from sheet \"%s\" of file \"%s\"\n",
								(unsigned int)row, (unsigned int)col, (char*)cell->str, _sheetName, xls->fileName);
						}
					}
				}
			}
			else if (cell->str != NULL) {
				char* endptr;
				ret = (int)_strtol_l((char*)cell->str, &endptr, 10, xls->loc);
				if (*endptr != 0) {
					ret = 0;
					ModelicaFormatError("Error in cell (%u,%u) when reading int value \"%s\" from sheet \"%s\" of file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, (char*)cell->str, _sheetName, xls->fileName);
				}
			}
		}
		else {
			ModelicaFormatError("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
				(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
		}
	}
	return ret;
}

#endif
