#include <utf.h>
#include <string.h>
#include <stdlib.h>
#include "apl.h"

typedef struct   {char k[64]; array *a;}         pair;
typedef struct   {long n,max; pair *p;}          bucket;
typedef struct _e{struct _e *up; bucket b[768];} tbl;

static unsigned long hash(char *s) {
	unsigned long c,h=3581;
	while((c=*s++))h=((h<<5)+h)^c; return h;
}

static int addnew(bucket *b, pair p) {
	pair *r;
	if(b->n >= b->max) {
		if(!(r=realloc(b->p, b->max+sizeof p)))
			return -1;
		else b->max++;
	}
	b->p[b->n++] = p;
	return 0;
}

static pair *find(bucket *b, char *k) {
	int i; for(i=0;i<b->n;i++)
		if(!strncmp(b->p[i].k,k,sizeof b->p[i].k))
			return b->p+i;
	return NULL;
}

static int add(bucket *b, pair p) {
	int i;
	for(i=0;i<b->n;i++) {
		if(!strncmp(b->p[i].k,p.k, sizeof p.k)) {
			b->p[i] = p;
			return 0;
		}
	}
	return addnew(b,p);
}

void *env(void *up) {
	int i; tbl *e;
	if(!(e=malloc(sizeof *e))) return NULL;
	e->up = (tbl*)up; 
	for(i=0;i<NELEM(e->b);i++) {
		if(!(e->b[i].p = malloc(sizeof (pair))))
			goto spill;
		e->b[i].p->k[0] = 0;
		e->b[i].max = 1;
		e->b[i].n = 0;
	}
	return e;
	spill: while(--i>=0) free(e->b[i].p); free(e);
	return NULL;
}

void env_free(void *p) {
	int i; tbl *e = p;
	for(i=0;i<NELEM(e->b);i++)
		free(e->b[i].p);
	free(e);
}

static bucket *slot(tbl *e, char *k) {
	return e->b+hash(k)%NELEM(e->b);
}

array *put(void *v, char *k, array *a) {
	bucket *b = slot((tbl*)v, k);
	pair p,*old = find(b,k);

	strncpy(p.k, k, sizeof p.k-1);
	if(old) {
		if(old->a->f&(FRDO|FSYS)) return NULL;
		else if(old->a == a)      return a;
		else decref(v,old->a);
	} 
	if (a->f&FTMP && !(a=acln(v,0,a))) 
		return NULL;
	else p.a = a;
	if(add(b,p)) return NULL;
	if(!(a->f&FSYS))incref(v,p.a);
	return p.a;
}

array *get(void *v,char *k) {
	tbl *e = v; pair *p;
	do{ p = find(slot(e,k),k);
		if(!p && e->up) e=e->up;
		else if(p) return p->a;
	} while(e->up); return NULL;
}
