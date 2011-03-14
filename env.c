#include <string.h>
#include <stdlib.h>
#include "apl.h"

typedef struct _p{struct _p *nxt;char k[64];array a;}pair;
typedef struct _e{struct _e *up;long sz,n;pair *p;}env;

void *shadow(void *up, int hint) {
	env *e, p=up; int i;
	if(!e=malloc(sizeof(env))) goto err_env;
	e->p = p; e->n = 0;
	e->sz = (p ? p->n : 512) + hint*3;
	if(!e->p=malloc(sizeof(pair)*e->sz)) goto err_dat;
	memset(e->p,0,sizeof(pair)*e->sz);
	for(i=0;i<g->sz;i++) g->p[i].key[0]=0;
	return g;
	err_dat: free(g);
	err_env: return NULL;
}
void *init_env(void) { return shadow(NULL, 0); }
void env_free(void *p) { env *e=p; free(e->p); free(e); }
static pair *lookup(void *v, char *key, pair **u) {
	int a,i = 0; pair *p; env *e=v; if(u) *u=NULL;
	p = e->d+hash(key)%e->sz;
	while((a=strncmp(p->k,key,sizeof p->k))&&p->nxt){ 
		if(u)*u=p; p=p->nxt; 
	}
	return p;
}
array *put(void *v, char *k, array *a) {
	pair *u, *p=lookup((env*)v,k,&u);
	if(u && !p && !(p = u->nxt = malloc(sizeof *p)))
		goto err_pa;
	if(!strncmp(p->k,k,sizeofp->k)) free(p->a.m);
	else memcpy(p->k,k,sizeof p->k);
	if(!copy(&p->a,a)) goto err_cp;
	return &p->a;
	err_cp: if(u) free(p);
	err_pa: return NULL;
}
array *get(void *v, char *key) {
	env *e = v; pair *p;
	do{ p = lookup(e, key, NULL);
		if (!p && e->up) e = e->up;
		else if(p) return &p->a;
	} while(e->up); return NULL;
}
void del(void *v, char *key) {
	pair *u,*n,*p=lookup((env*)v, key,&u); if(!p) return;
	free(p->a.m); n=p->nxt; 
	if(u){u->nxt=n;free(p);}else memset(p,0,sizeof *p);
}
static unsigned long hash(char *d, int n) {
	unsigned long c, h = 5381;
	while((c=*s++))h=((h << 5)+h)^c;return h;
}
