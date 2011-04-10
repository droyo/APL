#include <utf.h>
#include <string.h>
#include <stdlib.h>
#include "apl.h"

typedef struct {char k[64];array *a;} pair;
static int dirty(array *a) { return a->f&(FTMP|FRDO); }
static unsigned long hash(char *s) {
	unsigned long c,h=3581;
	while((c=*s++))h=((h<<5)+h)^c; return h;
}

static int addnew(array *e, int x, pair p) {
	array **v=aval(e),*b=v[x]; pair *n;
	if(b->n >= b->z/sizeof p && !agrow(&b,sizeof p))
		return -1;
	v[x]=b; n=aval(b); n[b->n++]=p;
	return 0;
}

static pair *find(array *b, char *k) {
	pair *v = aval(b);
	int i; for(i=0;i<b->n;i++)
		if(!strncmp(v[i].k,k,sizeof v[i].k)) return v+i;
	return NULL;
}

static int add(array *e, long x, pair p) {
	int i; 
	array **v=aval(e),*b=v[x];
	pair *n = aval(b);

	for(i=0;i<b->n;i++) {
		if(!strncmp(n[i].k,p.k, sizeof p.k)) {
			n[i] = p;
			return 0;
		}
	}
	return addnew(e,x,p);
}

array *shadow(array *up) {
	array *e,**b; pair *p;
	if(!(e=abox(768,NULL))) return NULL;
	b=aval(e); b[0] = up;
	for(e->n=1;e->n<e->z;e->n++) {
		if(!(b[e->n]=anew(TRAW,0,1,sizeof(pair))))
			return NULL;
		b[e->n]->n = 0;
		p=aval(b[e->n]);p->k[0] = '\0';
	}
	return e;
}

static array *slot(array *e, char *k) {
	array **b = aval(e);
	return b[hash(k)%e->n];
}

array *put(array *e, char *k, array *a) {
	array *b = slot(e,k);
	pair p,*old = find(b,k);

	strncpy(p.k, k, sizeof p.k-1);
	if(old) {
		if(old->a->f&FRDO) return NULL;
		else if(old->a == a) return a;
		else decref(old->a);
	} 
	if (dirty(a) && !(a = acln(a))) 
		return NULL;
	else p.a = a;
	if(add(e,hash(k)%e->n,p))
		return NULL;
	incref(p.a);
	return p.a;
}

array *get(array *e,char *k) {
	pair *p; array **a;
	do{ p = find(slot(e,k),k); a=aval(e);
		if(!p && *a) e=*a;
		else if(p) return p->a;
	} while(*a); return NULL;
}
