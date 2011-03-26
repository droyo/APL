#include <utf.h>
#include <string.h>
#include <stdlib.h>
#include "apl.h"
#define dirty(a) (a->f&(tmpmem|rdonly))

typedef struct   {char k[64]; array *a;}              pair;
typedef struct   {long n,max; pair *p;}               bucket;
typedef struct _e{struct _e *up; bucket b[768];} env;

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
		if(!strncmp(b->p[i].k,k,64)) return b->p+i;
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

void *shadow(void *up) {
	int i; env *e;
	if(!(e=malloc(sizeof *e))) return NULL;
	e->up = (env*)up; 
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
	int i; env *e = p;
	for(i=0;i<NELEM(e->b);i++)
		free(e->b[i].p);
	free(e);
}

static bucket *slot(env *e, char *k) {
	return e->b+hash(k)%NELEM(e->b);
}

array *put(void *v, char *k, array *a) {
	bucket *b = slot((env*)v, k);
	pair p,*old = find(b,k);

	strncpy(p.k, k, sizeof p.k-1);
	if(old) {
		if(old->a->f&rdonly) return NULL;
		else if(old->a == a) return a;
		else decref(old->a);
	} 
	if (dirty(a) && !(a = aclone(a))) 
		return NULL;
	else p.a = a;
	if(add(b,p)) return NULL;
	incref(p.a);
	return p.a;
}

array *get(void *v,char *k) {
	env *e = v; pair *p;
	do{ p = find(slot(e,k),k);
		if(!p && e->up) e=e->up;
		else if(p) return p->a;
	} while(e->up); return NULL;
}
