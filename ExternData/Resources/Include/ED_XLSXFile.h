/* ED_XLSXFile.h - Excel XLSX functions header
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

#if !defined(ED_XLSXFILE_H)
#define ED_XLSXFILE_H

#include "msvc_compatibility.h"

void* ED_createXLSX(const char* fileName, int verbose);
void ED_destroyXLSX(void* _xlsx);
double ED_getDoubleFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist);
const char* ED_getStringFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist);
int ED_getIntFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist);
void ED_getDoubleArray2DFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, double* a, size_t m, size_t n);
void ED_getArray2DDimensionsFromXLSX(void* _xlsx, const char* sheetName, int* m, int* n);

#endif
