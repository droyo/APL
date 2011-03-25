#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "apl.h"

static array *refs[2048];
static array *bot = NULL;
static array *toofar;
static array *toocls;

static array *c(array*,int);
static array *p(array*);
static void swap(array*,array*);
static void bubbledn(array*);
static void bubbleup(array*);

int mem_init(void) { 
	if(!(toofar = anew(empty,rdonly,0,0))) return -1;
	if(!(toocls = anew(empty,rdonly,0,0))) return -1;
	toocls->c = 0;
	toofar->c = UCHAR_MAX;
	memset(refs, 0, sizeof refs); 
	return 0;
}
void mem_free(void) {
	int i; for(i=0;refs[i];i++)
		free(refs[i]);
}
void mem_coll(void) {
	array *t;
	while((t=refs[0]) && !t->c) {
		t->c = UCHAR_MAX;
		bubbledn(t);
		refs[t->gc] = NULL;
		print("free(%A)\n",t);
		free(t);
	}
}
void record(array *a) {
	if(a->f&(tmpmem|rdonly)) return;
	int i;for(i=bot?bot->gc:0;refs[i];i++);
	a->gc = i; refs[i] = a;
	bot=a; bubbleup(a);
	a->f |= managed;
}
void incref(array *a) { 
	if(!(a->f&managed)) return;
	if(a->c+1>a->c) a->c++; bubbledn(a);
}
void decref(array *a) {
	if(a->f&(tmpmem|rdonly)) return;
	if(a->c) a->c--; bubbleup(a); 
}
static void bubbledn(array *a) {
	array *u; int n = a->c;
	while(n>(u=c(a,n>c(a,1)->c?1:2))->c)
		swap(a,u);
}
static void bubbleup(array *a) {
	while(a->c<p(a)->c) swap(a,p(a));
}
static array *p(array *a) {
	return a->gc?refs[(a->gc-1)/2]:toocls;
}
static array *c(array *a,int x) {
	int i = a->gc * 2+x;
	if(i > NELEM(refs) || !refs[i])
		return toofar;
	return refs[i];
}
static void swap(array *x, array *y) {
	long tmp;
	refs[x->gc]=y;refs[y->gc]=x;
	tmp=x->gc; x->gc=y->gc; y->gc=tmp;
}
