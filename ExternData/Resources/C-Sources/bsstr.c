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
    int lenght;
    int allocated;
};

#define ALLOC_BLOCK 64

bsstr *bsstr_create(const char *data)
{
    bsstr *str = (bsstr *)malloc(sizeof (bsstr));
    if (!str) return NULL;
    if (data != NULL) {
        int len = strlen(data);
        str->allocated = (ALLOC_BLOCK > len) ? ALLOC_BLOCK : (len+1);
        str->buf = calloc(str->allocated, 1);
        if (str->buf) {
            str->lenght = len;
            strncpy(str->buf, data, len+1);
        }
    } else {
        str->allocated = ALLOC_BLOCK;
        str->buf = calloc(str->allocated, 1);
        str->lenght = 0;
    }

    return str;
}

static int bsstr_realloc(bsstr *str , int len)
{
    if (str->allocated <= len) {
        char *new;
        int new_size = str->allocated * 2;

        if (new_size < len)
            new_size += len;

        str->allocated += len + 1;
        new = realloc(str->buf , new_size +1);
        if (new) {
            str->buf = new;
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
    int end = str->lenght;
    while (1) {
        bsstr_realloc(str, end + size + 1);
        va_start(ap, format);
        n = vsnprintf(str->buf + end, size, format, ap);
        va_end(ap);
        if (n > -1 && n < size) {
            str->lenght = end + n;
            return;
        }
        size *= 2;
    }
}

void bsstr_add(bsstr* str, const char* string)
{
    int len = strlen(string);
    bsstr_add_size(str, string, len);
}

void bsstr_addchr(bsstr* str, char ch)
{
    bsstr_realloc(str, str->lenght + 1);
    str->buf[str->lenght] = ch;
    str->lenght++;
    str->buf[str->lenght] = '\0';
}

void bsstr_add_size(bsstr* str, const char* string, int len)
{
    int end = str->lenght;
    bsstr_realloc(str, str->lenght + len+1);
    strncpy(str->buf + end, string, len+1);
    str->lenght += len;
    str->buf[str->lenght] = '\0';
}

char *bsstr_get_copy(bsstr* str)
{
    char* result = malloc(str->allocated +1);
    if (!result) return NULL;
    strncpy(result, str->buf, str->lenght);
    result[str->lenght] = '\0';
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
    str->buf = calloc(str->allocated, 1);
    str->lenght = 0;
    return result;
}

char* bsstr_release(bsstr* str)
{
    char* result = str->buf;
    free(str);
    return result;
}

int bsstr_lenght(bsstr *str)
{
    return str->lenght;
}

void bsstr_clear(bsstr* str)
{
    str->lenght = 0;
    str->buf[0] = '\0';
}
