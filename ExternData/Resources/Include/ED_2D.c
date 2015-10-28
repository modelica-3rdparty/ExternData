/* ED_2D.c - 2D array functions
 *
 * Copyright (C) 2015 tbeu
 *
 * This file is part of ExternData.
 *
 * ExternData is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * ExternData is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ExternData; if not, see http://www.gnu.org/licenses
 *
 */

#if !defined(ED_2D_C)
#define ED_2D_C

#if !defined(ITI_COMP_SIM)

#include <stdlib.h>
#include <string.h>
#include "../C-Sources/ModelicaUtilities.h"
#include "ED_MATFile.h"
#include "ED_XMLFile.h"

#if defined(__GNUC__)
#define MODELICA_NONNULLATTR __attribute__((nonnull))
#else
#define MODELICA_NONNULLATTR
#endif
#if !defined(__ATTR_SAL)
#define _In_
#define _In_z_
#define _Out_
#endif

void ED_getDimDoubleArray2D(_In_z_ const char* fileName, _In_z_ const char* arrayName, _Out_ int* dim) MODELICA_NONNULLATTR;
void ED_getDoubleArray2D(_In_z_ const char* fileName, _In_z_ const char* arrayName, _Out_ double* a, size_t m, size_t n) MODELICA_NONNULLATTR;
int ED_writeDoubleArray2D(_In_z_ const char* fileName, _In_z_ const char* arrayName, _In_ double* a, size_t m, size_t n, int append) MODELICA_NONNULLATTR;

void ED_getDimDoubleArray2D(const char* fileName, const char* arrayName, int* dim)
{
	/* File can be binary MATLAB MAT-file or XML file */
	const char* ext = strrchr(fileName, '.');
	if (ext != NULL) {
		if (0 == strncmp(ext, ".mat", 4) ||
			0 == strncmp(ext, ".MAT", 4)) {
			void* ed = ED_createMAT(fileName);
			if (ed != NULL) {
				ED_getDimDoubleArray2DFromMAT(ed, arrayName, dim);
				ED_destroyMAT(ed);
			}
			return;
		}
		else if (0 == strncmp(ext, ".xml", 4) ||
			0 == strncmp(ext, ".XML", 4)) {
			void* ed = ED_createXML(fileName);
			if (ed != NULL) {
				ED_getDimDoubleArray2DFromXML(ed, arrayName, dim);
				ED_destroyXML(ed);
			}
			return;
		}
	}
	ModelicaFormatError("Function \"ED_getDimDoubleArray2D\" is not implemented for \"%s\"\n", fileName);
}

void ED_getDoubleArray2D(const char* fileName, const char* arrayName, double* a, size_t m, size_t n)
{
	/* File can be binary MATLAB MAT-file or XML file */
	const char* ext = strrchr(fileName, '.');
	if (ext != NULL) {
		if (0 == strncmp(ext, ".mat", 4) ||
			0 == strncmp(ext, ".MAT", 4)) {
			void* ed = ED_createMAT(fileName);
			if (ed != NULL) {
				ED_getDoubleArray2DFromMAT(ed, arrayName, a, m, n);
				ED_destroyMAT(ed);
			}
			return;
		}
		else if (0 == strncmp(ext, ".xml", 4) ||
			0 == strncmp(ext, ".XML", 4)) {
			void* ed = ED_createXML(fileName);
			if (ed != NULL) {
				ED_getDoubleArray2DFromXML(ed, arrayName, a, m, n);
				ED_destroyXML(ed);
			}
			return;
		}
	}
	ModelicaFormatError("Function \"ED_getDoubleArray2D\" is not implemented for \"%s\"\n", fileName);
}

int ED_writeDoubleArray2D(const char* fileName, const char* arrayName, double* a, size_t m, size_t n, int append)
{
	/* File can be binary MATLAB MAT-file */
	const char* ext = strrchr(fileName, '.');
	if (ext != NULL) {
		if (0 == strncmp(ext, ".mat", 4) ||
			0 == strncmp(ext, ".MAT", 4)) {
			int status = 0;
			void* ed = ED_createMAT(fileName);
			if (ed != NULL) {
				status = ED_writeDoubleArray2DToMAT(ed, arrayName, a, m, n, append);
				ED_destroyMAT(ed);
			}
			return status;
		}
	}
	ModelicaFormatError("Function \"ED_writeDoubleArray2D\" is not implemented for \"%s\"\n", fileName);
	return 0;
}

#endif /* !defined(ITI_COMP_SIM) */

#endif
