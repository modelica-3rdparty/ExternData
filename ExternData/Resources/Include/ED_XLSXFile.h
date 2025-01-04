/* ED_XLSXFile.h - Excel XLSX functions header
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_XLSXFILE_H)
#define ED_XLSXFILE_H

#include <stddef.h>
#include "msvc_compatibility.h"

#if defined(__cplusplus)
extern "C" {
#endif

void* ED_createXLSX(const char* fileName, int verbose, int detectMissingData);
void ED_destroyXLSX(void* _xlsx);
double ED_getDoubleFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist);
const char* ED_getStringFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist);
int ED_getIntFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, int* exist);
void ED_getDoubleArray2DFromXLSX(void* _xlsx, const char* cellAddress, const char* sheetName, double* a, size_t m, size_t n);
void ED_getArray2DDimensionsFromXLSX(void* _xlsx, const char* sheetName, int* m, int* n);

#if defined(__cplusplus)
}
#endif

#endif
