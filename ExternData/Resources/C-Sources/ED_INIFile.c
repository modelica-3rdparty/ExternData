/* ED_INIFile.c - INI functions
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
#include "ED_locale.h"
#include "ED_ptrtrack.h"
#include "array.h"
#include "ini.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_INIFile.h"

typedef struct {
	char* key;
	char* value;
} INIPair;

typedef struct {
	char* name;
	cpo_array_t* pairs;
} INISection;

typedef struct {
	char* fileName;
	ED_LOCALE_TYPE loc;
	cpo_array_t* sections;
} INIFile;

static int compareSection(const void *a, const void *b)
{
	return strcmp(((INISection*)a)->name, ((INISection*)b)->name);
}

static int compareKey(const void *a, const void *b)
{
	return strcmp(((INIPair*)a)->key, ((INIPair*)b)->key);
}

static INISection* findSection(INIFile* ini, const char* name)
{
	INISection tmpSection = {(char*)name, NULL};
	INISection* ret = (INISection*)cpo_array_bsearch(ini->sections, &tmpSection, compareSection);
	return ret;
}

static INIPair* findKey(INISection* section, const char* key)
{
	INIPair tmpPair = {(char*)key, NULL};
	INIPair* ret = (INIPair*)cpo_array_bsearch(section->pairs, &tmpPair, compareKey);
	return ret;
}

/* Callback function for ini_parse */
static int fillValues(void* userdata, const char* section, const char* key, const char* value)
{
	INIFile* ini = (INIFile*)userdata;
	if (ini != NULL) {
		INIPair* pair;
		INISection* _section = findSection(ini, section);
		if (_section == NULL) {
			_section = (INISection*)cpo_array_push(ini->sections);
			_section->name = (section != NULL) ? strdup(section) : NULL;
			_section->pairs = cpo_array_create(4 , sizeof(INIPair));
		}
		pair = (INIPair*)cpo_array_push(_section->pairs);
		pair->key = (key != NULL) ? strdup(key) : NULL;
		pair->value = (value != NULL) ? strdup(value) : NULL;
		return 1;
	}
	return 0;
}

void* ED_createINI(const char* fileName, int verbose)
{
	int ret;
	INIFile* ini = (INIFile*)malloc(sizeof(INIFile));
	if (ini == NULL) {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	ini->fileName = strdup(fileName);
	if (ini->fileName == NULL) {
		free(ini);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}

	ini->sections = cpo_array_create(1 , sizeof(INISection));

	if (verbose == 1) {
		/* Print info message, that file is loading */
		ModelicaFormatMessage("... loading \"%s\"\n", fileName);
	}

	ret = ini_parse(fileName, fillValues, ini);
	if (0 != ret) {
		cpo_array_destroy(ini->sections);
		free(ini->fileName);
		free(ini);
		ModelicaFormatError("Cannot read \"%s\"\n", fileName);
		return NULL;
	}
	ini->loc = ED_INIT_LOCALE;
	ED_PTR_ADD(ini);
	return ini;
}

void ED_destroyINI(void* _ini)
{
	INIFile* ini = (INIFile*)_ini;
	ED_PTR_CHECK(ini);
	if (ini != NULL) {
		if (ini->fileName != NULL) {
			free(ini->fileName);
		}
		ED_FREE_LOCALE(ini->loc);
		if (ini->sections != NULL) {
			size_t i;
			for (i = 0; i < ini->sections->num; i++) {
				INISection* section = (INISection*)cpo_array_get_at(ini->sections, i);
				free(section->name);
				if (section->pairs != NULL) {
					size_t j;
					for (j = 0; j < section->pairs->num; j++) {
						INIPair* pair = (INIPair*)cpo_array_get_at(section->pairs, j);
						free(pair->key);
						free(pair->value);
					}
					cpo_array_destroy(section->pairs);
				}
			}
			cpo_array_destroy(ini->sections);
		}
		free(ini);
		ED_PTR_DEL(ini);
	}
}

double ED_getDoubleFromINI(void* _ini, const char* varName, const char* section, int strict, int* exist)
{
	double ret = 0.;
	INIFile* ini = (INIFile*)_ini;
	ED_PTR_CHECK(ini);
	if (ini != NULL) {
		INISection* _section = findSection(ini, section);
		if (_section != NULL) {
			INIPair* pair = findKey(_section, varName);
			*exist = 1;
			if (pair != NULL) {
				if (NULL != pair->value) {
					if (ED_strtod(pair->value, ini->loc, &ret, strict)) {
						ModelicaFormatError("Cannot read double value \"%s\" from file \"%s\"\n",
							pair->value, ini->fileName);
					}
				}
				else {
					ModelicaFormatError("Cannot read value for key \"%s\" from file \"%s\"\n",
						varName, ini->fileName);
					*exist = 0;
				}
			}
			else {
				ModelicaFormatMessage("Cannot read key \"%s\" from file \"%s\"\n",
					varName, ini->fileName);
				*exist = 0;
			}
		}
		else {
			if (strlen(section) > 0) {
				ModelicaFormatMessage("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ModelicaFormatMessage("Cannot read empty section from file \"%s\"\n",
					ini->fileName);
			}
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return ret;
}

const char* ED_getStringFromINI(void* _ini, const char* varName, const char* section, int* exist)
{
	INIFile* ini = (INIFile*)_ini;
	ED_PTR_CHECK(ini);
	if (ini != NULL) {
		INISection* _section = findSection(ini, section);
		if (_section != NULL) {
			INIPair* pair = findKey(_section, varName);
			*exist = 1;
			if (pair != NULL) {
				if (NULL != pair->value) {
					char* ret = ModelicaAllocateString(strlen(pair->value));
					strcpy(ret, pair->value);
					return (const char*)ret;
				}
				else {
					ModelicaFormatError("Cannot read value for key \"%s\" from file \"%s\"\n",
						varName, ini->fileName);
					*exist = 0;
				}
			}
			else {
				ModelicaFormatMessage("Cannot read key \"%s\" from file \"%s\"\n",
					varName, ini->fileName);
				*exist = 0;
			}
		}
		else {
			if (strlen(section) > 0) {
				ModelicaFormatMessage("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ModelicaFormatMessage("Cannot read empty section from file \"%s\"\n",
					ini->fileName);
			}
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return "";
}

int ED_getIntFromINI(void* _ini, const char* varName, const char* section, int strict, int* exist)
{
	long ret = 0;
	INIFile* ini = (INIFile*)_ini;
	ED_PTR_CHECK(ini);
	if (ini != NULL) {
		INISection* _section = findSection(ini, section);
		if (_section != NULL) {
			INIPair* pair = findKey(_section, varName);
			*exist = 1;
			if (pair != NULL) {
				if (NULL != pair->value) {
					if (ED_strtol(pair->value, ini->loc, &ret, strict)) {
						ModelicaFormatError("Cannot read int value \"%s\" from file \"%s\"\n",
							pair->value, ini->fileName);
					}
				}
				else {
					ModelicaFormatError("Cannot read value for key \"%s\" from file \"%s\"\n",
						varName, ini->fileName);
					*exist = 0;
				}
			}
			else {
				ModelicaFormatMessage("Cannot read key \"%s\" from file \"%s\"\n",
					varName, ini->fileName);
				*exist = 0;
			}
		}
		else {
			if (strlen(section) > 0) {
				ModelicaFormatMessage("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ModelicaFormatMessage("Cannot read empty section from file \"%s\"\n",
					ini->fileName);
			}
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return (int)ret;
}
