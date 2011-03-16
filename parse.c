#include <stdlib.h>
#include <utf.h>
#include <fmt.h>
#include "apl.h"
#include "parse.h"

#define FUNC (primitive + function)
#define NAME (symbol + string)
#define OPER (operator)
#define ATOM (string + number)
#define EDGE (lparen + lbracket + assign + empty)
#define FNOA (FUNC + NAME + OPER + ATOM)
#define FOA (FUNC + OPER + ATOM)
#define ANY (~0)

rule cases[] = {
{{EDGE,			FUNC,		ATOM,		ANY},	monad,	1,2},
{{EDGE+FOA,		FUNC,		FUNC,		ATOM},	monad,	2,3},
{{EDGE+FOA,		ATOM,		FUNC,		ATOM},	dyad,	1,3},
{{EDGE+FOA,		ATOM+FUNC,	OPER,		ANY},	oper,	1,2},
{{NAME,			assign,		FNOA,		ANY},	set,	0,2},
{{lparen,		FNOA,		rparen,		ANY},	punc,	0,2},
{{0,0,0,0},NULL,0,0}};

array empty_array = { empty, 0, 0, 0, NULL };

int parse(array *end) {
	if(!end) return -1;
	array *e = end;
	stack s = mkstack(e);
	do {
		if (top(&s)->t == assign)
			push(&s, e);
		else push(&s, eval(e));
		exec(&s);
	} while((e->t != empty&&e--) || exec(&s));
	return 1;
}

array *eval(array *a) {
	return a;
}
int exec(stack *s) {
	int i, j, a, p;
	for(i = 0; i < NELEM(cases); i++) {
		for(j=0;j<NELEM(cases[0].c);j++) {
			p = cases[i].c[j];
			a = nth(s,j)->t;
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
	for(i = 0; i <= r->e; i++) {
		a[i] = pop(s);
		disp(a[i]);
	}print(") ");
	x = (*r->f)(NULL, a[r->b], a[r->e]);
	push(s, &x);
	for(i=r->b-1;i>=0;i--) push(s,a[i]);
	return 0;
}
array monad(void *env, array *b, array *e) {
	print("monad "); 
	disp(b);print(" ");disp(e);
	return *e;
}
array dyad(void *env, array *b, array *e) {
	print("dyad ");
	disp(b+1);print(" ");
	disp(b);print(",");disp(e);
	return *e;
}
array oper(void *env, array *b, array *e) {
	print("oper ");
	return empty_array;
}
array set(void *env, array *b, array *e) {
	print("set ");
	return empty_array;
}
array punc(void *env, array *b, array *e) {
	return empty_array;
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
