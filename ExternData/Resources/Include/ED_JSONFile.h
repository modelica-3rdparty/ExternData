/* ED_JSONFile.h - JSON functions header
 *
 * Copyright (C) 2015-2026, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_JSONFILE_H)
#define ED_JSONFILE_H

#include <stddef.h>
#include "msvc_compatibility.h"

#if defined(__cplusplus)
extern "C" {
#endif

void* ED_createJSON(const char* fileName, int verbose, int detectMissingData);
void ED_destroyJSON(void* _json);
double ED_getDoubleFromJSON(void* _json, const char* varName, int* exist);
const char* ED_getStringFromJSON(void* _json, const char* varName, int* exist);
int ED_getIntFromJSON(void* _json, const char* varName, int* exist);
int ED_getBooleanFromJSON(void* _json, const char* varName, int* exist);
void ED_getArray1DDimensionFromJSON(void* _json, const char* varName, int* n);
void ED_getArray2DDimensionsFromJSON(void* _json, const char* varName, int* m, int* n);
void ED_getDoubleArray1DFromJSON(void* _json, const char* varName, double* a, size_t n);
void ED_getStringArray1DFromJSON(void* _json, const char* varName, char** a, size_t n);
void ED_getIntArray1DFromJSON(void* _json, const char* varName, int* a, size_t n);
void ED_getBooleanArray1DFromJSON(void* _json, const char* varName, int* a, size_t n);
void ED_getDoubleArray2DFromJSON(void* _json, const char* varName, double* a, size_t m, size_t n);
void ED_getStringArray2DFromJSON(void* _json, const char* varName, char** a, size_t m, size_t n);
void ED_getIntArray2DFromJSON(void* _json, const char* varName, int* a, size_t m, size_t n);
void ED_getBooleanArray2DFromJSON(void* _json, const char* varName, int* a, size_t m, size_t n);

#if defined(__cplusplus)
}
#endif

#endif
