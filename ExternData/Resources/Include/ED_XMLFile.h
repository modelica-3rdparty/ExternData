/* ED_XMLFile.h - XML functions header
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_XMLFILE_H)
#define ED_XMLFILE_H

#include <stddef.h>
#include "msvc_compatibility.h"

#if defined(__cplusplus)
extern "C" {
#endif

void* ED_createXML(const char* fileName, int verbose, int detectMissingData);
void ED_destroyXML(void* _xml);
double ED_getDoubleFromXML(void* _xml, const char* varName, int* exist);
const char* ED_getStringFromXML(void* _xml, const char* varName, int* exist);
int ED_getIntFromXML(void* _xml, const char* varName, int* exist);
void ED_getDoubleArray1DFromXML(void* _xml, const char* varName, double* a, size_t n);
void ED_getDoubleArray2DFromXML(void* _xml, const char* varName, double* a, size_t m, size_t n);
void ED_getArray1DDimensionFromXML(void* _xml, const char* varName, int* n);
void ED_getArray2DDimensionsFromXML(void* _xml, const char* varName, int* m, int* n);

#if defined(__cplusplus)
}
#endif

#endif
