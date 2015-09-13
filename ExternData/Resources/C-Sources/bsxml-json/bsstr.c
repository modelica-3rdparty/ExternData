/* String utility
 *
 * Copyright (C) 2014 Borislav Sapundzhiev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or (at
 * your option) any later version.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>
#include "bsstr.h"

struct bsstr {
    char *buf;
    int length;
    int allocated;
};

#define ALLOC_BLOCK 64

bsstr *bsstr_create(const char *data)
{
    bsstr *str = (bsstr *)malloc(sizeof (bsstr));
    if (!str) return NULL;
    if (data != NULL) {
        int len = (int)strlen(data);
        str->allocated = (ALLOC_BLOCK > len) ? ALLOC_BLOCK : (len+1);
        str->buf = (char*)calloc(str->allocated, 1);
        if (str->buf) {
            str->length = len;
            strncpy(str->buf, data, len+1);
        }
    } else {
        str->allocated = ALLOC_BLOCK;
        str->buf = (char*)calloc(str->allocated, 1);
        str->length = 0;
    }

    return str;
}

static int bsstr_realloc(bsstr *str , int len)
{
    if (str->allocated <= len) {
        char *_new;
        int new_size = str->allocated * 2;

        if (new_size < len)
            new_size += len;

        str->allocated += len + 1;
        _new = (char*)realloc(str->buf , new_size +1);
        if (_new) {
            str->buf = _new;
            str->allocated = new_size;
        }
        return str->allocated;
    }

    return 0;
}

void bsstr_delete(bsstr *str)
{
    free(str->buf);
    free(str);
}

void bsstr_printf(bsstr* str, char* format, ...)
{
    va_list ap;
    int n, size = 100;
    int end = str->length;
    while (1) {
        bsstr_realloc(str, end + size + 1);
        va_start(ap, format);
        n = vsnprintf(str->buf + end, size, format, ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str->length = end + n;
            return;
        }
        size *= 2;
    }
}

void bsstr_add(bsstr* str, const char* string)
{
    int len = (int)strlen(string);
    bsstr_add_size(str, string, len);
}

void bsstr_addchr(bsstr* str, char ch)
{
    bsstr_realloc(str, str->length + 1);
    str->buf[str->length] = ch;
    str->length++;
    str->buf[str->length] = '\0';
}

void bsstr_add_size(bsstr* str, const char* string, int len)
{
    int end = str->length;
    bsstr_realloc(str, str->length + len+1);
    strncpy(str->buf + end, string, len+1);
    str->length += len;
    str->buf[str->length] = '\0';
}

char *bsstr_get_copy(bsstr* str)
{
    char* result = (char*)malloc(str->allocated +1);
    if (!result) return NULL;
    strncpy(result, str->buf, str->length);
    result[str->length] = '\0';
    return result;
}

char* bsstr_get_bufref(bsstr* str)
{
    return str->buf;
}

char *bsstr_get_buf(bsstr* str)
{
    char* result = str->buf;
    str->allocated = ALLOC_BLOCK;
    str->buf = (char*)calloc(str->allocated, 1);
    str->length = 0;
    return result;
}

char* bsstr_release(bsstr* str)
{
    char* result = str->buf;
    free(str);
    return result;
}

int bsstr_length(bsstr *str)
{
    return str->length;
}

void bsstr_clear(bsstr* str)
{
    str->length = 0;
    str->buf[0] = '\0';
}
