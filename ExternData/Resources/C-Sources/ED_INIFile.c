/* ED_INIFile.c - INI functions
 *
 * Copyright (C) 2015 tbeu
 *
 * This file is part of ExternData.
 * 
 * ExternData is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * ExternData is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with ExternData; if not, see http://www.gnu.org/licenses
 *
 */

#if defined(__gnu_linux__)
#define _GNU_SOURCE 1
#endif

#include <string.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include "ED_locale.h"
#include "array.h"
#define INI_BUFFERSIZE 1024
#include "minIni.h"
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

/* Callback function for ini_browse */
static int fillValues(const char *section, const char *key, const char *value, const void *userdata)
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

void* ED_createINI(const char* fileName)
{
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
	if (1 != ini_browse(fillValues, ini, fileName)) {
		cpo_array_destroy(ini->sections);
		free(ini->fileName);
		free(ini);
		ModelicaFormatError("Cannot read \"%s\"\n", fileName);
		return NULL;
	}
	ini->loc = ED_INIT_LOCALE;
	return ini;
}

void ED_destroyINI(void* _ini)
{
	INIFile* ini = (INIFile*)_ini;
	if (ini != NULL) {
		if (ini->fileName != NULL) {
			free(ini->fileName);
		}
		ED_FREE_LOCALE(ini->loc);
		if (ini->sections != NULL) {
			int i;
			for (i = 0; i < ini->sections->num; i++) {
				INISection* section = (INISection*)cpo_array_get_at(ini->sections, i);
				free(section->name);
				if (section->pairs != NULL) {
					int j;
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
	}
}

double ED_getDoubleFromINI(void* _ini, const char* varName, const char* section)
{
	double ret = 0.;
	INIFile* ini = (INIFile*)_ini;
	if (ini != NULL) {
		INISection* _section = findSection(ini, section);
		if (_section != NULL) {
			INIPair* pair = findKey(_section, varName);
			if (pair != NULL) {
				if (ED_strtod(pair->value, ini->loc, &ret)) {
					ModelicaFormatError("Cannot read double value \"%s\" from file \"%s\"\n",
						pair->value, ini->fileName);
				}
			}
			else {
				ModelicaFormatError("Cannot read key \"%s\" from file \"%s\"\n",
					varName, ini->fileName);
			}
		}
		else {
			if (strlen(section) > 0) {
				ModelicaFormatError("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ModelicaFormatError("Cannot read empty section from file \"%s\"\n",
					ini->fileName);
			}
		}
	}
	return ret;
}

const char* ED_getStringFromINI(void* _ini, const char* varName, const char* section)
{
	INIFile* ini = (INIFile*)_ini;
	if (ini != NULL) {
		INISection* _section = findSection(ini, section);
		if (_section != NULL) {
			INIPair* pair = findKey(_section, varName);
			if (pair != NULL) {
				char* ret = ModelicaAllocateString(strlen(pair->value));
				strcpy(ret, pair->value);
				return (const char*)ret;
			}
			else {
				ModelicaFormatError("Cannot read key \"%s\" from file \"%s\"\n",
					varName, ini->fileName);
			}
		}
		else {
			if (strlen(section) > 0) {
				ModelicaFormatError("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ModelicaFormatError("Cannot read empty section from file \"%s\"\n",
					ini->fileName);
			}
		}
	}
	return "";
}

int ED_getIntFromINI(void* _ini, const char* varName, const char* section)
{
	int ret = 0;
	INIFile* ini = (INIFile*)_ini;
	if (ini != NULL) {
		INISection* _section = findSection(ini, section);
		if (_section != NULL) {
			INIPair* pair = findKey(_section, varName);
			if (pair != NULL) {
				if (ED_strtoi(pair->value, ini->loc, &ret)) {
					ModelicaFormatError("Cannot read int value \"%s\" from file \"%s\"\n",
						pair->value, ini->fileName);
				}
			}
			else {
				ModelicaFormatError("Cannot read key \"%s\" from file \"%s\"\n",
					varName, ini->fileName);
			}
		}
		else {
			if (strlen(section) > 0) {
				ModelicaFormatError("Cannot read section \"%s\" from file \"%s\"\n",
					section, ini->fileName);
			}
			else {
				ModelicaFormatError("Cannot read empty section from file \"%s\"\n",
					ini->fileName);
			}
		}
	}
	return ret;
}
