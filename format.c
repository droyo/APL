#include <utf.h>
#include <fmt.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "apl.h"

static char buf[1024];
static char *top;
static void add(char*, ...);
static void num(array *);
#define END (buf+NELEM(buf))
#define SPACE (END-top)

/* Calls to fmt trash previous strings returned by fmt. The
 * Return value must be used immediately or copied to a fresh
 * array */
char* fmt(array *a) {
	char *s = top;
	switch(a->t) {
	case assign: add("←");  break;
	case number: num(a);    break;
	case empty:  add("⍝");  break;
	case null:   add("∘");  break;
	case lparen: add("(");  break;
	case rparen: add(")");  break;
	case colon:  add(":");  break;
	case dydop:case monop:case function:
		if(a->f&primitive)
			add("%C",*(Rune*)aval(a));
		else goto String;
		break;
	default:String: add("%s", aval(a));
	}
	*top++ = 0;
	return s;
}

void fmt_reset(void) { top = buf; }
static void num(array *a) {
	int i;
	double *d = aval(a);
	for(i = 0; i < a->n; i++) {
		add(i?" ":"");
		if(d[i] == INFINITY)add ("∞"); else 
		if(d[i]==-INFINITY) add("-∞");
		else add("%g",d[i]);
	}
}

static void add(char *fmt, ...) {
	va_list ap;
	va_start(ap,fmt);
	top=vseprint(top,END,fmt,ap);
	va_end(ap);
}
