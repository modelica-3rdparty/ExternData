/* ED_CSVFile.c - CSV functions
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
#include <stdio.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include "ED_locale.h"
#include "ED_ptrtrack.h"
#include "array.h"
#include "utstring.h"
#include "zstring_strtok_dquotes.h"
#include "zstring_rtrim.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_CSVFile.h"

#if !defined(LINE_BUFFER_LENGTH)
#define LINE_BUFFER_LENGTH (64)
#endif

typedef UT_string Line;

typedef struct {
	char* fileName;
	char* sep;
	char quote;
	ED_LOCALE_TYPE loc;
	cpo_array_t* lines;
} CSVFile;

static int readLine(char** buf, int* bufLen, FILE* fp) {
	char* offset;
	int oldBufLen;

	if (fgets(*buf, *bufLen, fp) == NULL) {
		return EOF;
	}

	do {
		char* p;
		char* tmp;

		if ((p = strchr(*buf, '\n')) != NULL) {
			*p = '\0';
			return 0;
		}

		oldBufLen = *bufLen;
		*bufLen *= 2;
		tmp = (char*)realloc(*buf, (size_t)*bufLen);
		if (tmp == NULL) {
			fclose(fp);
			free(*buf);
			ModelicaError("Memory allocation error\n");
			return 1;
		}
		*buf = tmp;
		offset = &((*buf)[oldBufLen - 1]);

	} while (fgets(offset, oldBufLen + 1, fp));

	return 0;
}

void* ED_createCSV(const char* fileName, const char* sep, const char* quote, int verbose)
{
	char* buf;
	int bufLen = LINE_BUFFER_LENGTH;
	int readError;
	FILE* fp;
	CSVFile* csv;

	if (strlen(sep) != 1) {
		ModelicaError("Invalid column delimiter, must be a single character.\n");
		return NULL;
	}
	if (strlen(quote) != 1) {
		ModelicaError("Invalid quotation, must be a single character.\n");
		return NULL;
	}
	csv = (CSVFile*)malloc(sizeof(CSVFile));
	if (csv == NULL) {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	csv->fileName = strdup(fileName);
	if (csv->fileName == NULL) {
		free(csv);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	csv->sep = strdup(sep);
	if (csv->sep == NULL) {
		free(csv->fileName);
		free(csv);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	csv->quote = quote[0];
	csv->lines = cpo_array_create(1 , sizeof(Line));
	if (csv->lines == NULL) {
		free(csv->sep);
		free(csv->fileName);
		free(csv);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}

	if (verbose == 1) {
		/* Print info message, that file is loading */
		ModelicaFormatMessage("... loading \"%s\"\n", fileName);
	}

	fp = fopen(fileName, "r");
	if (fp == NULL) {
		cpo_array_destroy(csv->lines);
		free(csv->sep);
		free(csv->fileName);
		free(csv);
		ModelicaFormatError("Not possible to open file \"%s\": "
			"No such file or directory\n", fileName);
		return NULL;
	}

	buf = (char*)malloc(LINE_BUFFER_LENGTH*sizeof(char));
	if (buf == NULL) {
		fclose(fp);
		cpo_array_destroy(csv->lines);
		free(csv->sep);
		free(csv->fileName);
		free(csv);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}

	/* Loop over lines of file */
	while ((readError = readLine(&buf, &bufLen, fp)) == 0) {
		Line* line = (Line*)cpo_array_push(csv->lines);
		utstring_init(line);
		utstring_bincpy(line, zstring_rtrim(buf), strlen(buf));
	}

	if (1 != readError) {
		free(buf);
		fclose(fp);
	}

	csv->loc = ED_INIT_LOCALE;
	ED_PTR_ADD(csv);
	return csv;
}

