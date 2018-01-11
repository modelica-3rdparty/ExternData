/* ED_JSONFile.c - JSON functions
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
#include "parson.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_JSONFile.h"

static JSON_Value_Type json_array_get_type(const JSON_Array *array);
static JSON_Value_Type json_array_get_type2D(const JSON_Array *array);
static int json_array_check_dimensions2D(const JSON_Array *array);

typedef struct {
	char* fileName;
	JSON_Value* rootValue;
	JSON_Object* root;
	ED_LOCALE_TYPE loc;
} JSONFile;

void* ED_createJSON(const char* fileName, int verbose)
{
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

	if (verbose == 1) {
		/* Print info message, that file is loading */
		ModelicaFormatMessage("... loading \"%s\"\n", fileName);
	}

	json->rootValue = json_parse_file_with_comments(fileName);
	json->root = json_value_get_object(json->rootValue);
	if (json->root == NULL) {
		free(json->fileName);
		json_value_free(json->rootValue);
		free(json);
		ModelicaFormatError("Failed to parse json file \"%s\"\n", fileName);
		return NULL;
	}

	/* ModelicaFormatMessage("Loaded JSON data:\n%s\n", json_serialize_to_string(rootValue)); */

	json->loc = ED_INIT_LOCALE;
	ED_PTR_ADD(json);
	return json;
}

void ED_destroyJSON(void* _json)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json->fileName != NULL) {
			free(json->fileName);
		}
		json_object_clear(json->root);
		json_value_free(json->rootValue);
		ED_FREE_LOCALE(json->loc);
		free(json);
		ED_PTR_DEL(json);
	}
}

