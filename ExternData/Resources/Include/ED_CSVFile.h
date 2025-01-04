/* ED_CSVFile.h - CSV functions header
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_CSVFILE_H)
#define ED_CSVFILE_H

#include <stddef.h>
#include "msvc_compatibility.h"

#if defined(__cplusplus)
extern "C" {
#endif

void* ED_createCSV(const char* fileName, const char* sep, const char* quote, int nHeaderLines, int verbose);
void ED_destroyCSV(void* _csv);
void ED_getDoubleArray2DFromCSV(void* _csv, int* field, double* a, size_t m, size_t n);
void ED_getArray2DDimensionsFromCSV(void* _csv, int* m, int* n);

#if defined(__cplusplus)
}
#endif

#endif
