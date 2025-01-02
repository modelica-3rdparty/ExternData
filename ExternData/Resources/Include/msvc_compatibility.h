/* msvc_compatibility.h - MSVC compatibility header
 *
 * Copyright (C) 2015-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
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
