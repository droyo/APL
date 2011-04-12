#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "apl.h"

static array *toofar;
static array *toocls;

static array *c(array*,array*,int);
static array *p(array*,array*);
static void swap(array**,array*,array*);
static void bubbledn(array*,array*);
static void bubbleup(array*,array*);

int mem_init(array *E) { 
	array *ref=NULL;
	if(!(ref=anew(E,TBOX,FSYS,0,1024))) return -1;
	if(!(toofar=anew(E,TNIL,FSYS,0,0))) return -1;
	if(!(toocls=anew(E,TNIL,FSYS,0,0))) return -1;
	toofar->c = UCHAR_MAX;
	toocls->c = 0;
	if(!put(E,"⎕REF",ref)) return -1;
	return ref->n = 0;
}

void mem_free(array *E) {
	array *ref = get(E,"⎕REF");
	int i; for(i=0;i<ref->n;i++)
		free(*(array**)aget(ref,i));
	free(toofar);
	free(toocls);
}

void mem_coll(array *E) {
	array *t=NULL, *ref = get(E,"⎕REF");
	while(ref->n && !(t=*(array**)aget(ref,0))->c){
		t->c = UCHAR_MAX;
		bubbledn(ref,t);
		ref->n--; free(t);
	}
}

void record(array *E, array *a) {
	array *ref = get(E,"⎕REF");
	a->gc = ref->n;
	if(!apush(ref, &a)) return;
	bubbleup(ref,a);
	a->f |= FMAN;
}

void incref(array *E, array *a) { 
	int i;
	array *ref = get(E,"⎕REF");
	if(a->c+1>a->c)a->c++;
	bubbledn(ref,a);
	if(a->t==TBOX||a->t==TFUN) {
		for(i=0;i<a->n;i++)
			incref(E,*(array**)aget(a,i));
	}
}

void decref(array *E, array *a) {
	int i;
	array *ref = get(E,"⎕REF");
	if(a->c) a->c--;
	bubbleup(ref,a);
	if(a->t==TBOX||a->t==TFUN) {
		for(i=0;i<a->n;i++)
			decref(E,*(array**)aget(a,i));
	}
}
static void bubbledn(array *r, array *a) {
	array *u=NULL; int n = a->c;
	while(n>(u=c(r,a,n>c(r,a,1)->c?1:2))->c)
		swap(aval(r),a,u);
}
static void bubbleup(array *r, array *a) {
	while(a->c<p(r,a)->c) swap(aval(r),a,p(r,a));
}
static array *p(array *r, array *a) {
	array **x = aval(r);
	return a->gc?x[(a->gc-1)/2]:toocls;
}
static array *c(array *r,array *a,int x) {
	int i = a->gc * 2+x;
	if(i>r->n) return toofar;
	else       return aget(r,i);
}
static void swap(array **r, array *x, array *y) {
	long tmp; 
	r[x->gc]=y;r[y->gc]=x;
	tmp=x->gc; x->gc=y->gc; y->gc=tmp;
}
