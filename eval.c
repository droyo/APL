#include <stdlib.h>
#include <utf.h>
#include <fmt.h>
#include "apl.h"
#include "eval.h"

#define L (lparen|assign|empty)
#define F (function|primitive)
#define N (symbol|string)
#define V (string|number)
#define D (dydop)
#define M (monop)
#define R (~0)

rule cases[] = {
	V|M|F|L, V|F,     D,       V|F, doper, 1,3,
	V|M|F|L, V,       F,       V,   dyad,  1,3,
	V|M|F|L, V|F,     M,       R,   moper, 1,2,
	M|F|L,   F,       V,       R,   monad, 1,2,
	D,       V,       F,       V,   monad, 2,3,
	N,       assign,  V|M|D|F, R,   bind,  0,2,
	lparen,  V|M|D|F, rparen,  R,   punc,  0,2,
	0,       0,       0,       0,   NULL,  0,0
};

array *eval(void *E, array ***t) {
	if (!t) return NULL;
	stack l = mkstack(t[0],+1); l.top = t[1];
	stack r = mkstack(t[1],-1);
	return parse(E, &l, &r, 0);
}
array *parse(void *E, stack *l, stack *r, int lvl) {
	stack n;
	array *a, *e, *v;
	do {
		a = pop(l);
		if(a->t == rparen) {
			n=mkstack(r->top-1,-1);
			push(&n,a);
			if(!(e = parse(E,l,&n,lvl+1)))
				return NULL; 
			else push(r,e);
		}else if(a->t == lparen) {
			if (!lvl) return NULL;
			push(r,a); break;
		}else if (top(r)->t == assign)
			push(r,a);
		else {
			if(!(v = lookup(E,a))) {
				fprint(2, "Unbound var `%A'\n", a);
				return NULL;
			} else push(r,v);
		}
		while(exec(E, r));
	} while(a->t != empty);
	while(exec(E, r));
	if (count(r) > 2) {
		print("Parsing error\n");
		return NULL;
	}
	return nth(r,lvl?0:1);
}

array *lookup(void *E,array *a) {
	char k[64];
	if(a->t == symbol) {
		snprint(k, sizeof k, "%*R",a->n,aval(a));
		return get(E,k);
	}
	return a;
}
int exec(void *E, stack *s) {
	int i, j, a, p;
	for(i = 0; i < NELEM(cases); i++) {
		for(j=0;j<NELEM(cases[0].c);j++) {
			a = nth(s,j)->t;
			p = cases[i].c[j];
			if(!(a&p)) break;
		}
		if(j==4) {
			print("\t");
			apply(E, &cases[i], s);
			print("\n");
			break;
		}
	}
	return j == 4;
}
int apply(void *E, rule *r, stack *s) {
	int i; array *x, *a[4];
	
	for(i=0;i<=r->e;i++) a[i] = pop(s);
	x = (*r->f)(E, a, r->b, r->e);
	push(s,x);for(i=r->b-1;i>=0;i--) push(s,a[i]);
	return 0;
}
array* monad(void *E, array **a, int b, int e) {
	print("(%A %A)", a[b], a[e]);
	return a[e];
}
array* dyad(void *E, array **a, int b, int e)  {
	print("(%A %A,%A)",a[b+1],a[b],a[e]);
	return a[e];
}
array* moper(void *E, array **a, int b, int e) {
	return a[b];
}
array* doper(void *E, array **a, int b, int e) {
	print("(op %A%A%A)", a[b+1], a[b], a[e]);
	return a[e];
}
array* bind(void *E, array **a, int b, int e) {
	char k[64];
	print("(set %A %A)",a[b], a[e]);
	snprint(k,sizeof k,"%*R", a[b]->n, aval(a[b]));
	array *s = put(E, k, a[e]);
	if(!s) {
		fprint(2,"Binding error\n");
		return zilde;
	} 
	return s;
}
array* punc(void *E, array **a, int b, int e) {
	return a[b+1];
}
static stack mkstack(array **beg, char d) {
	stack s; s.dir = d;
	s.bot = beg; s.top = beg - d; 
	return s;
}
static array *pop(stack *s) {
	if (!count(s)) return zilde;
	s->top -= s->dir;
	return s->top[s->dir];
}
static array *nth(stack *s, int n) {
	if (count(s) <= n) return zilde;
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
