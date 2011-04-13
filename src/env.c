#include <utf.h>
#include <string.h>
#include <stdlib.h>
#include "apl.h"

typedef struct   {char k[32]; array *a;}         pair;
typedef struct   {long n,max; pair *p;}          bucket;
typedef struct _e{struct _e *up; bucket b[768];} table;

static ulong hash(char *s,ulong max) {
	unsigned long c,h=3581;
	while((c=*s++))h=(((h<<5)+h)^c)%max; 
	return h;
}

static int addnew(bucket *b, pair *p) {
	pair *r;
	if(b->n >= b->max) {
		if(!(r=realloc(b->p, b->max+sizeof *p)))
			return -1;
		else b->max++;
	}
	b->p[b->n++] = *p;
	return 0;
}

static pair *find(bucket *b, char *k) {
	int i; for(i=0;i<b->n;i++)
		if(!strncmp(b->p[i].k,k,sizeof b->p[i].k))
			return b->p+i;
	return NULL;
}

static int add(bucket *b, pair *p) {
	int i;
	for(i=0;i<b->n;i++) {
		if(!strncmp(b->p[i].k,p->k, sizeof p->k)) {
			b->p[i] = *p;
			return 0;
		}
	}
	return addnew(b,p);
}

void *env(void *up) {
	int i; table *e;
	if(!(e=malloc(sizeof *e))) return NULL;
	e->up = (table*)up; 
	for(i=0;i<NELEM(e->b);i++) {
		if(!(e->b[i].p = malloc(sizeof (pair))))
			goto spill;
		memset(e->b[i].p->k, 0, sizeof e->b[i].p->k);
		e->b[i].max = 1;
		e->b[i].n = 0;
	}
	return e;
	spill: while(--i>=0) free(e->b[i].p); free(e);
	return NULL;
}

void env_free(void *p) {
	int i; table *e = p;
	for(i=0;i<NELEM(e->b);i++)
		free(e->b[i].p);
	free(e);
}

static bucket *slot(table *e, char *k) {
	return e->b+1+hash(k,NELEM(e->b)-1);
}

array *put(void *e, char *k, array *a) {
	bucket *b = slot(e,k);
	pair p,*old = find(b,k);

	strncpy(p.k, k, sizeof p.k-1);
	if(old) {
		if(old->a->f&(FRDO|FSYS)) return NULL;
		else if(old->a == a)      return a;
		else decref(e,old->a);
	} 
	if (a->f&FTMP && !(a=acln(e,0,a))) 
		return NULL;
	else p.a = a;
	if(add(b,&p)) return NULL;
	if(!(a->f&FSYS))incref(e,p.a);
	return p.a;
}

array *get(void *v,char *k) {
	table *e = v; pair *p;
	do{ p = find(slot(e,k),k);
		if(!p && e->up) e=e->up;
		else if(p) return p->a;
	} while(e->up); return NULL;
}
