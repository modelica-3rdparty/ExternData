/* ED_XLSFile.h - Excel XLS functions header
 *
 * Copyright (C) 2015-2026, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_XLSFILE_H)
#define ED_XLSFILE_H

#include <stddef.h>
#include "msvc_compatibility.h"

#if defined(__cplusplus)
extern "C" {
#endif

void* ED_createXLS(const char* fileName, const char* encoding, int verbose, int detectMissingData);
void ED_destroyXLS(void* _xls);
double ED_getDoubleFromXLS(void* _xls, const char* cellAddress, const char* sheetName, int* exist);
const char* ED_getStringFromXLS(void* _xls, const char* cellAddress, const char* sheetName, int* exist);
int ED_getIntFromXLS(void* _xls, const char* cellAddress, const char* sheetName, int* exist);
void ED_getDoubleArray2DFromXLS(void* _xls, const char* cellAddress, const char* sheetName, double* a, size_t m, size_t n);
void ED_getArray2DDimensionsFromXLS(void* _xls, const char* sheetName, int* m, int* n);

#if defined(__cplusplus)
}
#endif

#endif
