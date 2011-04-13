#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include <string.h>
#include "apl.h"

array *toocls = NULL;
array *toofar = NULL;

/* Heap operations */
static void   bubbleup(array*,array**);
static void   bubbledn(array*,array**);
static void   swap(array**,array**);
static array**c(array*,array**,char);
static array**p(array*,array**);

static array* mr(void *E) { return get(E,"⎕OBJECTS"); }

int  mem_init(void *E) {
	array *ref = abox(E,1024,FSYS,NULL);
	if(!toocls) toocls=anew(E,TNIL,FSYS,0,0);
	if(!toofar) toofar=anew(E,TNIL,FSYS,0,0);
	if(!(ref&&toocls&&toofar)) return -1;
	if(!put(E,"⎕OBJECTS",ref)) return -1;
	if(!put(E,"⎕TOOFAR",ref))  return -1;
	if(!put(E,"⎕TOOCLS",ref))  return -1;
	toofar->c = ~0;
	toocls->c =  0;
	return 0;
}

void mem_free(void *E) {
	array *ref = mr(E);
	int i; for(i=0;i<ref->n;i++)
		free(*(array**)aget(ref,i));
	free(ref);
}

void mem_coll(void *E) {
	array *ref = mr(E);
	array **top,**bot;
	while(ref->n && !(*(top=aval(ref)))->c) {
		swap(top,bot=aget(ref,ref->n-1));
		free(*bot); ref->n--;
		if(ref->n) bubbledn(ref,top);
	}
}

void record(void *E, array *a) {
	array *ref = mr(E);
	array **x = apush(ref,&a);
	bubbleup(ref,x);
	a->f |= FMAN;
}

void incref(void *E, array *a) {
	int i; array *ref = mr(E);
	if(!(a->f&FMAN))   return;
	if(a->c+1>a->c) a->c++;
	if(a->t&(TFUN|TBOX)) {
		for(i=0;i<ref->n;i++) 
			incref(E,*(array**)aget(ref,i));
	}
	bubbledn(ref,afind(ref,&a));
}

void decref(void *E, array *a) {
	array *ref = mr(E); int i;
	if(!(a->f&FMAN)) return;
	a->c-=a->c?1:0;
	if(a->t&(TFUN|TBOX)) {
		for(i=0;i<ref->n;i++) 
			decref(E,*(array**)aget(ref,i));
	}
	bubbleup(ref,afind(ref,&a));
}

static void bubbleup(array *ref, array **a) {
	array **u;
	while((*(u=p(ref,a)))->c > (*a)->c) {
		swap(u,a);
		a = u;
	}
}

static void bubbledn(array *ref, array **a) {
	array **l = c(ref,a,1);
	array **r = c(ref,a,2);
	while((*l)->c+(*r)->c < (*a)->c * 2) {
		if((*l)->c < (*a)->c) {
			swap(l,a); a=l;
		} else {
			swap(r,a); r=l;
		}
		l = c(ref,a,1);
		r = c(ref,a,2);
	}
}

static void swap(array **x, array **y) {
	array *tmp = *x; *x = *y; *y = tmp;
}

static array** p(array *ref, array **a) {
	array **r = aval(ref);
	long k = (((long)(a-r))-1)/2;
	return k<0?&toocls:r+k;
}

static array** c(array *ref, array **a, char w) {
	array **r = aval(ref);
	long k = ((long)(a-r))*2+w;
	return k >= ref->n ? &toofar : r+k;
}
