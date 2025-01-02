/* ED_logging.h - Logging functions
 *
 * Copyright (C) 2021-2025, Thomas Beutlich
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 */

#if !defined(ED_LOGGING_H)
#define ED_LOGGING_H

#include "ModelicaUtilities.h"

enum {
	ED_LOG_NONE = 1,
	ED_LOG_DEBUG,
	ED_LOG_WARNING,
	ED_LOG_ERROR
};

typedef void (*ED_LOGGING_FUNC)(const char *, ...);

static void ED_LogNone(const char *string, ...) {
}

static void ED_LogDebug(const char *string, ...) {
	va_list args;
	va_start(args, string);
	ModelicaVFormatMessage(string, args);
	va_end(args);
}

static void ED_LogWarning(const char *string, ...) {
	va_list args;
	va_start(args, string);
	ModelicaVFormatWarning(string, args);
	va_end(args);
}

static void ED_LogError(const char *string, ...) {
	va_list args;
	va_start(args, string);
	ModelicaVFormatError(string, args);
	va_end(args);
}

#endif
