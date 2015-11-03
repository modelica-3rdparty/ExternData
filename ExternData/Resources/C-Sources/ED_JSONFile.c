/* ED_JSONFile.c - JSON functions
 *
 * Copyright (C) 2015, tbeu
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
#include "bsjson.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_JSONFile.h"

typedef struct {
	char* fileName;
	JsonNodeRef root;
	ED_LOCALE_TYPE loc;
} JSONFile;

void* ED_createJSON(const char* fileName)
{
	JsonParser jsonParser;
	JSONFile* json = (JSONFile*)malloc(sizeof(JSONFile));
	if (json == NULL) {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
	json->fileName = strdup(fileName);
	if (json->fileName == NULL) {
		free(json);
		ModelicaError("Memory allocation error\n");
		return NULL;
	}

	json->root = JsonParser_parseFile(&jsonParser, fileName);
	if (json->root == NULL) {
		free(json->fileName);
		free(json);
		if (JsonParser_getErrorLineSet(&jsonParser) != 0) {
			ModelicaFormatError("Error \"%s\" in line %lu: Cannot parse file \"%s\"\n",
				JsonParser_getErrorString(&jsonParser), JsonParser_getErrorLine(&jsonParser), fileName);
		}
		else {
			ModelicaFormatError("Cannot read \"%s\": %s\n", fileName, JsonParser_getErrorString(&jsonParser));
		}
		return NULL;
	}
	json->loc = ED_INIT_LOCALE;
	return json;
}

void ED_destroyJSON(void* _json)
{
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if (json->fileName != NULL) {
			free(json->fileName);
		}
		JsonNode_deleteTree(json->root);
		ED_FREE_LOCALE(json->loc);
		free(json);
	}
}

static char* findValue(JsonNodeRef* root, const char* varName, const char* fileName)
{
	char* token = NULL;
	char* buf = strdup(varName);
	if (buf != NULL) {
		int elementError = 0;
		token = strtok(buf, ".");
		if (token == NULL) {
			elementError = 1;
		}
		while (token != NULL && elementError == 0) {
			int i;
			int foundToken = 0;
			for (i = 0; i < JsonNode_getChildCount(*root); i++) {
				JsonNodeRef child = JsonNode_findChild(*root, token, JSON_OBJ);
				if (child != NULL) {
					*root = child;
					token = strtok(NULL, ".");
					foundToken = 1;
					break;
				}
			}
			if (foundToken == 0) {
				elementError = 1;
			}
		}
		if (token == NULL) {
			free(buf);
			ModelicaFormatError("Cannot read element \"%s\" from file \"%s\"\n",
				varName, fileName);
		}
		else {
			token = JsonNode_getPairValue(*root, token);
			free(buf);
			if (token == NULL) {
				ModelicaFormatError("Cannot read element \"%s\" from file \"%s\"\n",
					varName, fileName);
			}
		}
	}
	else {
		ModelicaError("Memory allocation error\n");
	}
	return token;
}

double ED_getDoubleFromJSON(void* _json, const char* varName)
{
	double ret = 0.;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		JsonNodeRef root = json->root;
		char* token = findValue(&root, varName, json->fileName);
		if (token != NULL) {
			if (ED_strtod(token, json->loc, &ret)) {
				ModelicaFormatError("Cannot read double value \"%s\" from file \"%s\"\n",
					token, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot read double value from file \"%s\"\n",
				json->fileName);
		}
	}
	return ret;
}

const char* ED_getStringFromJSON(void* _json, const char* varName)
{
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		JsonNodeRef root = json->root;
		char* token = findValue(&root, varName, json->fileName);
		if (token != NULL) {
			char* ret = ModelicaAllocateString(strlen(token));
			strcpy(ret, token);
			return (const char*)ret;
		}
		else {
			ModelicaFormatError("Cannot read value from file \"%s\"\n",
				json->fileName);
		}
	}
	return "";
}

int ED_getIntFromJSON(void* _json, const char* varName)
{
	int ret = 0;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		JsonNodeRef root = json->root;
		char* token = findValue(&root, varName, json->fileName);
		if (token != NULL) {
			if (ED_strtoi(token, json->loc, &ret)) {
				ModelicaFormatError("Cannot read int value \"%s\" from file \"%s\"\n",
					token, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot read int value from file \"%s\"\n",
				json->fileName);
		}
	}
	return ret;
}
