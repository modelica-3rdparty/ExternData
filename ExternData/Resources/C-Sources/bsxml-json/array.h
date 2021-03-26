/* Data array
 *
 * Copyright (C) 2012-2017, Borislav Sapundzhiev
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    * Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _CPO_ARRAY_H
#define _CPO_ARRAY_H

#include <stdint.h>

#define ARR_VAL(p)  *((uintptr_t*)p)
#define ARR_VAL2PTR(v)  ((uintptr_t)(v))

typedef size_t asize_t;
typedef int (*cmp_func)(const void *, const void *) ;

typedef struct s_array {
    asize_t num;
    asize_t max;
    void *v;
    asize_t elem_size;
    cmp_func cmp;
} cpo_array_t;

cpo_array_t *
cpo_array_create(asize_t size, asize_t elem_size);

void *
cpo_array_get_at(cpo_array_t *a, asize_t index);

void *
cpo_array_push(cpo_array_t *a);

void *
cpo_array_insert_at(cpo_array_t *a, asize_t index);

void *
cpo_array_remove(cpo_array_t *a, asize_t index);

void
cpo_array_qsort(cpo_array_t *a, cmp_func cmp);

void *cpo_array_bsearch(cpo_array_t *a, const void *key, cmp_func cmp);

void
cpo_array_destroy(cpo_array_t *a);
/*stack impl */
void * stack_push(cpo_array_t *stack);
void * stack_pop(cpo_array_t *stack);
void * stack_pop_back(cpo_array_t *stack);
void * stack_push_back(cpo_array_t *stack);
void * stack_back(cpo_array_t *stack);

int array_cmp_int_asc(const void *a,  const void *b);
int array_cmp_int_dsc(const void *a,  const void *b);
int array_cmp_str_asc(const void *a,  const void *b);
int array_cmp_str_dsc(const void *a,  const void *b);

#endif
