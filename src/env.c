#include <utf.h>
#include <string.h>
#include <stdlib.h>
#include "apl.h"

static unsigned long hash(char *s) {
	unsigned long c,h=3581;
	while((c=*s++))h=((h<<5)+h)^c; return h;
}

static int addnew(array **b, pair p) {
	if((*b)->n >= (*b)->z && !agrow(b,1))
		return -1;
	return apush(*b, &p) ? 0: -1;
}

static pair *find(array *b, char *k) {
	pair *v = aval(b);
	int i; for(i=0;i<b->n;i++)
		if(!strncmp(v[i].k,k,sizeof v[i].k)) return v+i;
	return NULL;
}

static int add(array **b, pair p) {
	int i; 
	pair *n = aval(*b);

	for(i=0;i<(*b)->n;i++) {
		if(!strncmp(n[i].k,p.k, sizeof p.k)) {
			n[i] = p;
			return 0;
		}
	}
	return addnew(b,p);
}

array *env(array *up) {
	array *e,**b;
	if(!(e=abox(NULL,768,FSYS,NULL)))
		return NULL;
	b=aval(e); b[0] = up;
	for(e->n=1;e->n<e->z;e->n++) {
		if(!(b[e->n]=anew(NULL,TREL,FSYS,1,1)))
			return NULL;
		b[e->n]->n = 0;
		((pair*)aval(b[e->n]))->k[0]=0;
	}
	return e;
}
int free_bucket(void *item, void *args) {
	free(*(array**)item);
	return 0;
}
void env_free(array *e) {
	aeach(e,free_bucket,NULL);
	free(e);
}
static array **slot(array *e, char *k) {
	return aget(e,1+(hash(k)%(e->n-1)));
}

array *put(array *e, char *k, array *a) {
	array   **b = slot(e,k);
	pair p,*old = find(*b,k);

	strncpy(p.k, k, sizeof p.k-1);
	if(old) {
		if(old->a->f&(FRDO|FSYS)) return NULL;
		else if(old->a == a) return a;
		else decref(e,old->a);
	} 
	if (a->f&FTMP && !(a = acln(e,a))) 
		return NULL;
	else p.a = a;
	if(add(b,p))
		return NULL;
	if(!(a->f&FSYS)) incref(e,p.a);
	return p.a;
}

array *get(array *e,char *k) {
	pair *p; array **a;
	do{ p = find(*slot(e,k),k); a=aval(e);
		if(!p && *a) e=*a;
		else if(p) return p->a;
	} while(*a); return NULL;
}