double ED_getDoubleFromJSON(void* _json, const char* varName, int* exist)
{
	double ret = 0.;
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		*exist = 1;
		if (json_object_dothas_value_of_type(json->root, varName, JSONNumber)) {
			return json_object_dotget_number(json->root, varName);
		}
		else if (json_object_dothas_value_of_type(json->root, varName, JSONString)) {
			const char* token = json_object_dotget_string(json->root, varName);
			if (NULL != token) {
				char* tokenCopy = strdup(token);
				if (NULL != tokenCopy) {
					int ok = ED_strtod(tokenCopy, json->loc, &ret, ED_STRICT);
					free(tokenCopy);
					if (ED_OK != ok) {
						ModelicaFormatError("Could not parse double from \"%s\" at \"%s\" in file \"%s\"\n",
							token, varName, json->fileName);
					}
				}
				else {
					ModelicaError("Memory allocation error\n");
				}
			}
			else {
				*exist = 0;
			}
		}
		else {
			ModelicaFormatMessage("Cannot find numeric value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return ret;
}

const char* ED_getStringFromJSON(void* _json, const char* varName, int* exist)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		*exist = 1;
		if (json_object_dothas_value_of_type(json->root, varName, JSONString)) {
			const char* token = json_object_dotget_string(json->root, varName);
			if (NULL != token) {
				char* ret = ModelicaAllocateString(strlen(token));
				strcpy(ret, token);
				return (const char*)ret;
			}
			else {
				*exist = 0;
			}
		}
		else {
			ModelicaFormatMessage("Cannot find string value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return "";
}

int ED_getIntFromJSON(void* _json, const char* varName, int* exist)
{
	long ret = 0;
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		*exist = 1;
		if (json_object_dothas_value_of_type(json->root, varName, JSONNumber)) {
			return (int)json_object_dotget_number(json->root, varName);
		}
		else if (json_object_dothas_value_of_type(json->root, varName, JSONString)) {
			const char* token = json_object_dotget_string(json->root, varName);
			if (NULL != token) {
				char* tokenCopy = strdup(token);
				if (NULL != tokenCopy) {
					int ok = ED_strtol(tokenCopy, json->loc, &ret, ED_STRICT);
					free(tokenCopy);
					if (ED_OK != ok) {
						ModelicaFormatError("Could not parse integer from \"%s\" at \"%s\" in file \"%s\"\n",
							token, varName, json->fileName);
					}
				}
				else {
					ModelicaError("Memory allocation error\n");
				}
			}
			else {
				*exist = 0;
			}
		}
		else {
			ModelicaFormatMessage("Cannot find numeric value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return (int)ret;
}

int ED_getBooleanFromJSON(void* _json, const char* varName, int* exist)
{
	int ret = 0;
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		*exist = 1;
		if (json_object_dothas_value_of_type(json->root, varName, JSONBoolean)) {
			return json_object_dotget_boolean(json->root, varName);
		}
		else if (json_object_dothas_value_of_type(json->root, varName, JSONString)) {
			const char* token = json_object_dotget_string(json->root, varName);
			if (NULL != token) {
				if (strcmp(token, "true") == 0) {
					ret = 1;
				}
				else if (strcmp(token, "false") == 0) {
					ret = 0;
				}
				else {
					ModelicaFormatError("Could not parse boolean from \"%s\" at \"%s\" in file \"%s\"\n",
						token, varName, json->fileName);
				}
			}
			else {
				*exist = 0;
			}
		}
		else {
			ModelicaFormatMessage("Cannot find boolean value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
			*exist = 0;
		}
	}
	else {
		*exist = 0;
	}
	return ret;
}

void ED_getArray1DDimensionFromJSON(void* _json, const char* varName, int* n)
{
	JSONFile* json = (JSONFile*)_json;
	*n = 0;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			*n = (int)json_array_get_count(jsonArray);
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

void ED_getArray2DDimensionsFromJSON(void* _json, const char* varName, int* m, int* n)
{
	JSONFile* json = (JSONFile*)_json;
	int _m = 0;
	int _n = 0;
	if (NULL != m)
		*m = 0;
	if (NULL != n)
		*n = 0;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (JSONArray == json_array_get_type(jsonArray)) {
				_m = (int)json_array_get_count(jsonArray);
				_n = json_array_check_dimensions2D(jsonArray);
				if (_n == -1) {
					ModelicaFormatError("Array value has 2 dimensions, but not all rows have same column dimension \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value has 1 dimension, not 2 \"%s\" in file \"%s\"\n",
					varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
	if (NULL != m)
		*m = _m;
	if (NULL != n)
		*n = _n;
}

void ED_getDoubleArray1DFromJSON(void* _json, const char* varName, double* a, size_t n)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (n<=json_array_get_count(jsonArray)) {
				size_t i;
				if (JSONNumber == json_array_get_type(jsonArray)) {
					for (i=0; i<n; i++) {
						a[i] = json_array_get_number(jsonArray, i);
					}
				}
				else if (JSONString == json_array_get_type(jsonArray)) {
					for (i=0; i<n; i++) {
						const char* token = json_array_get_string(jsonArray, i);
						if (NULL != token) {
							char* tokenCopy = strdup(token);
							if (NULL != tokenCopy) {
								int ok = ED_strtod(tokenCopy, json->loc, &a[i], ED_STRICT);
								free(tokenCopy);
								if (ED_OK != ok) {
									ModelicaFormatError("Could not parse double from \"%s\" in array at \"%s\" in file \"%s\"\n",
										token, varName, json->fileName);
								}
							}
							else {
								ModelicaError("Memory allocation error\n");
							}
						}
					}
				}
				else {
					ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value dimension (%d) does not match requested size (%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), n, varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

void ED_getStringArray1DFromJSON(void* _json, const char* varName, char** a, size_t n)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (n<=json_array_get_count(jsonArray)) {
				if (JSONString == json_array_get_type(jsonArray)) {
					size_t i;
					for (i=0; i<n; i++) {
						const char* token = json_array_get_string(jsonArray, i);
						if (NULL != token) {
							a[i] = ModelicaAllocateString(strlen(token));
							strcpy(a[i], token);
						}
					}
				}
				else {
					ModelicaFormatError("Array value is not string \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value dimension does not match requested size \"%s\" in file \"%s\"\n",
					varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

void ED_getIntArray1DFromJSON(void* _json, const char* varName, int* a, size_t n)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (n<=json_array_get_count(jsonArray)) {
				size_t i;
				if (JSONNumber == json_array_get_type(jsonArray)) {
					for (i=0; i<n; i++) {
						a[i] = (int)json_array_get_number(jsonArray, i);
					}
				}
				else if (JSONString == json_array_get_type(jsonArray)) {
					for (i=0; i<n; i++) {
						const char* token = json_array_get_string(jsonArray, i);
						if (NULL != token) {
							char* tokenCopy = strdup(token);
							if (NULL != tokenCopy) {
								long val;
								int ok = ED_strtol(tokenCopy, json->loc, &val, ED_STRICT);
								free(tokenCopy);
								if (ED_OK == ok) {
									a[i] = (int)val;
								}
								else {
									ModelicaFormatError("Could not parse integer from \"%s\" in array at \"%s\" in file \"%s\"\n",
										token, varName, json->fileName);
								}
							}
							else {
								ModelicaError("Memory allocation error\n");
							}
						}
					}
				}
				else {
					ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value dimension does not match requested size \"%s\" in file \"%s\"\n",
					varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

void ED_getBooleanArray1DFromJSON(void* _json, const char* varName, int* a, size_t n)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (n<=json_array_get_count(jsonArray)) {
				size_t i;
				if (JSONBoolean == json_array_get_type(jsonArray)) {
					for (i=0; i<n; i++) {
						a[i] = json_array_get_boolean(jsonArray, i);
					}
				}
				else if (JSONString == json_array_get_type(jsonArray)) {
					for (i=0; i<n; i++) {
						const char* token = json_array_get_string(jsonArray, i);
						if (NULL != token) {
							if (strcmp(token, "true") == 0) {
								a[i] = 1;
							}
							else if (strcmp(token, "false") == 0) {
								a[i] = 0;
							}
							else {
								ModelicaFormatError("Could not parse boolean from \"%s\" at \"%s\" in file \"%s\"\n",
									token, varName, json->fileName);
							}
						}
					}
				}
				else {
					ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value dimension does not match requested size \"%s\" in file \"%s\"\n",
					varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

void ED_getDoubleArray2DFromJSON(void* _json, const char* varName, double* a, size_t m, size_t n)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (m <= json_array_get_count(jsonArray) && n == json_array_check_dimensions2D(jsonArray)) {
				size_t i, j;
				if (JSONNumber == json_array_get_type2D(jsonArray)) {
					for (i=0; i<m; i++) {
						const JSON_Array* subArray = json_array_get_array(jsonArray, i);
						for (j=0; j<n; j++) {
							a[i*n + j] = json_array_get_number(subArray, j);
						}
					}
				}
				else if (JSONString == json_array_get_type2D(jsonArray)) {
					for (i=0; i<m; i++) {
						const JSON_Array* subArray = json_array_get_array(jsonArray, i);
						for (j=0; j<n; j++) {
							const char* token = json_array_get_string(subArray, j);
							if (NULL != token) {
								char* tokenCopy = strdup(token);
								if (NULL != tokenCopy) {
									int ok = ED_strtod(tokenCopy, json->loc, &a[i*n + j], ED_STRICT);
									free(tokenCopy);
									if (ED_OK != ok) {
										ModelicaFormatError("Could not parse double from \"%s\" in array at \"%s\" in file \"%s\"\n",
											token, varName, json->fileName);
									}
								}
								else {
									ModelicaError("Memory allocation error\n");
								}
							}
						}
					}
				}
				else {
					ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value dimensions (%dx%d) do not match requested size (%dx%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), json_array_check_dimensions2D(jsonArray), m, n, varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

void ED_getStringArray2DFromJSON(void* _json, const char* varName, char** a, size_t m, size_t n)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (m <= json_array_get_count(jsonArray) && n == json_array_check_dimensions2D(jsonArray)) {
				if (JSONString == json_array_get_type2D(jsonArray)) {
					size_t i, j;
					for (i=0; i<m; i++) {
						const JSON_Array* subArray = json_array_get_array(jsonArray, i);
						for (j=0; j<n; j++) {
							const char* token = json_array_get_string(subArray, j);
							if (NULL != token) {
								a[i*n + j] = ModelicaAllocateString(strlen(token));
								strcpy(a[i*n + j], token);
							}
						}
					}
				}
				else {
					ModelicaFormatError("Array value is not string \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value dimensions (%dx%d) do not match requested size (%dx%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), json_array_check_dimensions2D(jsonArray), m, n, varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

void ED_getIntArray2DFromJSON(void* _json, const char* varName, int* a, size_t m, size_t n)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (m <= json_array_get_count(jsonArray) && n == json_array_check_dimensions2D(jsonArray)) {
				size_t i, j;
				if (JSONNumber == json_array_get_type2D(jsonArray)) {
					for (i=0; i<m; i++) {
						const JSON_Array* subArray = json_array_get_array(jsonArray, i);
						for (j=0; j<n; j++) {
							a[i*n + j] = (int)json_array_get_number(subArray, j);
						}
					}
				}
				else if (JSONString == json_array_get_type2D(jsonArray)) {
					for (i=0; i<m; i++) {
						const JSON_Array* subArray = json_array_get_array(jsonArray, i);
						for (j=0; j<n; j++) {
							const char* token = json_array_get_string(subArray, j);
							if (NULL != token) {
								char* tokenCopy = strdup(token);
								if (NULL != tokenCopy) {
									long val;
									int ok = ED_strtol(tokenCopy, json->loc, &val, ED_STRICT);
									free(tokenCopy);
									if (ED_OK == ok) {
										a[i*n + j] = val;
									}
									else {
										ModelicaFormatError("Could not parse integer from \"%s\" in array at \"%s\" in file \"%s\"\n",
											token, varName, json->fileName);
									}
								}
								else {
									ModelicaError("Memory allocation error\n");
								}
							}
						}
					}
				}
				else {
					ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value dimensions (%dx%d) do not match requested size (%dx%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), json_array_check_dimensions2D(jsonArray), m, n, varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

void ED_getBooleanArray2DFromJSON(void* _json, const char* varName, int* a, size_t m, size_t n)
{
	JSONFile* json = (JSONFile*)_json;
	ED_PTR_CHECK(json);
	if (json != NULL) {
		if (json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			const JSON_Array* jsonArray = json_object_dotget_array(json->root, varName);
			if (m <= json_array_get_count(jsonArray) && n == json_array_check_dimensions2D(jsonArray)) {
				size_t i, j;
				if (JSONBoolean == json_array_get_type2D(jsonArray)) {
					for (i=0; i<m; i++) {
						const JSON_Array* subArray = json_array_get_array(jsonArray, i);
						for (j=0; j<n; j++) {
							a[i*n + j] = json_array_get_boolean(subArray, j);
						}
					}
				}
				else if (JSONString == json_array_get_type2D(jsonArray)) {
					for (i=0; i<m; i++) {
						const JSON_Array* subArray = json_array_get_array(jsonArray, i);
						for (j=0; j<n; j++) {
							const char* token = json_array_get_string(subArray, j);
							if (NULL != token) {
								if (strcmp(token, "true") == 0) {
									a[i*n + j] = 1;
								}
								else if (strcmp(token, "false") == 0) {
									a[i*n + j] = 0;
								}
								else {
									ModelicaFormatError("Could not parse boolean from \"%s\" at \"%s\" in file \"%s\"\n",
										token, varName, json->fileName);
								}
							}
						}
					}
				}
				else {
					ModelicaFormatError("Array value is not boolean \"%s\" in file \"%s\"\n",
						varName, json->fileName);
				}
			}
			else {
				ModelicaFormatError("Array value dimensions (%dx%d) do not match requested size (%dx%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), json_array_check_dimensions2D(jsonArray), m, n, varName, json->fileName);
			}
		}
		else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
				varName, json->fileName);
		}
	}
}

static JSON_Value_Type json_array_get_type(const JSON_Array *array)
{
	JSON_Value_Type type = JSONNull;
	size_t i;
	size_t n = json_array_get_count(array);
	for (i=0; i<n; i++) {
		JSON_Value* value = json_array_get_value(array, i);
		if (i == 0) {
			type = json_value_get_type(value);
		}
		else if (type != JSONNull && type != json_value_get_type(value)) {
			type = JSONNull;
		}
	}
	return type;
}

static JSON_Value_Type json_array_get_type2D(const JSON_Array *array)
{
	JSON_Value_Type type = JSONNull;
	if (JSONArray == json_array_get_type(array)) {
		size_t i;
		size_t n = json_array_get_count(array);
		for (i=0; i<n; i++) {
			const JSON_Array* subArray = json_array_get_array(array, i);
			if (i == 0) {
				type = json_array_get_type(subArray);
			}
			else if (type != JSONNull && type != json_array_get_type(subArray)) {
				type = JSONNull;
			}
		}
	}
	return type;
}

static int json_array_check_dimensions2D(const JSON_Array *array)
{
	int n = 0;
	if (JSONArray == json_array_get_type(array)) {
		size_t i;
		size_t m = json_array_get_count(array);
		for (i=0; i<m; i++) {
			const JSON_Array* subArray = json_array_get_array(array, i);
			if (i == 0) {
				n = (int)json_array_get_count(subArray);
			}
			else if (n >= 0 && n != json_array_get_count(subArray)) {
				n = -1;
			}
		}
	}
	return n;
}
