/* ED_XML2File.h - XML2 functions header
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_XML2FILE_H)
#define ED_XML2FILE_H

#include <stddef.h>
#include "msvc_compatibility.h"

#if defined(__cplusplus)
extern "C" {
#endif

void* ED_createXML2(const char* fileName, const char** ns, size_t sizeNS, int verbose, int detectMissingData);
void ED_destroyXML2(void* _xml);
double ED_getDoubleFromXML2(void* _xml, const char* varName, int* exist);
const char* ED_getStringFromXML2(void* _xml, const char* varName, int* exist);
int ED_getIntFromXML2(void* _xml, const char* varName, int* exist);
void ED_getDoubleArray1DFromXML2(void* _xml, const char* varName, double* a, size_t n);
void ED_getDoubleArray2DFromXML2(void* _xml, const char* varName, double* a, size_t m, size_t n);
void ED_getArray1DDimensionFromXML2(void* _xml, const char* varName, int* n);
void ED_getArray2DDimensionsFromXML2(void* _xml, const char* varName, int* m, int* n);

#if defined(__cplusplus)
}
#endif

#endif
