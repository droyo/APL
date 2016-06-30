#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "apl.h"

int apl_item_count(int rank, int dim[]);
size_t apl_buffer_size(apl_array_type t, int rank, int dim[]);
size_t apl_type_size_lut[APL_TYPE_COUNT];

size_t apl_type_size_lut[APL_TYPE_COUNT] = {
	sizeof(double), /* APL_TYPE_REAL */
	sizeof(char) /* APL_TYPE_CHAR */
};

/* Creates a 0-size array of given rank */
apl_array *apl_alloc_array(apl_array_type type, int rank) {
	apl_array *new = apl_emalloc(sizeof *new);
	new->rank = rank;
	new->dim = apl_emalloc((sizeof new->dim[0]) * rank);
	new->type = type;
	return new;
}

void apl_array_free(apl_array *a) {
	free(a->data);
	free(a->dim);
	free(a);
}

/* See http://www.jsoftware.com/papers/APLDictionary1.htm#rho */
apl_array *apl_array_reshape(apl_array *a, int rank, int *dim) {
	size_t new_size = apl_buffer_size(a->type, rank, dim);
	size_t old_size = apl_buffer_size(a->type, a->rank, a->dim);
	void *buf = apl_emalloc(new_size);
	size_t copied = 0;
	size_t upto = old_size;
	
	/*
	The APL Dictionary states that when shrinking
	an array, it is truncated, and when growing an array,
	it is repeated over the new space.
	*/
	while(old_size > 0 && copied < new_size) {
		if(copied + upto > new_size){
			upto = new_size - copied;
		}
		memcpy(buf, a->data, upto);
		copied += upto;
	}
	a->rank = rank;
	free(a->dim);
	a->dim = apl_strdup(dim, rank);
	free(a->data);
	a->data = buf;
	return a;
}

/* utility functions for working with arrays */
int apl_item_count(int rank, int dim[]) {
	int i;
	size_t sz = 1;
	
	for (i = 0; i < rank; i++) {
		sz *= dim[i];
	}
	return sz;
}

size_t apl_buffer_size(apl_array_type t, int rank, int dim[]) {
	return apl_type_size_lut[t] * (size_t)apl_item_count(rank, dim);
}
