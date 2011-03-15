#include <utf.h>
#include <fmt.h>
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

int parse(array *end) {
	if(!end) return -1;
	array *e = end;
	struct stack s = mkstack(e);
	
	for(;e->t != marker;e--) {
		if (top(&s)->t == assign)
			push(&s, e);
		else push(&s, eval(e));
		if (count(&s) == 4) 
			exec(&s);
	}
	return 1;
}

array *eval(array *a) {
	return a;
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
	case number: 
		print("num ");
		d = val(a);
		for(i = 0; i < a->n; i++)
			print("%s%g",i?" ":"", d[i]);
		break;
	case string:
		print("str %s", (char*)val(a));
		break;
	case subcmd:
		print("cmd %s", (char*)val(a));
		break;
	case symbol:
		print("sym %s", (char*)val(a));
		break;
	default:
		print("%s", (char*)val(a));
	}
}

static struct stack mkstack(array *beg) {
	struct stack s;
	s.bot = s.top = beg; return s;
}
static array *pop(struct stack *s) {
	return (s->top<=s->bot)?NULL:--s->top;
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