void ED_destroyCSV(void* _csv)
{
	CSVFile* csv = (CSVFile*)_csv;
	ED_PTR_CHECK(csv);
	if (csv != NULL) {
		if (csv->fileName != NULL) {
			free(csv->fileName);
		}
		if (csv->sep != NULL) {
			free(csv->sep);
		}
		ED_FREE_LOCALE(csv->loc);
		if (csv->lines != NULL) {
			size_t i;
			for (i = 0; i < csv->lines->num; i++) {
				Line* line = (Line*)cpo_array_get_at(csv->lines, i);
				utstring_done(line);
			}
			cpo_array_destroy(csv->lines);
		}
		free(csv);
		ED_PTR_DEL(csv);
	}
}

void ED_getDoubleArray2DFromCSV(void* _csv, int* field, double* a, size_t m, size_t n)
{
	CSVFile* csv = (CSVFile*)_csv;
	ED_PTR_CHECK(csv);
	if (field[0] < 1) {
		ModelicaError("Invalid line mumber, must be greater than or equal to one.\n");
	}
	if (field[1] < 1) {
		ModelicaError("Invalid column mumber, must be greater than or equal to one.\n");
	}
	if (csv != NULL) {
		size_t i;
		for (i = 0; i < m; i++) {
			size_t j = field[0] + i - 1;
			char* lineCopy;
			if (j >= csv->lines->num) {
				ModelicaFormatError("Error in line %i: Cannot read line from file \"%s\"\n",
					field[0] + (int)i, csv->fileName);
				return;
			}
			lineCopy = strdup(utstring_body((Line*)cpo_array_get_at(csv->lines, j)));
			if (NULL != lineCopy) {
				char* nextToken = NULL;
				int k;
				char* token = zstring_strtok_dquotes(lineCopy, csv->sep, csv->quote, &nextToken);
				for (k = 0; k < field[1] - 1; k++) {
					// Ignore leading tokens
					token = zstring_strtok_dquotes(NULL, csv->sep, csv->quote, &nextToken);
				}
				for (j = 0; j < n; j++) {
					if (token != NULL) {
						size_t len;
						if (token[0] == csv->sep[0]) {
							a[i*n + j] = 0.;
							continue;
						}
						len = strlen(token);
						if (token[0] == csv->quote && token[len - 1] == csv->quote) {
							token[0] = ' ';
							token[len - 1] = '\0';
						}
						if (ED_strtod(token, csv->loc, &a[i*n + j], ED_STRICT)) {
							free(lineCopy);
							ModelicaFormatError("Error in line %i: Cannot read double value at column %i from file \"%s\"\n",
								field[0] + (int)i, field[1] + (int)j, csv->fileName);
							return;
						}
						token = zstring_strtok_dquotes(NULL, csv->sep, csv->quote, &nextToken);
					}
					else {
						free(lineCopy);
						ModelicaFormatError("Error in line %i: Cannot read double value at column %i from file \"%s\"\n",
							field[0] + (int)i, field[1] + (int)j, csv->fileName);
						return;
					}
				}
				free(lineCopy);
			}
			else {
				ModelicaError("Memory allocation error\n");
			}
		}
	}
}

void ED_getArray2DDimensionsFromCSV(void* _csv, int* m, int* n)
{
	CSVFile* csv = (CSVFile*)_csv;
	int _m = 0;
	int _n = 0;
	if (NULL != m)
		*m = 0;
	if (NULL != n)
		*n = 0;
	ED_PTR_CHECK(csv);
	if (NULL != csv) {
		char *lineCopy = strdup(utstring_body((Line*)cpo_array_get_at(csv->lines, 0)));
		if (NULL != lineCopy) {
			char* nextToken = NULL;
			char* token = zstring_strtok_dquotes(lineCopy, csv->sep, csv->quote, &nextToken);
			_m = (int)csv->lines->num;
			while (NULL != token) {
				_n++;
				token = zstring_strtok_dquotes(NULL, csv->sep, csv->quote, &nextToken);
			}
			free(lineCopy);
		}
	}
	if (NULL != m)
		*m = _m;
	if (NULL != n)
		*n = _n;
}
