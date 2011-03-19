#include <string.h>
#include <stdlib.h>
#include "apl.h"

array zilde = {empty, 0, 0, 0, 0, NULL};
/* Make sure if you modify the tag order
 * in apl.h that you also modify this table
 * accordingly */
int type_sizes[] = {
	sizeof (double),	/* number    */
	sizeof (char),		/* string    */
	sizeof (char),		/* symbol    */
	sizeof (array),		/* function  */
	sizeof (array),		/* subcmd    */
	sizeof (array),		/* doperator */
	sizeof (array),		/* moperator */
	sizeof (array),		/* niladic   */
	sizeof (array),		/* boxed     */
};
static int asize(array *a) {
	unsigned long s,t;
	/* Does this depend on endianness? */
	for(s=0,t=a->t;t>>=1;s++);
	return a->n*(s>NELEM(type_sizes)?0:type_sizes[s]);
}
int acopy(array *dst, array *src) {
	long s = sizeof(int)*src->r + asize(src);
	memcpy(dst, src, sizeof *dst);
	if(!(dst->m = malloc(s))) return 0;
	return !!memcpy(dst->m,src->m,s);
}
array *aclone(array *a) {
	array *r;
	if(!(r = malloc(sizeof *r))) return NULL;
	if(!acopy(r,a)) { free(r); return NULL; }
	return a;
}
void *aval(array *a) {
	return a->m + (sizeof (int) * a->r);
}
void afree(array *a) {
	free(a->m); free(a);
}
