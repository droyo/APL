#include <utf.h>
#include <fmt.h>
#include <string.h>
#include <stdlib.h>
#include "apl.h"
#include "error.h"

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
	sizeof (array*), /* function  */
	sizeof (array*), /* dydop     */
	sizeof (array*), /* monop     */
	sizeof (array*), /* niladic   */
	sizeof (array*), /* boxed     */
	sizeof (char),   /* byte      */
};
static int msize(array *a) { 
	return sizeof(int)*a->k + a->n*tsize(a->t); 
}
static int tsize(enum tag t) {
	unsigned long s; for(s=0;t>>=1;s++); 
	return (s>NELEM(type_sizes)?0:type_sizes[s]);
}
long asiz(array *a) {
	return ASIZE + msize(a);
}
/* Suitable for passing to put() */
char *akey(array *a, char *buf, int n) {
	snprint(buf, n, "%*R", a->n, aval(a));
	return buf;
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
		return enil(Enomem);
	a->t=t;a->f = f&~tmpmem;
	a->r=r;a->n=n;a->k=s;a->z=z;
	if(r == 1) *ashp(a) = n;
	record(a); return a;
}
array *acln(array *a) {
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
array *abox(unsigned n, array **x) {
	int i, *s; array *a, **y;
	if(!(a=anew(boxed,0,n>1?1:0,n)))
		return NULL;
	for(i=0,y=aval(a);i<n;i++) {
		if(x[i]->f & tmpmem) {
			if(!(y[i]=acln(x[i]))) return NULL;
		}else
			y[i] = x[i];
	}
	s=ashp(a); s[0] = n;
	return a;
}
array *astr(char *s) {
	array *a;
	if(!(a=anew(string,0,1,utflen(s))))
		return NULL;
	runesnprint(aval(a),utflen(s)+1, "%s", s);
	return a;
}
array *afun(char *s, unsigned n, array **x) {
	int i; array *a, *k, **y;
	if(!(k=astr(s)))             return NULL;
	if(!(a=anew(boxed,0,1,n+1))) return NULL;
	y = aval(a); y[0] = k;
	for(i=0;i<n;i++) {
		y[i+1] = x[i]->f&tmpmem?acln(x[i]):x[i];
		if(!y[i+1]) return NULL;
	}
	a->t = function;
	return a;
}
void *amem(array *a, long sz) {
	void *m = a->m + a->n*tsize(a->t);
	if(a->n+sz > a->z) return NULL;
	else a->n += sz;   return m;
}

void aclr(array *a) { a->n = 0; }
int afull(array *a) { return a->n == a->z; }
