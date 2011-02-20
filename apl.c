/* toy interpreter */
#include <fmt.h>
#include <bio.h>
#include <stdarg.h>
#include <stdlib.h>

#define OK 1
#define ERR -1
#define NELEM(a) (sizeof(a)/sizeof(*a))

void err(char *fmt, ...);
void die(char *fmt, ...);

int eval(char *b, char *e);
int apply(short fn, char *ab, char *ae, char *ob, char *oe);

int isfun(char c);
int isnum(char *b, char *e);
int n(char *ne, char *nb);

int apply(short fn, char *ab, char *ae, char *ob, char *oe) {
	int alpha, omega;

	omega = eval(ob, oe);
	alpha = eval(ab, ae);
	
	switch(fn) {
		case '+' : return alpha + omega;
		case '-' : return alpha - omega;
		case '*' : return alpha * omega;
		case '/' : return alpha / omega;
		default: die("Invalid function %c\n", fn);
	}
}

int eval(char *b, char *e) {
	short f;
	char *m;
	
	if (isnum(b,e))
		return n(b, e);
	
	for(m = b; m < e; m++) {
		if (isfun(*m) && (m == b || m == e))
			die("Missing argument to '%c'\n",*m);
		if (isfun(*m))
			return apply(*m, b, m-1, m+1, e);
	}
}

int main(void) {
	char *line;
	Biobuf *input;
	
	input = Bfdopen(0, O_RDONLY);
	
	while(*(line = Brdline(input, '\n'))) {
		print("\t%d\n", eval(line, line + Blinelen(input)-1));
	}
	
	return 0;
}

void err(char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	vfprint(2, fmt, ap);
	va_end(ap);
}

void die(char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	vfprint(2, fmt, ap);
	va_end(ap);
	exit(1);
}

short fns[] = { '+', '-', '/', '*' };
int isfun(char c) {
	int i;for (i=0; i<NELEM(fns); i++)
		if(c==fns[i]) return 1;
	return 0;
}

int isnum(char *b, char *e) {
	for(;b<e;b++) if(!isdigit(*b)) return 0;
	return 1;
}

int n(char *ne, char *nb) {
	return strtol(ne, &nb, 0);
}
