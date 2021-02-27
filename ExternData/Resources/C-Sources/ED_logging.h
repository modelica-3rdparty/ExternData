/* ED_logging.h - Logging functions
 *
 * Copyright (C) 2021, tbeu
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
