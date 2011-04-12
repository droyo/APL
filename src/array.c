#include <utf.h>
#include <fmt.h>
#include <string.h>
#include <stdlib.h>
#include "apl.h"
#include "error.h"

static int msize(array *a); 
static int tsize(enum tag);
enum { def_rank = 4 };
enum { def_size = 4 };

/* Make sure if you modify the tag order
 * in apl.h that you also modify this table
 * accordingly */
static int type_sizes[] = {
	sizeof (double), /* TNUM */
	sizeof (Rune),   /* TSTR */
	sizeof (Rune),   /* TSYM */
	sizeof (array*), /* TFUN */
	sizeof (array*), /* TDYA */
	sizeof (array*), /* TMON */
	sizeof (array*), /* TCLK */
	sizeof (array*), /* TBOX */
	sizeof (pair),   /* TREL */
	sizeof (char),   /* TRAW */
};
static int msize(array *a) { 
	return sizeof(int)*a->k + a->z*tsize(a->t); 
}
static int tsize(enum tag t) {
	unsigned s; for(s=0;t>>=1;s++); 
	return (s>NELEM(type_sizes)?0:type_sizes[s]);
}
long asize(array *a) {
	return ASIZE + msize(a);
}
/* Suitable for passing to put() */
char *akey(array *a, char *buf, int n) {
	snprint(buf, n, "%*R", a->n, aval(a));
	return buf;
}
array *atmp(void *p,enum tag t, unsigned r, unsigned n) {
	array *a = p; a->k = MAX(def_rank,r);
	a->t = t; a->f=FTMP; 
	a->r = r; a->n = n; a->c = 0;
	return a;
}
array *anew(array *E,enum tag t, enum flag f, unsigned r, unsigned n) {
	array *a;
	int k = MAX(def_rank,r);
	int z = MAX(def_size,n);
	if(!(a=malloc(ASIZE+sizeof(int)*k+tsize(t)*z)))
		return enil(Enomem);
	a->t=t;a->f = f&~FTMP;
	a->r=r;a->n=n;a->k=k;a->z=z;
	if(r == 1) *ashp(a) = n;
	if(!(f&FSYS)) record(E,a);
	return a;
}
array *acln(array *E, array *a) {
	array *c = anew(E,a->t, a->f, a->r, a->n); 
	if(!c) return NULL; 
	memcpy(ashp(c),ashp(a),sizeof(int)*a->r);
	memcpy(aval(c),aval(a),msize(a)-sizeof(int)*a->k);
	return c;
}
int *ashp(array *a) {
	return (int*)a->m;
}
void *aval(array *a) {
	return a->m+(sizeof(int)*a->k);
}
array *abox(array *E,unsigned n,enum flag f,array **x) {
	int i, *s; array *a, **y;
	if(!(a=anew(E,TBOX,f,n>1?1:0,n)))
		return NULL;
	if(!x) {
		a->n = 0;
		return a;
	}
	for(i=0,y=aval(a);i<n;i++) {
		if(x[i]->f & FTMP) {
			if(!(y[i]=acln(E,x[i]))) return NULL;
		}else
			y[i] = x[i];
	}
	s=ashp(a); s[0] = n;
	return a;
}
array *astr(array *E, char *s) {
	array *a;
	if(!(a=anew(E,TSTR,0,1,utflen(s)+1)))
		return NULL;
	runesnprint(aval(a),a->n,"%s",s);
	return a;
}
void *amem(array *a, long sz) {
	void *m = aget(a,a->n);
	if(a->n+sz > a->z)     return NULL;
	*ashp(a)=(a->n += sz); return m;
}

void aclr(array *a) { a->n = 0; }
int afull(array *a) { return a->n == a->z; }
array *agrow(array **a, long n) {
	array *r;
	if(!(r=realloc(*a,asize(*a)+n*tsize((*a)->t))))
		return NULL;
	else r->z += n; 
	return *a=r;
}
void *aget(array *a, long i) {
	if(i >= a->z) return NULL;
	switch(a->t) {
	case TNUM: return ((double*)aval(a))+i;
	case TSTR: 
	case TSYM: return ((Rune*)aval(a))+i;
	case TFUN: 
	case TBOX: return ((array**)aval(a))+i;
	case TCLK: return ((void*)aval(a))+i;
	case TRAW: return ((char*)aval(a))+i;
	case TREL: return ((pair*)aval(a))+i;
	default:   return NULL;
	}
}

void *apush(array *a, const void *x) {
	void *p;
	if(afull(a)) return NULL;
	p = aget(a,a->n++);
	*ashp(a) = a->n;
	return memcpy(p,x,tsize(a->t));
}

void *afind(array *a, void *x) {
	int i,s = tsize(a->t); array *y;
	for(i=0,y=aget(a,0);i<a->n;i++,y+=s)
		if(!memcmp(x,y,s)) return y;
	return NULL;
}

int aeach(array *a, int(*f)(void*,void*),void *p) {
	int i; for(i=0;i<a->n;i++)
		if(f(p,aget(a,i))) return -1;
	return 0;
}
