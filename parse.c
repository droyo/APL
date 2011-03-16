#include <stdlib.h>
#include <utf.h>
#include <fmt.h>
#include "apl.h"
#include "parse.h"

#define L (lparen|lbracket|assign|empty)
#define F (primitive|function)
#define N (symbol|string)
#define V (string|number)
#define D (doperator)
#define M (moperator)
#define R (~0)

rule cases[] = {
{{V|M|F|L,	V|F,	D,			V|F},	doper,	1,3},
{{V|M|F|L,	V,		F,			V},		dyad,	1,3},
{{V|M|F|L,	V|F,	M,			R},		moper,	1,2},
{{M|F|L,	F,		V,			R},		monad,	1,2},
{{D,		V,		F,			V},		monad,  2,3},
{{N,		assign,	V|M|D|F,	R},		set,	0,2},
{{lparen,	V|M|D|F,rparen,		R},		punc,	0,2},
{{0,0,0,0},NULL,0,0}
};

array empty_array = { empty, 0, 0, 0, NULL };

int parse(array *end) {
	if(!end) return -1;
	int i;
	array *e = end;
	stack s = mkstack(e);
	for(push(&s,e--); top(&s)->t!=empty;e--) {
		while(exec(&s));
		if (top(&s)->t == assign)
			push(&s, e);
		else push(&s, eval(e));
	}
	while(exec(&s));
	print("Result(%d): ",count(&s));
	for(i=0;i<count(&s);i++){print(i?",":"");disp(nth(&s,i));}
	print("\n");
	return 1;
}

array *eval(array *a) {
	return a;
}
int exec(stack *s) {
	int i, j, a, p;
	for(i = 0; i < NELEM(cases); i++) {
		for(j=0;j<NELEM(cases[0].c);j++) {
			a = nth(s,j)->t;
			p = cases[i].c[j];
			if(!(a&p)) break;
		}
		if(j==4) {
			print("Rule %d: ", i);
			apply(&cases[i], s);
			print("\n");
			break;
		}
	}
	return j == 4;
}
int apply(rule *r, stack *s) {
	int i;
	array x;
	array *a[4];
	
	print("(");
	for(i=0;i<4;i++)disp(nth(s,i));
	for(i = 0; i <= r->e; i++) {
		a[i] = pop(s);
	}print(")[%d,%d] ",r->b,r->e);
	x = (*r->f)(NULL, a, r->b, r->e);
	push(s, &x);
	for(i=r->b-1;i>=0;i--) push(s,a[i]);
	return 0;
}
array monad(void *env, array **a, int b, int e) {
	print("monad "); 
	disp(a[b]);print(" ");disp(a[e]);
	return *(a[e]);
}
array dyad(void *env, array **a, int b, int e)  {
	print("dyad ");
	disp(a[b+1]);print(" ");
	disp(a[b]);print(",");disp(a[e]);
	return *(a[e]);
}
array moper(void *env, array **a, int b, int e) {
	print("monad oper (");
	disp(a[b]);disp(a[e]);print(")");
	return *(a[b]);
}
array doper(void *env, array **a, int b, int e) {
	print("dyad oper (");
	disp(a[b]);disp(a[b+1]);disp(a[e]);print(")");
	return *(a[e]);
}
array set(void *env, array **a, int b, int e) {
	print("set "); disp(a[b]); 
	print (" := "); disp(a[e]);
	return *(a[e]);
}
array punc(void *env, array **a, int b, int e) {
	return *(a[b+1]);
}

static stack mkstack(array *beg) {
	stack s;
	s.bot = s.top = beg; return s;
}
static array *pop(stack *s) {
	return (s->top<=s->bot)?&empty_array:--s->top;
}
static array *nth(stack *s, int n) {
	return (s->top-(n+1)<s->bot)?&empty_array:s->top-(n+1);
}
static array *top(stack *s) { 
	return nth(s,0);
}
static void push(stack *s, array *a) {
	*s->top++ = *a;
}
static int count(stack *s) {
	return s->top - s->bot;
}
