#if !defined(ED_XLSFILE_C)
#define ED_XLSFILE_C

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>
#include "ModelicaUtilities.h"
#include "libxls/xls.h"
#include "../Include/ED_XLSFile.h"

typedef struct {
	char* fileName;
	_locale_t loc;
	xlsWorkBook* pWB;
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
		if (xls->fileName) {
			free(xls->fileName);
		}
		_free_locale(xls->loc);
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

double ED_getDoubleFromXLS(void* _xls, const char* cellAddress, const char* sheetName)
{
	double ret = 0.;
	XLSFile* xls = (XLSFile*)_xls;
	if (xls) {
		int sheet = 0;
		xlsWorkSheet* pWS;
		xlsCell* cell;
		WORD row = 0, col = 0;
		if (strlen(sheetName) > 0) {
			DWORD i;
			/* Process all sheets */
			for (i = 0; i < xls->pWB->sheets.count; i++) {
				if (strcmp(sheetName, (char*)xls->pWB->sheets.sheet[i].name) == 0) {
					sheet = (int)i;
					break;
				}
			}
		}
		/* Open and parse the sheet */
		pWS = xls_getWorkSheet(xls->pWB, sheet);
		xls_parseWorkSheet(pWS);
		rc(cellAddress, &row, &col);
		cell = xls_cell(pWS, row, col);
		if (cell && !cell->isHidden) {
			/* Get the value of the cell (either numeric or string) */
			if (cell->id == 0x27e || cell->id == 0x0BD || cell->id == 0x203) {
				ret = cell->d;
			}
			else if (cell->id == 0x06) {
				/* Formula */
				if (cell->l == 0) { /* It is a number */
					ret = cell->d;
				}
				else {
					if (!strcmp((char*)cell->str, "bool")) { /* It is boolean */
						ret = (int)cell->d ? 1.0 : 0.0;
					}
					else if (!strcmp((char*)cell->str, "error")) { /* formula is in error */
						ModelicaFormatError("Error in formula of cell (%u,%u) in sheet \"%s\" of file \"%s\"\n",
							(unsigned int)row, (unsigned int)col, (char*)xls->pWB->sheets.sheet[sheet].name, xls->fileName);
					}
					else { /* ... cell->str is valid as the result of a string formula */
						char* endptr;
						ret = _strtod_l((char *)cell->str, &endptr, xls->loc);
						if (*endptr != 0) {
							ret = 0.;
							ModelicaFormatError("Error in cell (%u,%u) in sheet \"%s\" when reading double value %s from file \"%s\"\n",
								(unsigned int)row, (unsigned int)col, (char*)xls->pWB->sheets.sheet[sheet].name, (char *)cell->str, xls->fileName);
						}
					}
				}
			}
			else if (cell->str != NULL) {
				char* endptr;
				ret = _strtod_l((char *)cell->str, &endptr, xls->loc);
				if (*endptr != 0) {
					ret = 0.;
					ModelicaFormatError("Error in cell (%u,%u) in sheet \"%s\" when reading double value %s from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, (char*)xls->pWB->sheets.sheet[sheet].name, (char *)cell->str, xls->fileName);
				}
			}
		}
		else {
			ModelicaFormatError("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
				(unsigned int)row, (unsigned int)col, (char*)xls->pWB->sheets.sheet[sheet].name, xls->fileName);
		}
		xls_close_WS(pWS);
	}
	return ret;
}

const char* ED_getStringFromXLS(void* _xls, const char* varName, const char* section)
{
	XLSFile* xls = (XLSFile*)_xls;
	if (xls) {
	}
	return "";
}

int ED_getIntFromXLS(void* _xls, const char* varName, const char* section)
{
	int ret = 0;
	XLSFile* xls = (XLSFile*)_xls;
	if (xls) {
	}
	return ret;
}

#endif
