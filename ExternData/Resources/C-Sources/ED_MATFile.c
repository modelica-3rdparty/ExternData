/* ED_MATFile.c - MAT functions
 *
 * Copyright (C) 2015-2017, tbeu
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

#include <string.h>
#include <stdio.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include "ModelicaUtilities.h"
#include "ModelicaMatIO.h"
#include "../Include/ED_MATFile.h"

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

#if !defined(MATLAB_NAME_LENGTH_MAX)
#define MATLAB_NAME_LENGTH_MAX (64)
#endif

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

	return mat;
}

void ED_destroyMAT(void* _mat)
{
	MATFile* mat = (MATFile*)_mat;
	if (mat != NULL) {
		if (mat->fileName != NULL) {
			free(mat->fileName);
		}
		free(mat);
	}
}

static void transpose(double* table, size_t nRow, size_t nCol)
{
	/* Reference:

	   Cycle-based in-place array transposition
	   (http://en.wikipedia.org/wiki/In-place_matrix_transposition#Non-square_matrices:_Following_the_cycles)
	 */

	size_t i;
	for (i = 1; i < nRow*nCol - 1; i++) {
		size_t x = nRow*(i % nCol) + i/nCol; /* predecessor of i in the cycle */
		/* Continue if cycle is of length one or predecessor already was visited */
		if (x <= i) {
			continue;
		}
		/* Continue if cycle already was visited */
		while (x > i) {
			x = nRow*(x % nCol) + x/nCol;
		}
		if (x < i) {
			continue;
		}
		{
			double tmp = table[i];
			size_t s = i; /* start index in the cycle */
			x = nRow*(i % nCol) + i/nCol; /* predecessor of i in the cycle */
			while (x != i) {
				table[s] = table[x];
				s = x;
				x = nRow*(x % nCol) + x/nCol;
			}
			table[s] = tmp;
		}
	}
}

void ED_getDoubleArray2DFromMAT(void* _mat, const char* varName, double* a, size_t m, size_t n)
{
	MATFile* mat = (MATFile*)_mat;
	if (mat != NULL) {
		mat_t* matfp;
		matvar_t* matvar;
		matvar_t* matvarRoot;
		size_t nRow, nCol;
		int readError = 0;
		char* varNameCopy;
		char* token;
		char* nextToken = NULL;

		varNameCopy = strdup(varName);
		if (varNameCopy == NULL) {
			ModelicaError("Memory allocation error\n");
			return;
		}

		if (mat->verbose == 1) {
			/* Print info message, that file is loading */
			ModelicaFormatMessage("... loading \"%s\" from \"%s\"\n", varName, mat->fileName);
		}

		matfp = Mat_Open(mat->fileName, (int)MAT_ACC_RDONLY);
		if (matfp == NULL) {
			free(varNameCopy);
			ModelicaFormatError("Not possible to open file \"%s\": "
				"No such file or directory\n", mat->fileName);
			return;
		}

		token = strtok_r(varNameCopy, ".", &nextToken);
		matvarRoot = Mat_VarReadInfo(matfp, token == NULL ? varName : token);
		if (matvarRoot == NULL) {
			(void)Mat_Close(matfp);
			if (token == NULL) {
				free(varNameCopy);
				ModelicaFormatError(
					"Variable \"%s\" not found on file \"%s\".\n",
					varName, mat->fileName);
			}
			else {
				char varNameBuf[MATLAB_NAME_LENGTH_MAX];
				if (strlen(token) > MATLAB_NAME_LENGTH_MAX - 1) {
					strncpy(varNameBuf, token, MATLAB_NAME_LENGTH_MAX - 1);
					varNameBuf[MATLAB_NAME_LENGTH_MAX - 1] = '\0';
					free(varNameCopy);
					ModelicaFormatError(
						"Variable \"%s...\" not found on file \"%s\".\n",
						varNameBuf, mat->fileName);
				}
				else {
					strcpy(varNameBuf, token);
					free(varNameCopy);
					ModelicaFormatError(
						"Variable \"%s\" not found on file \"%s\".\n",
						varNameBuf, mat->fileName);
				}
			}
			return;
		}

		matvar = matvarRoot;
		token = strtok_r(NULL, ".", &nextToken);
		/* Get field while matvar is of struct class and of 1x1 size */
		while (token != NULL && matvar != NULL) {
			if (matvar->class_type == MAT_C_STRUCT && matvar->rank == 2 &&
				matvar->dims[0] == 1 && matvar->dims[1] == 1) {
				matvar = Mat_VarGetStructField(matvar, (void*)token, MAT_BY_NAME, 0);
				token = strtok_r(NULL, ".", &nextToken);
			}
			else {
				matvar = NULL;
				break;
			}
		}
		free(varNameCopy);

		if (matvar == NULL) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError(
				"Variable \"%s\" not found on file \"%s\".\n", varName,
				mat->fileName);
			return;
		}

		/* Check if matvar is a matrix */
		if (matvar->rank != 2) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError(
				"Array \"%s\" has not the required rank 2.\n", varName);
			return;
		}

		/* Check if matvar is of double precision class (and thus non-sparse) */
		if (matvar->class_type != MAT_C_DOUBLE) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError("2D array \"%s\" has not the required "
				"double precision class.\n", varName);
			return;
		}

		/* Check if matvar is purely real-valued */
		if (matvar->isComplex) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError("2D array \"%s\" must not be complex.\n",
				varName);
			return;
		}

		nRow = matvar->dims[0];
		nCol = matvar->dims[1];

		/* Check if number of rows matches */
		if (m != nRow) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError(
				"Cannot read %lu rows of array \"%s(%lu,%lu)\" "
				"from file \"%s\"\n", (unsigned long)m, varName,
				(unsigned long)nRow, (unsigned long)nCol, mat->fileName);
			return;
		}

		/* Check if number of columns matches */
		if (n != nCol) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError(
				"Cannot read %lu columns of array \"%s(%lu,%lu)\" "
				"from file \"%s\"\n", (unsigned long)n, varName,
				(unsigned long)nRow, (unsigned long)nCol, mat->fileName);
			return;
		}

		{
			int start[2] = {0, 0};
			int stride[2] = {1, 1};
			int edge[2];
			edge[0] = (int)nRow;
			edge[1] = (int)nCol;
			readError = Mat_VarReadData(matfp, matvar, a, start, stride, edge);
		}

		Mat_VarFree(matvarRoot);
		(void)Mat_Close(matfp);

		if (readError == 0) {
			/* Array is stored column-wise -> need to transpose */
			transpose(a, nRow, nCol);
		}
		else {
			ModelicaFormatError(
				"Error when reading numeric data of matrix \"%s(%lu,%lu)\" "
				"from file \"%s\"\n", varName, (unsigned long)nRow,
				(unsigned long)nCol, mat->fileName);
			return;
		}
	}
}

