/* ED_XLSFile.c - Excel XLS functions
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

#include <string.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include <ctype.h>
#include "ED_locale.h"
#include "ED_ptrtrack.h"
#include "ModelicaUtilities.h"
#include "libxls/xls.h"
#include "../Include/ED_XLSFile.h"
#define HASH_NONFATAL_OOM 1
#include "uthash.h"

typedef struct {
	char* sheetName;
	xlsWorkSheet* pWS;
	UT_hash_handle hh; /* Hashable structure */
} SheetShare;

typedef struct {
	char* fileName;
	ED_LOCALE_TYPE loc;
	xlsWorkBook* pWB;
	SheetShare* sheets;
} XLSFile;

void* ED_createXLS(const char* fileName, const char* encoding, int verbose)
{
	XLSFile* xls = (XLSFile*)malloc(sizeof(XLSFile));
	if (xls == NULL) {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	xls->fileName = strdup(fileName);
	if (xls->fileName == NULL) {
		free(xls);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}

	if (verbose == 1) {
		/* Print info message, that file is loading */
		ModelicaFormatMessage("... loading \"%s\"\n", fileName);
	}

	xls->pWB = xls_open(fileName, encoding);
	if (xls->pWB == NULL) {
		free(xls->fileName);
		free(xls);
		ModelicaFormatError("Cannot open file \"%s\"\n", fileName);
		return NULL;
	}
	xls->sheets = NULL;
	xls->loc = ED_INIT_LOCALE;
	ED_PTR_ADD(xls);
	return xls;
}

void ED_destroyXLS(void* _xls)
{
	XLSFile* xls = (XLSFile*)_xls;
	ED_PTR_CHECK(xls);
	if (xls != NULL) {
		SheetShare* iter;
		SheetShare* tmp;
		if (xls->fileName != NULL) {
			free(xls->fileName);
		}
		ED_FREE_LOCALE(xls->loc);
		HASH_ITER(hh, xls->sheets, iter, tmp) {
			free(iter->sheetName);
			xls_close_WS(iter->pWS);
			HASH_DEL(xls->sheets, iter);
			free(iter);
		}
		xls_close(xls->pWB);
		free(xls);
		ED_PTR_DEL(xls);
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

static xlsWorkSheet* findSheet(XLSFile* xls, char** sheetName)
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
	if (iter != NULL) {
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
			ModelicaFormatMessage("Cannot find sheet \"%s\" in file \"%s\"\n",
				*sheetName, xls->fileName);
			return NULL;
		}
		/* Open and parse the sheet */
		pWS = xls_getWorkSheet(xls->pWB, sheet);
		xls_parseWorkSheet(pWS);
		iter = malloc(sizeof(SheetShare));
		if (iter != NULL) {
			iter->sheetName = strdup(*sheetName);
			iter->pWS = pWS;
			HASH_ADD_KEYPTR(hh, xls->sheets, iter->sheetName, strlen(iter->sheetName), iter);
			if (NULL == iter->hh.tbl) {
				free(iter->sheetName);
				free(iter);
			}
		}
	}
	return pWS;
}

double ED_getDoubleFromXLS(void* _xls, const char* cellAddress, const char* sheetName, int* exist)
{
	double ret = 0.;
	XLSFile* xls = (XLSFile*)_xls;
	ED_PTR_CHECK(xls);
	if (xls != NULL) {
		char* _sheetName = (char*)sheetName;
		xlsWorkSheet* pWS = findSheet(xls, &_sheetName);
		if (NULL != pWS) {
			xlsCell* cell;
			WORD row = 0, col = 0;

			*exist = 1;
			rc(cellAddress, &row, &col);
			cell = xls_cell(pWS, row, col);
			if (cell != NULL && !cell->isHidden) {
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
							if (ED_strtod((char*)cell->str, xls->loc, &ret, ED_STRICT)) {
								ModelicaFormatError("Error in cell (%u,%u) when reading double value \"%s\" from sheet \"%s\" of file \"%s\"\n",
									(unsigned int)row, (unsigned int)col, cell->str, _sheetName, xls->fileName);
							}
						}
					}
				}
				else if (cell->id == XLS_RECORD_BLANK) {
					ModelicaFormatMessage("Found blank cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
				}
				else if (cell->str != NULL) {
					if (ED_strtod((char*)cell->str, xls->loc, &ret, ED_STRICT)) {
						ModelicaFormatError("Error in cell (%u,%u) when reading double value \"%s\" from sheet \"%s\" of file \"%s\"\n",
							(unsigned int)row, (unsigned int)col, cell->str, _sheetName, xls->fileName);
					}
				}
				else {
					*exist = 0;
				}
			}
			else {
				ModelicaFormatMessage("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
					(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
				*exist = 0;
			}
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

const char* ED_getStringFromXLS(void* _xls, const char* cellAddress, const char* sheetName, int* exist)
{
	XLSFile* xls = (XLSFile*)_xls;
	ED_PTR_CHECK(xls);
	if (xls != NULL) {
		char* _sheetName = (char*)sheetName;
		xlsWorkSheet* pWS = findSheet(xls, &_sheetName);
		if (NULL != pWS) {
			xlsCell* cell;
			WORD row = 0, col = 0;

			*exist = 1;
			rc(cellAddress, &row, &col);
			cell = xls_cell(pWS, row, col);
			if (cell != NULL && !cell->isHidden) {
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
				else if (cell->id == XLS_RECORD_BLANK) {
					ModelicaFormatMessage("Found blank cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
				}
				else if (cell->str != NULL) {
					char* ret = ModelicaAllocateString(strlen((char*)cell->str));
					strcpy(ret, (char*)cell->str);
					return (const char*)ret;
				}
				else {
					*exist = 0;
				}
			}
			else {
				ModelicaFormatMessage("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
					(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
				*exist = 0;
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

int ED_getIntFromXLS(void* _xls, const char* cellAddress, const char* sheetName, int* exist)
{
	long ret = 0;
	XLSFile* xls = (XLSFile*)_xls;
	ED_PTR_CHECK(xls);
	if (xls != NULL) {
		char* _sheetName = (char*)sheetName;
		xlsWorkSheet* pWS = findSheet(xls, &_sheetName);
		if (NULL != pWS) {
			xlsCell* cell;
			WORD row = 0, col = 0;

			*exist = 1;
			rc(cellAddress, &row, &col);
			cell = xls_cell(pWS, row, col);
			if (cell != NULL && !cell->isHidden) {
				/* Get the value of the cell (either numeric or string) */
				if (cell->id == XLS_RECORD_RK || cell->id == XLS_RECORD_MULRK || cell->id == XLS_RECORD_NUMBER) {
					ret = (long)cell->d;
				}
				else if (cell->id == XLS_RECORD_FORMULA) {
					if (cell->l == 0) { /* It is a number */
						ret = (long)cell->d;
					}
					else {
						if (0 == strcmp((char*)cell->str, "bool")) { /* It is boolean */
							ret = (long)cell->d ? 1 : 0;
						}
						else if (0 == strcmp((char*)cell->str, "error")) { /* Formula is in error */
							ModelicaFormatError("Error in formula of cell (%u,%u) in sheet \"%s\" of file \"%s\"\n",
								(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
						}
						else { /* Valid formula result */
							if (ED_strtol((char*)cell->str, xls->loc, &ret, ED_STRICT)) {
								ModelicaFormatError("Error in cell (%u,%u) when reading int value \"%s\" from sheet \"%s\" of file \"%s\"\n",
									(unsigned int)row, (unsigned int)col, (char*)cell->str, _sheetName, xls->fileName);
							}
						}
					}
				}
				else if (cell->id == XLS_RECORD_BLANK) {
					ModelicaFormatMessage("Found blank cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
						(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
				}
				else if (cell->str != NULL) {
					if (ED_strtol((char*)cell->str, xls->loc, &ret, ED_STRICT)) {
						ModelicaFormatError("Error in cell (%u,%u) when reading int value \"%s\" from sheet \"%s\" of file \"%s\"\n",
							(unsigned int)row, (unsigned int)col, (char*)cell->str, _sheetName, xls->fileName);
					}
				}
				else {
					*exist = 0;
				}
			}
			else {
				ModelicaFormatMessage("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
					(unsigned int)row, (unsigned int)col, _sheetName, xls->fileName);
				*exist = 0;
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

void ED_getDoubleArray2DFromXLS(void* _xls, const char* cellAddress, const char* sheetName, double* a, size_t m, size_t n)
{
	XLSFile* xls = (XLSFile*)_xls;
	ED_PTR_CHECK(xls);
	if (xls != NULL) {
		char* _sheetName = (char*)sheetName;
		xlsWorkSheet* pWS = findSheet(xls, &_sheetName);
		if (NULL != pWS) {
			WORD row = 0, col = 0;
			WORD i, j;

			rc(cellAddress, &row, &col);
			for (i = 0; i < m; i++) {
				for (j = 0; j < n; j++) {
					xlsCell* cell = xls_cell(pWS, row + i, col + j);
					if (cell != NULL && !cell->isHidden) {
						/* Get the value of the cell (either numeric or string) */
						if (cell->id == XLS_RECORD_RK || cell->id == XLS_RECORD_MULRK || cell->id == XLS_RECORD_NUMBER) {
							a[i*n + j] = cell->d;
						}
						else if (cell->id == XLS_RECORD_FORMULA) {
							if (cell->l == 0) { /* It is a number */
								a[i*n + j] = cell->d;
							}
							else {
								if (0 == strcmp((char*)cell->str, "bool")) { /* It is boolean */
									a[i*n + j] = (int)cell->d ? 1. : 0.;
								}
								else if (0 == strcmp((char*)cell->str, "error")) { /* Formula is in error */
									ModelicaFormatError("Error in formula of cell (%u,%u) in sheet \"%s\" of file \"%s\"\n",
										(unsigned int)(row + i), (unsigned int)(col + j), _sheetName, xls->fileName);
								}
								else { /* Valid formula result */
									if (ED_strtod((char*)cell->str, xls->loc, &a[i*n + j], ED_STRICT)) {
										ModelicaFormatError("Error in cell (%u,%u) when reading double value \"%s\" from sheet \"%s\" of file \"%s\"\n",
											(unsigned int)(row + i), (unsigned int)(col + j), cell->str, _sheetName, xls->fileName);
									}
								}
							}
						}
						else if (cell->id == XLS_RECORD_BLANK) {
							a[i*n + j] = 0.;
							ModelicaFormatMessage("Found blank cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
								(unsigned int)(row + i), (unsigned int)(col + j), _sheetName, xls->fileName);
						}
						else if (cell->str != NULL) {
							if (ED_strtod((char*)cell->str, xls->loc, &a[i*n + j], ED_STRICT)) {
								ModelicaFormatError("Error in cell (%u,%u) when reading double value \"%s\" from sheet \"%s\" of file \"%s\"\n",
									(unsigned int)(row + i), (unsigned int)(col + j), cell->str, _sheetName, xls->fileName);
							}
						}
						else {
							a[i*n + j] = 0.;
						}
					}
					else {
						a[i*n + j] = 0.;
						ModelicaFormatMessage("Cannot get cell (%u,%u) in sheet \"%s\" from file \"%s\"\n",
							(unsigned int)(row + i), (unsigned int)(col + j), _sheetName, xls->fileName);
					}
				}
			}
		}
	}
}

void ED_getArray2DDimensionsFromXLS(void* _xls, const char* sheetName, int* m, int* n)
{
	XLSFile* xls = (XLSFile*)_xls;
	int _m = 0;
	int _n = 0;
	if (NULL != m)
		*m = 0;
	if (NULL != n)
		*n = 0;
	ED_PTR_CHECK(xls);
	if (xls != NULL) {
		char* _sheetName = (char*)sheetName;
		const xlsWorkSheet* pWS = findSheet(xls, &_sheetName);
		if (NULL != pWS) {
			_m = pWS->rows.lastrow + 1;
			_n = pWS->rows.lastcol;
		}
	}
	if (NULL != m)
		*m = _m;
	if (NULL != n)
		*n = _n;
}
