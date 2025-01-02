/* ED_MATFile.h - MAT functions header
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_MATFILE_H)
#define ED_MATFILE_H

#include <stdlib.h>
#include "msvc_compatibility.h"

void* ED_createMAT(const char* fileName, int verbose);
void ED_destroyMAT(void* _mat);
void ED_getDoubleArray2DFromMAT(void* _mat, const char* varName, double* a, size_t m, size_t n);
void ED_getStringArray1DFromMAT(void* _mat, const char* varName, const char** a, size_t m);
void ED_getArray2DDimensionsFromMAT(void* _mat, const char* varName, int* m, int* n);

#endif
