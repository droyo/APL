#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include "apl.h"

struct stack {
	array *top;
	array *bot;
};

array *eval(array *);
int exec(struct stack *);
void disp(array*);
static array *pop(struct stack *);
static array *top(struct stack *);
static void push(struct stack *, array*);
static int count(struct stack *);
static struct stack mkstack(array *);

int parse(int end) {
	if(!end) return 0;
	if(end < 0) return -1;
	int e = end;
	struct stack s = mkstack(tok+e);
	
	for(;e>=0;e--) {
		if (top(&s)->t == assign)
			push(&s, tok+e);
		else push(&s, eval(tok+e));
		if (count(&s) == 4) 
			exec(&s);
	}
	exec (&s);
	return 1;
}

array *eval(array *a) {
	const int literal = number+string+diamond;
	if(a->t & literal) return a;
	else return a;
}

int exec(struct stack *s) {
	int i;
	array *a;
	print("exec(");
	for(i = 0; i < 4; i++) {
		a = pop(s);
		print(i?", ":"");
		if(a) disp(a); 
	}
	print(")\n");
	return 0;
}

void disp(array *a) {
	int i;
	Rune *r;
	double *d;
	switch(a->t) {
	case diamond:
		print("♦");
		break;
	case number: 
		print("num ");
		d = a->m;
		for(i = 0; i < a->n; i++)
			print("%s%g",i?" ":"", d[i]);
		break;
	case string:
		print("str %s", (char*)a->m);
		break;
	case subcmd:
		print("cmd %s", (char*)a->m);
		break;
	case symbol:
		print("sym %s", (char*)a->m);
		break;
	case assign:
		r = a->m;
		print("%C", *r);
		break;
	case function:
		r = a->m;
		print("fn %C", *r);
		break;
	default:
		r = a->m;
		print("del %C", *r);
		break;
	}
}

static struct stack mkstack(array *beg) {
	struct stack s;
	s.bot = s.top = beg; return s;
}
static array *pop(struct stack *s) {
	return (s->top<=s->bot)?0:--s->top;
}
static array *top(struct stack *s) { 
	return s->top; 
}
static void push(struct stack *s, array *a) {
	*s->top++ = *a;
}
static int count(struct stack *s) {
	return s->top - s->bot;
}
