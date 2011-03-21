#include <string.h>
#include <stdlib.h>
#include "apl.h"
#define dirty(a) (a->f&(tmpmem|rdonly))

typedef struct _p{struct _p *nxt;char k[64];array *a;}pair;
typedef struct _e{struct _e *up;long sz,n;pair *p;}env;
static unsigned long hash(char *d); 
static pair *slot(void *v, char *key, pair **u); 

void *shadow(void *up, int hint) {
	env *e, *p=up; int i;
	if(!(e=malloc(sizeof *e))) goto err_env;
	e->up = p; e->n = 0;
	e->sz = (p ? p->n : 512) + hint*3;
	if(!(e->p=malloc(sizeof(pair)*e->sz))) goto err_dat;
	memset(e->p,0,sizeof(pair)*e->sz);
	for(i=0;i<e->sz;i++) e->p[i].k[0]=0;
	return e;
	err_dat: free(e);
	err_env: return NULL;
}
void *env_init(void) { return shadow(NULL, 0); }
void env_free(void *p) { env *e=p; free(e->p); free(e); }
static pair *slot(void *v, char *key, pair **u) {
	int a; pair *p; env *e=v; if(u) *u=NULL;
	p = e->p+hash(key)%e->sz;
	while((a=strncmp(p->k,key,sizeof p->k))&&p->nxt){ 
		if(u)*u=p; p=p->nxt; 
	}
	return p;
}
array *put(void *v, char *k, array *a) {
	pair *u, *p=slot((env*)v,k,&u);
	if(u&&!p&&!(p=u->nxt=malloc(sizeof *p))) goto err_pa;
	if(!strncmp(p->k,k,sizeof p->k)){
		if(p->a==a) return a; else decref(p->a);
	} else strncpy(p->k,k,sizeof p->k-1);
	if(dirty(a) && !(a=aclone(a))) goto err_ca;
	else incref(a);
	return p->a = a;
	err_ca: if(u) free(p);
	err_pa: return NULL;
}
array *get(void *v, char *key) {
	env *e = v; pair *p;
	do{ p = slot(e, key, NULL);
		if (!p && e->up) e = e->up;
		else if(p) return p->a;
	} while(e->up); return NULL;
}
static unsigned long hash(char *s) {
	unsigned long c, h = 5381;
	while((c=*s++))h=((h << 5)+h)^c;return h;
}
