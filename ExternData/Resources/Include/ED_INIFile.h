/* ED_INIFile.h - INI functions header
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_INIFILE_H)
#define ED_INIFILE_H

#include "msvc_compatibility.h"

void* ED_createINI(const char* fileName, int verbose, int detectMissingData);
void ED_destroyINI(void* _ini);
double ED_getDoubleFromINI(void* _ini, const char* varName, const char* section, int strict, int* exist);
const char* ED_getStringFromINI(void* _ini, const char* varName, const char* section, int* exist);
int ED_getIntFromINI(void* _ini, const char* varName, const char* section, int strict, int* exist);

#endif
