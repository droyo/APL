#include <stdlib.h>
#include <utf.h>
#include <fmt.h>
#include "apl.h"
#include "parse.h"

#define L (lparen|assign|empty)
#define F (function|primitive)
#define N (symbol|string)
#define V (string|number)
#define D (doperator)
#define M (moperator)
#define R (~0)

rule cases[] = {
{{V|M|F|L, V|F,     D,       V|F},doper, 1,3},
{{V|M|F|L, V,       F,       V},  dyad,  1,3},
{{V|M|F|L, V|F,     M,       R},  moper, 1,2},
{{M|F|L,   F,       V,       R},  monad, 1,2},
{{D,       V,       F,       V},  monad, 2,3},
{{N,       assign,  V|M|D|F, R},  set,   0,2},
{{lparen,  V|M|D|F, rparen,  R},  punc,  0,2},
{{0,0,0,0},NULL,0,0}
};

void showdbg(stack *l, stack *r) {
	array *a;
	print("[");
	for(a=l->bot;a <= r->bot;a++) {
		if(a > l->top && a < r->top) {
			print(" ");
			continue;
		}
		if(a == r->top) print("♦");
		disp(a);
	} print("]\n");
}
void top4(stack *s) {
	int i; for (i=0;i<4;i++) {
		disp(nth(s,i));
		print(",");
	}print("\n");
}

array *parse(void *E, array **t) {
	if (!t) return NULL;
	stack l = mkstack(t[0],+1); l.top = t[1];
	stack r = mkstack(t[1],-1);
	return process(E, &l, &r, 0);
}
array *process(void *E, stack *l, stack *r, int lvl) {
	stack n;
	array *a, *e;
	do {
		a = pop(l);
		if(a->t == rparen) {
			n = mkstack(a,-1); push(&n,a);
			if(!(e = process(E,l,&n,lvl+1)))
				return NULL; 
			else push(r,e);
		}else if(a->t == lparen) {
			if (!lvl) return NULL;
			push(r,a); break;
		}else if (top(r)->t == assign)
			push(r,a);
		else push(r, eval(E,a));
		showdbg(l,r);while(exec(E, r));
	} while(a->t != empty);
	while(exec(E, r));
	if (count(r) > 2) {
		print("Parsing error\n");
		return NULL;
	}
	return nth(r,lvl?0:1);
}

array *eval(void *E,array *a) {
	array *r;
	switch(a->t) {
	case symbol: 
		return (r=get(E,aval(a)))?r:&zilde;
	default: return a;
	}
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
	int i;
	array x;
	array *a[4];
	
	print("|");
	for(i=0;i<4;i++)disp(nth(s,i));
	for(i = 0; i <= r->e; i++) {
		a[i] = pop(s);
	}print("| ",r->b,r->e);
	x = (*r->f)(E, a, r->b, r->e);
	push(s, &x);
	for(i=r->b-1;i>=0;i--) push(s,a[i]);
	return 0;
}
array monad(void *E, array **a, int b, int e) {
	print("monad "); 
	disp(a[b]);print(" ");disp(a[e]);
	return *(a[e]);
}
array dyad(void *E, array **a, int b, int e)  {
	print("dyad ");
	disp(a[b+1]);print(" ");
	disp(a[b]);print(",");disp(a[e]);
	return *(a[e]);
}
array moper(void *E, array **a, int b, int e) {
	print("monad oper (");
	disp(a[b]);disp(a[e]);print(")");
	return *(a[b]);
}
array doper(void *E, array **a, int b, int e) {
	print("dyad oper (");
	disp(a[b]);disp(a[b+1]);disp(a[e]);print(")");
	return *(a[e]);
}
array set(void *E, array **a, int b, int e) {
	print("set "); disp(a[b]); 
	print(" := "); disp(a[e]);
	array *s = put(E, aval(a[b]), a[e]);
	if(!s) {
		fprint(2,"Could not store ");
		disp(a[e]);print(" in ");
		disp(a[b]);print("\n");
		return zilde;
	}
	s->c++;
	return *s;
}
array punc(void *E, array **a, int b, int e) {
	print("punc ");
	return *(a[b+1]);
}

static stack mkstack(array *beg, char d) {
	stack s; s.dir = d;
	s.bot = beg; s.top = beg - d; 
	return s;
}
static array *pop(stack *s) {
	array *a = s->top;
	if (!count(s)) return &zilde;
	s->top -= s->dir;
	return a;
}
static array *nth(stack *s, int n) {
	if (count(s) <= n) return &zilde;
	return s->top-(s->dir*n);
}
static array *top(stack *s) { 
	return nth(s,0);
}
static void push(stack *s, array *a) {
	*(s->top+=s->dir) = *a;
}
static int count(stack *s) {
	array *hi = s->dir>0?s->top:s->bot;
	array *lo = s->dir>0?s->bot:s->top;
	return hi - lo + 1;
}
