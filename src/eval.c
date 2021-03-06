#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include <string.h>
#include "apl.h"
#include "eval.h"
#include "error.h"

#define L (TLPR|TSET|TEND)
#define F (TFUN)
#define N (TSYM|TSTR)
#define V (TSTR|TNUM|TBOX|TNIL)
#define D (TDYA)
#define M (TMON)
#define R (~0)

rule cases[] = {
{{	V|M|F|L, V|F,     D,       V|F}, doper, 1,3},
{{	V|M|F|L, V,       F,       V},   dyad,  1,3},
{{	V|M|F|L, V|F,     M,       R},   moper, 1,2},
{{	M|F|L,   F,       V,       R},   monad, 1,2},
{{	D,       V,       F,       V},   monad, 2,3},
{{	N,       TSET,    V|M|D|F, R},   bind,  0,2},
{{	TLPR,    V|M|D|F, TRPR,    R},   punc,  0,2},
{{	0,       0,       0,       0},   NULL,  0,0}
};

array *eval(array *E, array *tok) {
	array **a = aval(tok);
	stack l = mkstack(a,+1); l.top = a+tok->n-1;
	stack r = mkstack(a+tok->n-1,-1);
	return parse(E, &l, &r, 0);
}
array *parse(array *E, stack *l, stack *r, int lvl) {
	stack n;
	array *a, *e, *v;
	do {
		a = pop(l);
		if(lvl < 0) {
			if(a->t == TRDF) {
				n=mkstack(r->top-1,-1);
				if(!(e=parse(E,l,&n,lvl-1)))
					return NULL;
				else push(r,e);
			}else if(a->t == TLDF) return mkfun(E,r);
			else push(r,a);
			continue;
		}else if(a->t == TRDF) {
			n=mkstack(r->top-1,-1);
			if(!(e = parse(E,l,&n,-1)))
				return NULL;
			else push(r,e);
		}else if(a->t == TRPR) {
			n=mkstack(r->top-1,-1);
			push(&n,a);
			if(!(e = parse(E,l,&n,lvl+1)))
				return NULL; 
			else push(r,e);
		}else if(a->t == TLPR) {
			if (!lvl) return enil(Eparen);
			push(r,a); break;
		}else if (top(r)->t == TSET)
			push(r,a);
		else {
			if(!(v = lookup(E,a)))
				return enil(Eunbound, a);
			else push(r,v);
		}
		while(exec(E,r));
	} while(a->t != TEND);
	while(exec(E,r));
	if (lvl < 0)    return enil(Esyntax);
	if (count(r)>2) return enil(Esyntax);
	return nth(r,lvl?0:1);
}

array *mkfun(array *E, stack *s) {
	array *a;
	if(!count(s)) return zilde;
	if(!(a=abox(count(s),s->top)))
		return enil(Enomem);
	a->t = TFUN;
	return a;
}

array *lookup(array *E,array *a) {
	char k[64];
	if(a->t == TSYM) {
		return get(E,akey(a,k,sizeof k));
	}
	return a;
}
int exec(array *E, stack *s) {
	int i, j, a, p;
	for(i = 0; i <NELEM(cases); i++) {
		for(j=0;j<NELEM(cases[0].c);j++) {
			a = nth(s,j)->t;
			p = cases[i].c[j];
			if(!(a&p)) break;
		}
		if(j==4) {
			apply(E, &cases[i], s);
			print("\n");
			break;
		}
	}
	return j == 4;
}
int apply(array *E, rule *r, stack *s) {
	int i; array *x, *a[4];
	for(i=0;i<=r->e;i++) a[i] = pop(s);
	x = (*r->f)(E, a, r->b, r->e);
	if(!x) return -1;
	push(s,x);for(i=r->b-1;i>=0;i--) push(s,a[i]);
	return 0;
}
array* monad(array *E, array **a, int b, int e) {
	print("(%A %A)", a[b], a[e]);
	return a[e];
}
array* dyad(array *E, array **a, int b, int e)  {
	print("(%A %A,%A)",a[b+1],a[e],a[b]);
	return a[e];
}
array* moper(array *E, array **a, int b, int e) {
	print("(op '%A%A')",a[b],a[e]);
	return a[b];
}
array* doper(array *E, array **a, int b, int e) {
	print("(op %A%A%A)", a[b+1], a[b], a[e]);
	return a[e];
}
array* bind(array *E, array **a, int b, int e) {
	char k[64];
	print("(set %A %A)",a[b], a[e]);
	array *s = put(E, akey(a[b],k,sizeof k), a[e]);
	if(!s) return ezil(Ebind, a[b]);
	s->f |= FSIL;
	return s;
}
array* punc(array *E, array **a, int b, int e) {
	return a[b+1];
}
static stack mkstack(array **beg, char d) {
	stack s; s.dir = d;
	s.bot = beg; s.top = beg - d; 
	return s;
}
static array *pop(stack *s) {
	if (!count(s)) return marker;
	s->top -= s->dir;
	return s->top[s->dir];
}
static array *nth(stack *s, int n) {
	if (count(s) <= n) return marker;
	return *(s->top - s->dir*n);
}
static array *top(stack *s) { 
	return nth(s,0);
}
static void push(stack *s, array *a) {
	*(s->top+=s->dir) = a;
}
static int count(stack *s) {
	array **hi = s->dir>0?s->top:s->bot;
	array **lo = s->dir>0?s->bot:s->top;
	return hi - lo + 1;
}
