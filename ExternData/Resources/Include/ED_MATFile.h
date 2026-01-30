/* ED_MATFile.h - MAT functions header
 *
 * Copyright (C) 2015-2026, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_MATFILE_H)
#define ED_MATFILE_H

#include <stddef.h>
#include "msvc_compatibility.h"

#if defined(__cplusplus)
extern "C" {
#endif

void* ED_createMAT(const char* fileName, int verbose);
void ED_destroyMAT(void* _mat);
void ED_getDoubleArray2DFromMAT(void* _mat, const char* varName, double* a, size_t m, size_t n);
void ED_getStringArray1DFromMAT(void* _mat, const char* varName, const char** a, size_t m);
void ED_getArray2DDimensionsFromMAT(void* _mat, const char* varName, int* m, int* n);

#if defined(__cplusplus)
}
#endif

#endif
