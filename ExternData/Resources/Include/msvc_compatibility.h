/* msvc_compatibility.h - MSVC compatibility header
 *
 * Copyright (C) 2015-2024, Thomas Beutlich
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

#if !defined(MSVC_COMPATIBILITY_H)
#define MSVC_COMPATIBILITY_H

#if defined(_MSC_VER)
/* MSVC compatibility handling for linking static libraries compiled by Visual Studio 2010 */

#if _MSC_VER >= 1900

#include <stdio.h>
#include <time.h>
#include "gconstructor.h"

extern long timezone = 0;

#if defined(G_HAS_CONSTRUCTORS)
#ifdef G_DEFINE_CONSTRUCTOR_NEEDS_PRAGMA
#pragma G_DEFINE_CONSTRUCTOR_PRAGMA_ARGS(Timezone_initialize)
#endif
G_DEFINE_CONSTRUCTOR(Timezone_initialize)
static void Timezone_initialize(void) {
	_get_timezone(&timezone);
}
#endif

/* Dymola 2017 FD01 defines __iob_func in dsutil.h */
/* Dymola 2024x R1 no longer defines __iob_func in dsutil.h (by default) */
#if !defined(HACK_SUPPORT_VS2015)
FILE* __cdecl __iob_func(void)
{
	unsigned char* retaddr = (unsigned char*)_ReturnAddress();
#ifndef _M_AMD64

	if (retaddr[0] != 0x83 || retaddr[1] != 0xC0)
		return stdin;
	switch (retaddr[2]) {
		case 0x20: return (FILE*)((char*)stdout - 0x20);
		case 0x40: return (FILE*)((char*)stderr - 0x40);
		default:return stdin;
	}
#else
	if (retaddr[0] != 0x48 || retaddr[1] != 0x83 || retaddr[2] != 0xC0)
		return stdin;
	switch (retaddr[3]) {
		case 0x30: return (FILE*)((char*)stdout - 0x30);
		case 0x60: return (FILE*)((char*)stderr - 0x60);
		default:return stdin;
	}
#endif
}
#endif

#pragma comment( lib, "legacy_stdio_definitions.lib" )

#endif

#endif

#endif
