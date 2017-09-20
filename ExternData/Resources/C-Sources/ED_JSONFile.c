/* ED_JSONFile.c - JSON functions
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

#if defined(__gnu_linux__)
#define _GNU_SOURCE 1
#endif

#include <string.h>
#if defined(_MSC_VER)
#define strdup _strdup
#endif
#include "ED_locale.h"
#include "parson/parson.h"
#include "ModelicaUtilities.h"
#include "../Include/ED_JSONFile.h"
#include <stdio.h>

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

JSON_Value_Type json_array_get_type(const JSON_Array *array);
JSON_Value_Type json_array_get_type2D(const JSON_Array *array);
int json_array_check_dimensions2D(const JSON_Array *array);

typedef struct {
	char* fileName;
	JSON_Object* root;
	ED_LOCALE_TYPE loc;
} JSONFile;

void* ED_createJSON(const char* fileName, int verbose)
{
	JSON_Value* rootValue;
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

	rootValue = json_parse_file_with_comments(fileName);
	json->root = json_value_get_object(rootValue);
	if (json->root == NULL) {
		free(json->fileName);
		free(json);
	    ModelicaFormatError("Failed to parse json file \"%s\"\n", fileName);
		return NULL;
	}

	ModelicaFormatMessage("Loaded JSON data:\n%s\n", json_serialize_to_string(rootValue));

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
		json_object_clear(json->root);
		ED_FREE_LOCALE(json->loc);
		free(json);
	}
}

double ED_getDoubleFromJSON(void* _json, const char* varName)
{
	char* strValue;
	double ret = 0;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONNumber)) {
			return json_object_dotget_number(json->root, varName);
		} else if(json_object_dothas_value_of_type(json->root, varName, JSONString)) {
			strValue = strdup(json_object_dotget_string(json->root, varName));
			if (ED_strtod(strValue, json->loc, &ret)) {
				ModelicaFormatError("Could not parse double from \"%s\" at \"%s\" in file \"%s\"\n",
					strValue, varName, json->fileName);
			}
			free(strValue);
		} else {
			ModelicaFormatError("Cannot find numeric value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
	return ret;
}

const char* ED_getStringFromJSON(void* _json, const char* varName)
{
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONString)) {
			return json_object_dotget_string(json->root, varName);
		} else {
			ModelicaFormatError("Cannot find string value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
	return "";
}

int ED_getIntFromJSON(void* _json, const char* varName)
{
	char* strValue;
	long ret = 0;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONNumber)) {
			return (int)json_object_dotget_number(json->root, varName);
		} else if(json_object_dothas_value_of_type(json->root, varName, JSONString)) {
			strValue = strdup(json_object_dotget_string(json->root, varName));
			if (ED_strtol(strValue, json->loc, &ret)) {
				ModelicaFormatError("Could not parse integer from \"%s\" at \"%s\" in file \"%s\"\n",
					strValue, varName, json->fileName);
			}
			free(strValue);
		} else {
			ModelicaFormatError("Cannot find numeric value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
	return (int)ret;
}

int ED_getBooleanFromJSON(void* _json, const char* varName)
{
	char* strValue;
	int ret = 0;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONBoolean)) {
			return json_object_dotget_boolean(json->root, varName);
		} else if(json_object_dothas_value_of_type(json->root, varName, JSONString)) {
			strValue = strdup(json_object_dotget_string(json->root, varName));
			if(strcmp(strValue, "true")==0) {
				ret = 1;
			} else if(strcmp(strValue, "false")==0) {
				ret = 0;
			} else {
				ModelicaFormatError("Could not parse boolean from \"%s\" at \"%s\" in file \"%s\"\n",
					strValue, varName, json->fileName);
			}
			free(strValue);
		} else {
			ModelicaFormatError("Cannot find boolean value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
	return ret;
}

void ED_getArrayDimensionFromJSON(void* _json, const char* varName, size_t* n) {
	JSON_Array* jsonArray;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			*n = json_array_get_count(jsonArray);
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getArray2DDimensionsFromJSON(void* _json, const char* varName, size_t* n, size_t* m) {
	JSON_Array* jsonArray;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(JSONArray==json_array_get_type(jsonArray)) {
				*n = json_array_get_count(jsonArray);
				*m = json_array_check_dimensions2D(jsonArray);
				if(*m==-1) {
					ModelicaFormatError("Array value has 2 dimensions, but not all rows have same column dimension \"%s\" in file \"%s\"\n",
					varName, json->fileName);
				}
			} else {
				ModelicaFormatError("Array value has 1 dimension, not 2 \"%s\" in file \"%s\"\n",
					varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getDoubleArrayFromJSON(void* _json, const char* varName, double* arr, size_t n) {
	JSON_Array* jsonArray;
	size_t i;
	char* strValue;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(n<=json_array_get_count(jsonArray)) {
			    if(JSONNumber==json_array_get_type(jsonArray)) {
                    for(i=0;i<n;i++) {
						arr[i] = json_array_get_number(jsonArray, i);
					}
			    } else if(JSONString==json_array_get_type(jsonArray)) {
                    for(i=0;i<n;i++) {
						strValue = strdup(json_array_get_string(jsonArray, i));
						if(ED_strtod(strValue, json->loc, &(arr[i]))) {
							ModelicaFormatError("Could not parse double from \"%s\" in array at \"%s\" in file \"%s\"\n",
					           strValue, varName, json->fileName);
						}
						free(strValue);
					}
			    } else {
			    	ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
			    		varName, json->fileName);
			    }
			} else {
				ModelicaFormatError("Array value dimension (%d) does not match requested size (%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), n, varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getStringArrayFromJSON(void* _json, const char* varName, char** arr, size_t n) {
	JSON_Array* jsonArray;
	size_t i;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(n<=json_array_get_count(jsonArray)) {
			    if(JSONString==json_array_get_type(jsonArray)) {
                    for(i=0;i<n;i++) {
						arr[i] = strdup(json_array_get_string(jsonArray, i));
					}
			    } else {
			    	ModelicaFormatError("Array value is not string \"%s\" in file \"%s\"\n",
			    		varName, json->fileName);
			    }
			} else {
				ModelicaFormatError("Array value dimension does not match requested size \"%s\" in file \"%s\"\n",
					varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getIntArrayFromJSON(void* _json, const char* varName, int* arr, size_t n) {
	JSON_Array* jsonArray;
	size_t i;
	char* strValue;
	long lValue;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(n<=json_array_get_count(jsonArray)) {
			    if(JSONNumber==json_array_get_type(jsonArray)) {
                    for(i=0;i<n;i++) {
						arr[i] = (int)json_array_get_number(jsonArray, i);
					}
			    } else if(JSONString==json_array_get_type(jsonArray)) {
                    for(i=0;i<n;i++) {
						strValue = strdup(json_array_get_string(jsonArray, i));
						if(ED_strtol(strValue, json->loc, &lValue)) {
							ModelicaFormatError("Could not parse integer from \"%s\" in array at \"%s\" in file \"%s\"\n",
					           strValue, varName, json->fileName);
						}
						arr[i] = (int)lValue;
						free(strValue);
					}
			    } else {
			    	ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
			    		varName, json->fileName);
			    }
			} else {
				ModelicaFormatError("Array value dimension does not match requested size \"%s\" in file \"%s\"\n",
					varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getBooleanArrayFromJSON(void* _json, const char* varName, int* arr, size_t n) {
	JSON_Array* jsonArray;
	size_t i;
	char* strValue;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(n<=json_array_get_count(jsonArray)) {
			    if(JSONBoolean==json_array_get_type(jsonArray)) {
                    for(i=0;i<n;i++) {
						arr[i] = json_array_get_boolean(jsonArray, i);
					}
			    } else if(JSONString==json_array_get_type(jsonArray)) {
                    for(i=0;i<n;i++) {
						strValue = strdup(json_array_get_string(jsonArray, i));
						if(strcmp(strValue, "true")==0) {
				            arr[i] = 1;
			            } else if(strcmp(strValue, "false")==0) {
				            arr[i] = 0;
			            } else {
				            ModelicaFormatError("Could not parse boolean from \"%s\" at \"%s\" in file \"%s\"\n",
					            strValue, varName, json->fileName);
			            }
						free(strValue);
					}
			    } else {
			    	ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
			    		varName, json->fileName);
			    }
			} else {
				ModelicaFormatError("Array value dimension does not match requested size \"%s\" in file \"%s\"\n",
					varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getDoubleArray2DFromJSON(void* _json, const char* varName, double* arr, size_t n, size_t m) {
	JSON_Array* jsonArray;
	JSON_Array* subArray;
	size_t i, j;
	char* strValue;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(n<=json_array_get_count(jsonArray) && m==json_array_check_dimensions2D(jsonArray)) {
			    if(JSONNumber==json_array_get_type2D(jsonArray)) {
                    for(i=0;i<n;i++) {
						subArray = json_array_get_array(jsonArray, i);
						for(j=0;j<m;j++) {
						    arr[i*m + j] = json_array_get_number(subArray, j);
						}
					}
			    } else if(JSONString==json_array_get_type2D(jsonArray)) {
					for(i=0;i<n;i++) {
						subArray = json_array_get_array(jsonArray, i);
						for(j=0;j<m;j++) {
							strValue = strdup(json_array_get_string(jsonArray, i));
						    if(ED_strtod(strValue, json->loc, &(arr[i*m + j]))) {
							    ModelicaFormatError("Could not parse double from \"%s\" in array at \"%s\" in file \"%s\"\n",
					               strValue, varName, json->fileName);
						    }
						    free(strValue);
						}
					}
			    } else {
			    	ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
			    		varName, json->fileName);
			    }
			} else {
				ModelicaFormatError("Array value dimensions (%dx%d) do not match requested size (%dx%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), json_array_check_dimensions2D(jsonArray), n, m, varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getStringArray2DFromJSON(void* _json, const char* varName, char** arr, size_t n, size_t m) {
	JSON_Array* jsonArray;
	JSON_Array* subArray;
	size_t i, j;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(n<=json_array_get_count(jsonArray) && m==json_array_check_dimensions2D(jsonArray)) {
			    if(JSONString==json_array_get_type2D(jsonArray)) {
                    for(i=0;i<n;i++) {
						subArray = json_array_get_array(jsonArray, i);
						for(j=0;j<m;j++) {
						    arr[i*m + j] = strdup(json_array_get_string(subArray, j));
						}
					}
			    } else {
			    	ModelicaFormatError("Array value is not string \"%s\" in file \"%s\"\n",
			    		varName, json->fileName);
			    }
			} else {
				ModelicaFormatError("Array value dimensions (%dx%d) do not match requested size (%dx%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), json_array_check_dimensions2D(jsonArray), n, m, varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getIntArray2DFromJSON(void* _json, const char* varName, int* arr, size_t n, size_t m) {
	JSON_Array* jsonArray;
	JSON_Array* subArray;
	size_t i, j;
	char* strValue;
	long lValue;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(n<=json_array_get_count(jsonArray) && m==json_array_check_dimensions2D(jsonArray)) {
			    if(JSONNumber==json_array_get_type2D(jsonArray)) {
                    for(i=0;i<n;i++) {
						subArray = json_array_get_array(jsonArray, i);
						for(j=0;j<m;j++) {
						    arr[i*m + j] = (int)json_array_get_number(subArray, j);
						}
					}
			    } else if(JSONString==json_array_get_type2D(jsonArray)) {
					for(i=0;i<n;i++) {
						subArray = json_array_get_array(jsonArray, i);
						for(j=0;j<m;j++) {
							strValue = strdup(json_array_get_string(jsonArray, i));
						    if(ED_strtol(strValue, json->loc, &lValue)) {
							    ModelicaFormatError("Could not parse double from \"%s\" in array at \"%s\" in file \"%s\"\n",
					               strValue, varName, json->fileName);
						    }
							arr[i*m + j] = lValue;
						    free(strValue);
						}
					}
			    } else {
			    	ModelicaFormatError("Array value is not numeric \"%s\" in file \"%s\"\n",
			    		varName, json->fileName);
			    }
			} else {
				ModelicaFormatError("Array value dimensions (%dx%d) do not match requested size (%dx%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), json_array_check_dimensions2D(jsonArray), n, m, varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

void ED_getBooleanArray2DFromJSON(void* _json, const char* varName, int* arr, size_t n, size_t m) {
	JSON_Array* jsonArray;
	JSON_Array* subArray;
	size_t i, j;
	char* strValue;
	JSONFile* json = (JSONFile*)_json;
	if (json != NULL) {
		if(json_object_dothas_value_of_type(json->root, varName, JSONArray)) {
			jsonArray = json_object_dotget_array(json->root, varName);
			if(n<=json_array_get_count(jsonArray) && m==json_array_check_dimensions2D(jsonArray)) {
			    if(JSONBoolean==json_array_get_type2D(jsonArray)) {
                    for(i=0;i<n;i++) {
						subArray = json_array_get_array(jsonArray, i);
						for(j=0;j<m;j++) {
						    arr[i*m + j] = json_array_get_boolean(subArray, j);
						}
					}
			    } else if(JSONString==json_array_get_type2D(jsonArray)) {
					for(i=0;i<n;i++) {
						subArray = json_array_get_array(jsonArray, i);
						for(j=0;j<m;j++) {
							strValue = strdup(json_array_get_string(jsonArray, i));
							if(strcmp(strValue, "true")==0) {
				                arr[i*m + j] = 1;
			                } else if(strcmp(strValue, "false")==0) {
				                arr[i*m + j] = 0;
			                } else {
				                ModelicaFormatError("Could not parse boolean from \"%s\" at \"%s\" in file \"%s\"\n",
					                strValue, varName, json->fileName);
			                }
						    free(strValue);
						}
					}
			    } else {
			    	ModelicaFormatError("Array value is not boolean \"%s\" in file \"%s\"\n",
			    		varName, json->fileName);
			    }
			} else {
				ModelicaFormatError("Array value dimensions (%dx%d) do not match requested size (%dx%d) \"%s\" in file \"%s\"\n",
					json_array_get_count(jsonArray), json_array_check_dimensions2D(jsonArray), n, m, varName, json->fileName);
			}
		} else {
			ModelicaFormatError("Cannot find array value \"%s\" in file \"%s\"\n",
					varName, json->fileName);
		}
	}
}

JSON_Value_Type json_array_get_type(const JSON_Array *array) {
	JSON_Value* value;
	JSON_Value_Type type;
	size_t n;
	size_t i;
	type = JSONNull;
	n = json_array_get_count(array);
	for(i=0; i<n; i++) {
		value = json_array_get_value(array, i);
		if(i==0) {
			type = json_value_get_type(value);
		} else if(type!=JSONNull && type!=json_value_get_type(value)) {
			type = JSONNull;
		}
	}
	return type;
}

JSON_Value_Type json_array_get_type2D(const JSON_Array *array) {
	JSON_Array* subarray;
	JSON_Value_Type type;
	size_t n;
	size_t i;
	type = JSONNull;
	if(JSONArray==json_array_get_type(array)) {
		n = json_array_get_count(array);
	    for(i=0; i<n; i++) {
			subarray = json_array_get_array(array, i);
			if(i==0) {
			    type = json_array_get_type(subarray);
		    } else if(type!=JSONNull && type!=json_array_get_type(subarray)) {
			    type = JSONNull;
		    }
	    }
	}
	return type;
}

int json_array_check_dimensions2D(const JSON_Array *array) {
	JSON_Array* subarray;
	size_t n;
	size_t m;
	size_t i;
	if(JSONArray==json_array_get_type(array)) {
		n = json_array_get_count(array);
	    for(i=0; i<n; i++) {
			subarray = json_array_get_array(array, i);
			if(i==0) {
			    m = json_array_get_count(subarray);
		    } else if(m>=0 && m!=json_array_get_count(subarray)) {
			    m = -1;
		    }
	    }
	}
	return (int)m;
}
