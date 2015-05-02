#if !defined(ED_JSONFILE_C)
#define ED_JSONFILE_C

#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "bsjson.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_JSONFile.h"

typedef struct {
	char* fileName;
	JsonNodeRef root;
	_locale_t loc;
} JSONFile;

void* ED_createJSON(const char* fileName) {
	JSONFile* json = NULL;
	JsonParser jsonParser;
	JsonNodeRef root = JsonParser_parseFile(&jsonParser, fileName);
	if (root == NULL) {
		ModelicaFormatError("Cannot parse file \"%s\"\n", fileName);
	}
	json = (JSONFile*)malloc(sizeof(JSONFile));
	if (json) {
		json->fileName = _strdup(fileName);
		if (json->fileName == NULL) {
			free(json);
			json = NULL;
			ModelicaError("Memory allocation error\n");
		}
		json->loc = _create_locale(LC_NUMERIC, "C");
		json->root = root;
	}
	else {
		ModelicaError("Memory allocation error\n");
	}
	return json;
}

void ED_destroyJSON(void* _json)
{
	JSONFile* json = (JSONFile*)_json;
	if (json) {
		if (json->fileName) {
			free(json->fileName);
		}
		JsonNode_deleteTree(json->root);
		_free_locale(json->loc);
		free(json);
	}
}

static char* findValue(JsonNodeRef* root, const char* varName, const char* fileName)
{
	char* token = NULL;
	char* buf = _strdup(varName);
	if (buf) {
		int elementError = 0;
		strcpy(buf, varName);
		token = strtok(buf, ".");
		if (token == NULL) {
			elementError = 1;
		}
		while (token && elementError == 0) {
			int i;
			int foundToken = 0;
			for (i = 0; i < JsonNode_getChildCount(*root); i++) {
				JsonNodeRef child = JsonNode_findChild(*root, token, JSON_OBJ);
				if (child) {
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
			ModelicaFormatError("Error in when reading element %s from file \"%s\"\n",
				varName, fileName);
		}
		else {
			token = JsonNode_getPairValue(*root, token);
			free(buf);
			if (token == NULL) {
				ModelicaFormatError("Error in when reading element %s from file \"%s\"\n",
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
	if (json) {
		JsonNodeRef root = json->root;
		char* token = findValue(&root, varName, json->fileName);
		if (token) {
			char* endptr;
			ret = _strtod_l(token, &endptr, json->loc);
			if (*endptr != 0) {
				ret = 0.;
				ModelicaFormatError("Error when reading double value %s from file \"%s\"\n",
					token, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Error in line %i when reading double value from file \"%s\"\n",
				json->fileName);
		}
	}
	return ret;
}

const char* ED_getStringFromJSON(void* _json, const char* varName)
{
	JSONFile* json = (JSONFile*)_json;
	if (json) {
		JsonNodeRef root = json->root;
		char* token = findValue(&root, varName, json->fileName);
		if (token) {
			char* ret = ModelicaAllocateString(strlen(token));
			strcpy(ret, token);
			return (const char*)ret;
		}
		else {
			ModelicaFormatError("Error when reading value from file \"%s\"\n",
				json->fileName);
		}
	}
	return "";
}

int ED_getIntFromJSON(void* _json, const char* varName)
{
	int ret = 0;
	JSONFile* json = (JSONFile*)_json;
	if (json) {
		JsonNodeRef root = json->root;
		char* token = findValue(&root, varName, json->fileName);
		if (token) {
			char* endptr;
			ret = (int)_strtol_l(token, &endptr, 10, json->loc);
			if (*endptr != 0) {
				ret = 0;
				ModelicaFormatError("Error when reading int value %s from file \"%s\"\n",
					token, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Error when reading int value from file \"%s\"\n",
				json->fileName);
		}
	}
	return ret;
}

#endif
