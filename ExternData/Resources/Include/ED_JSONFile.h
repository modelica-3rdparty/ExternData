/* ED_JSONFile.h - JSON functions header
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

#if !defined(ED_JSONFILE_H)
#define ED_JSONFILE_H

#include "msvc_compatibility.h"

void* ED_createJSON(const char* fileName, int verbose);
void ED_destroyJSON(void* _json);
double ED_getDoubleFromJSON(void* _json, const char* varName);
const char* ED_getStringFromJSON(void* _json, const char* varName);
int ED_getIntFromJSON(void* _json, const char* varName);
int ED_getBooleanFromJSON(void* _json, const char* varName);
void ED_getArrayDimensionFromJSON(void* _json, const char* varName, size_t* n);
void ED_getArray2DDimensionsFromJSON(void* _json, const char* varName, size_t* n, size_t* m);
void ED_getDoubleArrayFromJSON(void* _json, const char* varName, double* arr, size_t n);
void ED_getStringArrayFromJSON(void* _json, const char* varName, char** arr, size_t n);
void ED_getIntArrayFromJSON(void* _json, const char* varName, int* arr, size_t n);
void ED_getBooleanArrayFromJSON(void* _json, const char* varName, int* arr, size_t n);
void ED_getDoubleArray2DFromJSON(void* _json, const char* varName, double* arr, size_t n, size_t m);
void ED_getStringArray2DFromJSON(void* _json, const char* varName, char** arr, size_t n, size_t m);
void ED_getIntArray2DFromJSON(void* _json, const char* varName, int* arr, size_t n, size_t m);
void ED_getBooleanArray2DFromJSON(void* _json, const char* varName, int* arr, size_t n, size_t m);

#endif
