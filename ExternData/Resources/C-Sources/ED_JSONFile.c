#if !defined(ED_JSONFILE_C)
#define ED_JSONFILE_C

#if !defined(_MSC_VER)
#define _strdup strdup
#endif

#include <string.h>
#include "ED_locale.h"
#include "bsjson.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_JSONFile.h"

typedef struct {
	char* fileName;
	JsonNodeRef root;
	ED_LOCALE_TYPE loc;
} JSONFile;

void* ED_createJSON(const char* fileName) {
	JSONFile* json = NULL;
	JsonParser jsonParser;
	JsonNodeRef root = JsonParser_parseFile(&jsonParser, fileName);
	if (root == NULL) {
		ModelicaFormatError("Cannot parse file \"%s\"\n", fileName);
		return json;
	}
	json = (JSONFile*)malloc(sizeof(JSONFile));
	if (json != NULL) {
		json->fileName = _strdup(fileName);
		if (json->fileName == NULL) {
			JsonNode_deleteTree(root);
			free(json);
			ModelicaError("Memory allocation error\n");
			return NULL;
		}
		json->loc = ED_INIT_LOCALE;
		json->root = root;
	}
	else {
		ModelicaError("Memory allocation error\n");
		return NULL;
	}
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
	char* buf = _strdup(varName);
	if (buf != NULL) {
		int elementError = 0;
		strcpy(buf, varName);
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
			ModelicaFormatError("Error when reading element \"%s\" from file \"%s\"\n",
				varName, fileName);
		}
		else {
			token = JsonNode_getPairValue(*root, token);
			free(buf);
			if (token == NULL) {
				ModelicaFormatError("Error when reading element \"%s\" from file \"%s\"\n",
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
				ModelicaFormatError("Error when reading double value \"%s\" from file \"%s\"\n",
					token, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Error when reading double value from file \"%s\"\n",
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
	if (json != NULL) {
		JsonNodeRef root = json->root;
		char* token = findValue(&root, varName, json->fileName);
		if (token != NULL) {
			if (ED_strtoi(token, json->loc, &ret)) {
				ModelicaFormatError("Error when reading int value \"%s\" from file \"%s\"\n",
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
