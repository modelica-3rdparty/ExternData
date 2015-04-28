/* array.c - data array
 *
 * Copyright (C) 2012 Borislav Sapundzhiev
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
#include <stddef.h>
#include <assert.h>
#include <errno.h>
#include "array.h"

static int
cpo_array_setsize(cpo_array_t *a, int elements);

cpo_array_t *
cpo_array_create(int size, int elem_size)
{
    cpo_array_t *a = malloc(sizeof(cpo_array_t));
    if (a == NULL)
        return NULL;

    a->v = calloc(size, elem_size);
    a->num = 0;
    a->max = size;
    a->elem_size = elem_size;

    return a;
}

static int cpo_array_preallocate(cpo_array_t *a, int elements)
{
    void *newv;
    int newmax = a->max;

    assert(a->num >= 0 && a->num <= a->max);

    while (elements > newmax) {
        newmax = (newmax + 1) * 2;
    }

    newv = malloc(newmax * a->elem_size);

    if (newv == NULL)
        return ENOMEM;

    memcpy(newv, a->v, a->num * a->elem_size);

    if (a->v)
        free(a->v);

    a->v = newv;
    a->max = newmax;
    return 0;
}

static int cpo_array_setsize(cpo_array_t *a, int elements)
{
    int result;

    assert(a->num >= 0 && a->num <= a->max);

    if (elements > a->max) {
        result = cpo_array_preallocate(a, elements);
        if (result) {
            return result;
        }
    }

    a->num = elements;
    return 0;
}

void *
cpo_array_get_at(cpo_array_t *a, int index)
{
    void *elt;
    assert(a->num <= a->max);
    assert(index >= 0 && index <= a->num);

    elt = (unsigned char*) a->v + a->elem_size * index;
    return elt;
}

void *
cpo_array_push(cpo_array_t *a)
{
    int ix, result;
    void * elt;
    ix = a->num;

    result = cpo_array_setsize(a, ix + 1);
    if (result) {
        return NULL;
    }

    elt = (unsigned char*) a->v + a->elem_size * ix;
    return elt;
}

void *
cpo_array_insert_at(cpo_array_t *a, int index)
{
    int i,nmove;
    void *elt;
    elt =  cpo_array_push(a);
    elt =  cpo_array_get_at(a, index);

    if (index < a->num) {

        for (i = a->num-1 ; i >= index -1; i--) {
            nmove = i + 1;
            memmove((unsigned char*)a->v + a->elem_size * nmove,
                    (unsigned char*)a->v + a->elem_size * i, a->elem_size);
        }
    }

    //elt = (unsigned char*) a->v + a->elem_size * index;
    return elt;
}

/*
void *
cpo_array_remove(cpo_array_t *a, int index)
{
    int nmove;
    void *elt;
    assert(a->num <= a->max);
    assert(index >= 0 && index < a->num);

    nmove = a->num - index;

    memmove(a->v, (unsigned char*) a->v + index, nmove * a->elem_size);

    elt = (unsigned char *) a->v + (a->num * a->elem_size);
    a->num--;
    return elt;
}
*/
void *
cpo_array_remove(cpo_array_t *a, int index)
{
    void *elt;
    int i, nmove;
    assert(a->num <= a->max);
    assert(index >= 0 && index < a->num);

    for ( i = index ; i < a->num; i++) {
        nmove = i + 1;

        if (i == index) {
            memmove((unsigned char*)a->v + a->elem_size * a->num,
                    (unsigned char*)a->v + a->elem_size * i, a->elem_size);
        }

        memmove((unsigned char*)a->v + a->elem_size * i,
                (unsigned char*)a->v + a->elem_size * nmove, a->elem_size);

    }

    elt = (unsigned char*)a->v + a->elem_size * a->num;
    a->num--;
    return elt;
}

void cpo_array_destroy(cpo_array_t *a)
{
    if (a->v)
        free(a->v);
    free(a);
}

void cpo_array_qsort(cpo_array_t *a,
                     int (*cmp_func)(const void *, const void *))
{
    qsort(a->v, a->num, a->elem_size, cmp_func);
}

void *cpo_array_bsearch(cpo_array_t *ar, const void *key,
                        int (*compar)(const void *, const void *))
{
    return bsearch(key, ar->v, ar->num, ar->elem_size, compar);
}

int array_cmp_int_asc(const void *a, const void *b)
{
    return (*(int*) a - *(int*) b);
}

int array_cmp_int_dsc(const void *a, const void *b)
{
    return (*(int*) b - *(int*) a);
}

int array_cmp_str_asc(const void *a, const void *b)
{
    return strcmp((char *) a, (char *) b);
}

int array_cmp_str_dsc(const void *a, const void *b)
{
    return strcmp((char *) b, (char *) a);
}

/* d */
void cpo_array_dump_int(cpo_array_t *arr)
{
    int i = 0;
    void* x;
    for (i = 0; i < arr->num; i++) {
        x =  cpo_array_get_at(arr, i);
        printf("[%d] %d\n",i, *((int*)x) );
    }
}

void cpo_array_dump_str(cpo_array_t *arr)
{
    int i = 0;
    for (i = 0; i < arr->num; i++) {
        char *x = cpo_array_get_at(arr, i);
        printf("[%d] %s\n",i, x);
    }
}

/* stack impl */
void * stack_push(cpo_array_t *stack)
{
    if (stack->num == stack->max) {
        fputs("Error: stack overflow\n", stderr);
        return NULL;
    }

    return cpo_array_insert_at(stack, 0);
}

void * stack_push_back(cpo_array_t *stack)
{
    if (stack->num == stack->max) {
        fputs("Error: stack overflow\n", stderr);
        return NULL;
    }

    return cpo_array_push(stack);
}

void * stack_back(cpo_array_t *stack)
{
    return cpo_array_get_at(stack, stack->num -1 );
}

void * stack_pop(cpo_array_t *stack)
{
    if (stack->num == 0) {
        fputs("Error: stack underflow\n", stderr);
        return NULL;
    }

    return  cpo_array_remove(stack, 0);
}

void * stack_pop_back(cpo_array_t *stack)
{
    if (stack->num == 0) {
        fputs("Error: stack underflow\n", stderr);
        return NULL;
    }

    return  cpo_array_remove(stack, stack->num -1);
}

#if _TEST
int main()
{
    int i;
    void *x;
    cpo_array_t arr;

    arr.elem_size = sizeof(int);
    arr.v = calloc(32, sizeof(int));
    arr.num = 0;
    arr.max = 32;

    for (i=0; i< 10; i++) {

        //x = cpo_array_push(&arr);

        x = stack_push(&arr);
        *((int*)x) = i;
    }

    cpo_array_dump_int(&arr);

    for (i=0; i< 10; i++) {
        x = stack_pop_back(&arr);
        printf("pop %d\n", *((int*)x) );
    }
    //printf("ins at %d num %d\n", i, arr.num);
    //x = cpo_array_insert_at(&arr, 6);
    //*((int*)x) = 5000;*/

    cpo_array_dump_int(&arr);
    free(arr.v);
}
#endif