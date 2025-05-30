/* ED_INIFile.c - INI functions
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
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include "ED_locale.h"
#include "ED_logging.h"
#include "ED_ptrtrack.h"
#include "array.h"
#include "ini.h"
#include "ModelicaUtilities.h"
#include "ED_INIFile.h"

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
	ED_LOGGING_FUNC log;
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

void* ED_createINI(const char* fileName, int verbose, int detectMissingData)
{
	int line;
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

	line = ini_parse(fileName, fillValues, ini);
	if (0 != line) {
		cpo_array_destroy(ini->sections);
		free(ini->fileName);
		free(ini);
		ModelicaFormatError("Error in line %i: Cannot read from file \"%s\"\n", line, fileName);
		return NULL;
	}
	ini->loc = ED_INIT_LOCALE;
	switch (detectMissingData) {
		case ED_LOG_NONE:
			ini->log = ED_LogNone;
			break;
		case ED_LOG_DEBUG:
			ini->log = ED_LogDebug;
			break;
		case ED_LOG_ERROR:
			ini->log = ED_LogError;
			break;
		default:
			ini->log = ED_LogWarning;
			break;
	}
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
						if (section[0] != '\0') {
							ModelicaFormatError("Cannot read double value \"%s\" of section \"%s\" "
								"from file \"%s\"\n", pair->value, section, ini->fileName);
						}
						else {
							ModelicaFormatError("Cannot read double value \"%s\" of default section "
								"from file \"%s\"\n", pair->value, ini->fileName);
						}
					}
				}
				else {
					*exist = 0;
					if (section[0] != '\0') {
						ini->log("Cannot read value for key \"%s\" of section \"%s\" from file \"%s\"\n",
							varName, section, ini->fileName);
					}
					else {
						ini->log("Cannot read value for key \"%s\" of default section from file \"%s\"\n",
							varName, ini->fileName);
					}
				}
			}
			else {
				*exist = 0;
				if (section[0] != '\0') {
					ini->log("Cannot read key \"%s\" of section \"%s\" from file \"%s\"\n",
						varName, section, ini->fileName);
				}
				else {
					ini->log("Cannot read key \"%s\" of default section from file \"%s\"\n",
						varName, ini->fileName);
				}
			}
		}
		else {
			*exist = 0;
			if (section[0] != '\0') {
				ini->log("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ini->log("Cannot read default section from file \"%s\"\n",
					ini->fileName);
			}
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
					*exist = 0;
					if (section[0] != '\0') {
						ini->log("Cannot read value for key \"%s\" of section \"%s\" from file \"%s\"\n",
							varName, section, ini->fileName);
					}
					else {
						ini->log("Cannot read value for key \"%s\" of default section from file \"%s\"\n",
							varName, ini->fileName);
					}
				}
			}
			else {
				*exist = 0;
				if (section[0] != '\0') {
					ini->log("Cannot read key \"%s\" of section \"%s\" from file \"%s\"\n",
						varName, section, ini->fileName);
				}
				else {
					ini->log("Cannot read key \"%s\" of default section from file \"%s\"\n",
						varName, ini->fileName);
				}
			}
		}
		else {
			*exist = 0;
			if (section[0] != '\0') {
				ini->log("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ini->log("Cannot read default section from file \"%s\"\n",
					ini->fileName);
			}
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
						if (section[0] != '\0') {
							ModelicaFormatError("Cannot read int value \"%s\" of section \"%s\" "
								"from file \"%s\"\n", pair->value, section, ini->fileName);
						}
						else {
							ModelicaFormatError("Cannot read int value \"%s\" of default section "
								"from file \"%s\"\n", pair->value, ini->fileName);
						}
					}
				}
				else {
					*exist = 0;
					if (section[0] != '\0') {
						ini->log("Cannot read value for key \"%s\" of section \"%s\" from file \"%s\"\n",
							varName, section, ini->fileName);
					}
					else {
						ini->log("Cannot read value for key \"%s\" of default section from file \"%s\"\n",
							varName, ini->fileName);
					}
				}
			}
			else {
				*exist = 0;
				if (section[0] != '\0') {
					ini->log("Cannot read key \"%s\" of section \"%s\" from file \"%s\"\n",
						varName, section, ini->fileName);
				}
				else {
					ini->log("Cannot read key \"%s\" of default section from file \"%s\"\n",
						varName, ini->fileName);
				}
			}
		}
		else {
			*exist = 0;
			if (section[0] != '\0') {
				ini->log("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ini->log("Cannot read default section from file \"%s\"\n",
					ini->fileName);
			}
		}
	}
	else {
		*exist = 0;
	}
	return (int)ret;
}
