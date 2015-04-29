#if !defined(ED_INIFILE_C)
#define ED_INIFILE_C

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "minIni.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_INIFile.h"

typedef struct {
	char* fileName;
	const char* key;
	const char* section;
	_locale_t loc;
	char* token;
} INIFile;

void* ED_createINI(const char* fileName) {
	INIFile* ini = (INIFile*)malloc(sizeof(INIFile));
	if (ini) {
		ini->fileName = _strdup(fileName);
		if (ini->fileName == NULL) {
			free(ini);
			ini = NULL;
			ModelicaError("Memory allocation error\n");
		}
		ini->loc = _create_locale(LC_NUMERIC, "C");
	}
	else {
		ModelicaError("Memory allocation error\n");
	}
	return ini;
}

void ED_destroyINI(void* _ini)
{
	INIFile* ini = (INIFile*)_ini;
	if (ini) {
		if (ini->fileName) {
			free(ini->fileName);
		}
		_free_locale(ini->loc);
		free(ini);
	}
}

/* Callback function for ini_browse */
int findValue(const char *section, const char *key, const char *value, const void *userdata)
{
	INIFile* ini = (INIFile*)userdata;
	if (ini) {
		if (0 == strcmp(section, ini->section) && 0 == strcmp(key, ini->key)) {
			ini->token = _strdup(value);
			return 0;
		}
		return 1;
	}
	return 0;
}

double ED_getDoubleFromINI(void* _ini, const char* varName, const char* section)
{
	double ret = 0.;
	INIFile* ini = (INIFile*)_ini;
	if (ini) {
		ini->key = varName;
		ini->section = section;
		ini->token = NULL;
		if (1 == ini_browse(findValue, _ini, ini->fileName)) {
			if (ini->token) {
				char* endptr;
				ret = _strtod_l(ini->token, &endptr, ini->loc);
				if (*endptr != 0) {
					ret = 0.;
					ModelicaFormatError("Error when reading double value %s from file \"%s\"\n",
						ini->token, ini->fileName);
				}
				free(ini->token);
				ini->token = NULL;
			}
			else {
				ModelicaFormatError("Error when reading key from file \"%s\"\n",
					ini->fileName);
			}
		}
		else {
			ModelicaFormatError("Error: Cannot read \"%s\"\n", ini->fileName);
		}
	}
	return ret;
}

const char* ED_getStringFromINI(void* _ini, const char* varName, const char* section)
{
	INIFile* ini = (INIFile*)_ini;
	if (ini) {
		ini->key = varName;
		ini->section = section;
		ini->token = NULL;
		if (1 == ini_browse(findValue, _ini, ini->fileName)) {
			if (ini->token) {
				char* ret = ModelicaAllocateString(strlen(ini->token));
				strcpy(ret, ini->token);
				free(ini->token);
				ini->token = NULL;
				return (const char*)ret;
			}
			else {
				ModelicaFormatError("Error when reading key from file \"%s\"\n",
					ini->fileName);
			}
		}
		else {
			ModelicaFormatError("Error: Cannot read \"%s\"\n", ini->fileName);
		}
	}
	return "";
}

int ED_getIntFromINI(void* _ini, const char* varName, const char* section)
{
	int ret = 0;
	INIFile* ini = (INIFile*)_ini;
	if (ini) {
		ini->key = varName;
		ini->section = section;
		ini->token = NULL;
		if (1 == ini_browse(findValue, _ini, ini->fileName)) {
			if (ini->token) {
				char* endptr;
				ret = (int)_strtol_l(ini->token, &endptr, 10, ini->loc);
				if (*endptr != 0) {
					ret = 0;
					ModelicaFormatError("Error when reading int value %s from file \"%s\"\n",
						ini->token, ini->fileName);
				}
				free(ini->token);
				ini->token = NULL;
			}
			else {
				ModelicaFormatError("Error when reading key from file \"%s\"\n",
					ini->fileName);
			}
		}
		else {
			ModelicaFormatError("Error: Cannot read \"%s\"\n", ini->fileName);
		}
	}
	return ret;
}

#endif
