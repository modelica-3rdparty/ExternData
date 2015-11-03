/* ED_2D.c - 2D array functions
 *
 * Copyright (C) 2015, tbeu
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
