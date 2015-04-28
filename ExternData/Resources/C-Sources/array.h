#ifndef _CPO_ARRAY_H
#define _CPO_ARRAY_H

#define ARR_VAL(p) 	*((long*)p)
#define ARR_VAL2PTR(v) 	((long)(v))

typedef struct s_array {
	int num;
	int max;
	void *v;
	int elem_size;
} cpo_array_t;

cpo_array_t *
cpo_array_create(int size , int elem_size);

void *
cpo_array_get_at(cpo_array_t *a, int index);

void *
cpo_array_push(cpo_array_t *a);

void * 
cpo_array_insert_at(cpo_array_t *a, int index);

void *
cpo_array_remove(cpo_array_t *a, int index);

void 
cpo_array_qsort(cpo_array_t *a, int (*cmp_func)(const void *, const void *));

void *cpo_array_bsearch(cpo_array_t *ar, const void *key,
                        int (*compar)(const void *, const void *));

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
