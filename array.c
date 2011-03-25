#include <utf.h>
#include <string.h>
#include <stdlib.h>
#include "apl.h"

#define max(a,b) ((a)>(b))?(a):(b)
static int msize(array *a); 
static int tsize(enum tag);
enum { def_rank = 4 };
enum { def_size = 32};

/* Make sure if you modify the tag order
 * in apl.h that you also modify this table
 * accordingly */
static int type_sizes[] = {
	sizeof (double), /* number    */
	sizeof (Rune),   /* string    */
	sizeof (Rune),   /* symbol    */
	sizeof (void*),  /* function  */
	sizeof (void*),  /* dydop     */
	sizeof (void*),  /* monop     */
	sizeof (void*),  /* niladic   */
	sizeof (array*), /* boxed     */
};
static int msize(array *a) { 
	return sizeof(int)*a->k + a->n*tsize(a->t); 
}
static int tsize(enum tag t) {
	unsigned long s; for(s=0;t>>=1;s++); 
	return (s>NELEM(type_sizes)?0:type_sizes[s]);
}
long asize(array *a) {
	return ASIZE + msize(a);
}
array *atmp(void *p,enum tag t, unsigned r, unsigned n) {
	array *a = p; a->k = max(def_rank,r);
	a->t = t; a->f=tmpmem; 
	a->r = r; a->n = n; a->c = 0;
	return a;
}
array *anew(enum tag t, enum flag f, unsigned r, unsigned n) {
	array *a;
	int s = max(def_rank,r);
	int z = max(def_size,n);
	if(!(a=malloc(ASIZE+sizeof(int)*s+tsize(t)*z)))
		return NULL;
	a->t=t;a->f=f&~tmpmem;
	a->r=r;a->n=n;a->k=s;a->z=z;
	record(a); return a;
}
array *aclone(array *a) {
	array *c = anew(a->t, a->f, a->r, a->n);
	if(!c) return NULL; else memcpy(c->m, a->m, msize(a));
	return c;
}
int *ashp(array *a) {
	return (int*)a->m;
}
void *aval(array *a) {
	return a->m+(sizeof(int)*a->k);
}
