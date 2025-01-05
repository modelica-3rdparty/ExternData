/* ED_MATFile.c - MAT functions
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
#include <stdio.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include "ED_ptrtrack.h"
#include "../Include/ED_MATFile.h"
#include "ModelicaIO.h"
#include "ModelicaMatIO.h"
#include "ModelicaUtilities.h"

typedef struct {
	char* fileName;
	int verbose;
} MATFile;

void* ED_createMAT(const char* fileName, int verbose)
{
	MATFile* mat = (MATFile*)malloc(sizeof(MATFile));
	if (mat == NULL) {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	mat->fileName = strdup(fileName);
	if (mat->fileName == NULL) {
		free(mat);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	mat->verbose = verbose;
	ED_PTR_ADD(mat);
	return mat;
}

void ED_destroyMAT(void* _mat)
{
	MATFile* mat = (MATFile*)_mat;
	ED_PTR_CHECK(mat);
	if (mat != NULL) {
		if (mat->fileName != NULL) {
			free(mat->fileName);
		}
		free(mat);
		ED_PTR_DEL(mat);
	}
}

void ED_getDoubleArray2DFromMAT(void* _mat, const char* varName, double* a, size_t m, size_t n)
{
	MATFile* mat = (MATFile*)_mat;
	ED_PTR_CHECK(mat);
	if (mat != NULL) {
		ModelicaIO_readRealMatrix(mat->fileName, varName, a, m, n, mat->verbose);
	}
}

void ED_getStringArray1DFromMAT(void* _mat, const char* varName, const char** a, size_t m)
{
	MATFile* mat = (MATFile*)_mat;
	ED_PTR_CHECK(mat);
	if (mat != NULL) {
		MatIO matio = {NULL, NULL, NULL};

		if (mat->verbose == 1) {
			/* Print info message, that matrix / file is loading */
			ModelicaFormatMessage("... loading \"%s\" from \"%s\"\n", varName, mat->fileName);
		}

		readMatIO(mat->fileName, varName, &matio);
		if (NULL != matio.matvar) {
			matvar_t* matvar = matio.matvar;
			size_t nRow, nCol, i;

			/* Check if matvar is of character class */
			if (matvar->class_type != MAT_C_CHAR) {
				Mat_VarFree(matio.matvarRoot);
				(void)Mat_Close(matio.mat);
				ModelicaFormatError("2D array \"%s\" has not the required "
					"character class.\n", varName);
				return;
			}

			nRow = matvar->dims[0];
			nCol = matvar->dims[1];

			/* Check number of rows */
			if (m > nRow) {
				Mat_VarFree(matio.matvarRoot);
				(void)Mat_Close(matio.mat);
				ModelicaFormatError(
					"Cannot read %lu rows of array \"%s(%lu,%lu)\" "
					"from file \"%s\"\n", (unsigned long)m, varName,
					(unsigned long)nRow, (unsigned long)nCol, mat->fileName);
				return;
			}

			(void)Mat_VarReadDataAll(matio.mat, matvar);

			for (i = 0; i < m; i++) {
				char* str = ModelicaAllocateStringWithErrorReturn(nCol);
				if (NULL != str) {
					size_t j;
					/* Array is stored column-wise -> need to transpose */
					for (j = 0; j < nCol; j++) {
						str[j] = ((char*)matvar->data)[i + j*nRow];
					}
					str[nCol] = '\0';
					a[i] = str;
				}
				else {
					Mat_VarFree(matio.matvarRoot);
					(void)Mat_Close(matio.mat);
					ModelicaError("Memory allocation error\n");
					return;
				}
			}

			Mat_VarFree(matio.matvarRoot);
			(void)Mat_Close(matio.mat);
		}
	}
}

void ED_getArray2DDimensionsFromMAT(void* _mat, const char* varName, int* m, int* n)
{
	MATFile* mat = (MATFile*)_mat;
	int _m = 0;
	int _n = 0;
	if (NULL != m)
		*m = 0;
	if (NULL != n)
		*n = 0;
	ED_PTR_CHECK(mat);
	if (NULL != mat) {
		int dim[2];
		ModelicaIO_readMatrixSizes(mat->fileName, varName, dim);
		_m = dim[0];
		_n = dim[1];
	}
	if (NULL != m)
		*m = _m;
	if (NULL != n)
		*n = _n;
}
