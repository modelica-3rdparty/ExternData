/******************************************************************************
* zstring_rtrim.h
* Copyright (c) 2016-2018, Fehmi Noyan ISI fnoyanisi@yahoo.com
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
*
* THIS SOFTWARE IS PROVIDED BY Fehmi Noyan ISI ''AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Fehmi Noyan ISI BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* Description :
*  C function to trim trailing white-spaces from a character string
*
*  Function arguments:
*   char *zstring_trim(char *str)
*      - str is the string of char that is subject to right-trim operation
*
*  Return values
*      - the resulting string is returned
*
*  Example Usage
*      char s[]="     Free software is a matter of liberty.     ";
*      printf("%sLike free speech!\n",zstring_ltrim(s));
*
*  Example Output
*           Free software is a matter of liberty.Like free speech!
*************************************************************************/
#ifndef ZSTRING_RTRIM_H
#define ZSTRING_RTRIM_H

#include <stdio.h>

char *
zstring_rtrim(char *str){
    char *i = str; /* last non-space character */
    char *p = str; /* iterator */
    if (str != NULL) {
        while (*p != '\0'){
            if (*p != '\t' && *p != '\v' && *p != '\f' &&
                *p != '\n' && *p != '\r' && *p != ' ') {
                i = p;
            }
            p++;
        }

        if (*i != '\0'){
            *(i+1) = '\0'; /* terminate the string */
        }
    }

    return str;
}

#endif