void ED_getStringArray1DFromMAT(void* _mat, const char* varName, const char* string[], size_t m)
{
	MATFile* mat = (MATFile*)_mat;
	if (mat != NULL) {
		mat_t* matfp;
		matvar_t* matvar;
		matvar_t* matvarRoot;
		size_t nRow, nCol, i;
		char* varNameCopy;
		char* token;
		char* nextToken = NULL;

		varNameCopy = strdup(varName);
		if (varNameCopy == NULL) {
			ModelicaError("Memory allocation error\n");
			return;
		}

		if (mat->verbose == 1) {
			/* Print info message, that file is loading */
			ModelicaFormatMessage("... loading \"%s\" from \"%s\"\n", varName, mat->fileName);
		}

		matfp = Mat_Open(mat->fileName, (int)MAT_ACC_RDONLY);
		if (matfp == NULL) {
			free(varNameCopy);
			ModelicaFormatError("Not possible to open file \"%s\": "
				"No such file or directory\n", mat->fileName);
			return;
		}

		token = strtok_r(varNameCopy, ".", &nextToken);
		matvarRoot = Mat_VarReadInfo(matfp, token == NULL ? varName : token);
		if (matvarRoot == NULL) {
			(void)Mat_Close(matfp);
			if (token == NULL) {
				free(varNameCopy);
				ModelicaFormatError(
					"Variable \"%s\" not found on file \"%s\".\n",
					varName, mat->fileName);
			}
			else {
				char varNameBuf[MATLAB_NAME_LENGTH_MAX];
				if (strlen(token) > MATLAB_NAME_LENGTH_MAX - 1) {
					strncpy(varNameBuf, token, MATLAB_NAME_LENGTH_MAX - 1);
					varNameBuf[MATLAB_NAME_LENGTH_MAX - 1] = '\0';
					free(varNameCopy);
					ModelicaFormatError(
						"Variable \"%s...\" not found on file \"%s\".\n",
						varNameBuf, mat->fileName);
				}
				else {
					strcpy(varNameBuf, token);
					free(varNameCopy);
					ModelicaFormatError(
						"Variable \"%s\" not found on file \"%s\".\n",
						varNameBuf, mat->fileName);
				}
			}
			return;
		}

		matvar = matvarRoot;
		token = strtok_r(NULL, ".", &nextToken);
		/* Get field while matvar is of struct class and of 1x1 size */
		while (token != NULL && matvar != NULL) {
			if (matvar->class_type == MAT_C_STRUCT && matvar->rank == 2 &&
				matvar->dims[0] == 1 && matvar->dims[1] == 1) {
				matvar = Mat_VarGetStructField(matvar, (void*)token, MAT_BY_NAME, 0);
				token = strtok_r(NULL, ".", &nextToken);
			}
			else {
				matvar = NULL;
				break;
			}
		}
		free(varNameCopy);

		if (matvar == NULL) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError(
				"Variable \"%s\" not found on file \"%s\".\n", varName,
				mat->fileName);
			return;
		}

		/* Check if matvar is a matrix */
		if (matvar->rank != 2) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError(
				"Array \"%s\" has not the required rank 2.\n", varName);
			return;
		}

		/* Check if matvar is of character class */
		if (matvar->class_type != MAT_C_CHAR) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError("2D array \"%s\" has not the required "
				"character class.\n", varName);
			return;
		}

		nRow = matvar->dims[0];
		nCol = matvar->dims[1];

		/* Check number of rows */
		if (m > nRow) {
			Mat_VarFree(matvarRoot);
			(void)Mat_Close(matfp);
			ModelicaFormatError(
				"Cannot read %lu rows of array \"%s(%lu,%lu)\" "
				"from file \"%s\"\n", (unsigned long)m, varName,
				(unsigned long)nRow, (unsigned long)nCol, mat->fileName);
			return;
		}

		(void)Mat_VarReadDataAll(matfp, matvar);

		for (i = 0; i < m; i++) {
			char* str = ModelicaAllocateStringWithErrorReturn(nCol);
			if (str != NULL) {
				size_t j;
				/* Array is stored column-wise -> need to transpose */
				for (j = 0; j < nCol; j++) {
					str[j] = ((char*)matvar->data)[i + j*nRow];
				}
				str[nCol] = '\0';
				string[i] = str;
			}
			else {
				Mat_VarFree(matvarRoot);
				(void)Mat_Close(matfp);
				ModelicaError("Memory allocation error\n");
				return;
			}
		}

		Mat_VarFree(matvarRoot);
		(void)Mat_Close(matfp);
	}
}
