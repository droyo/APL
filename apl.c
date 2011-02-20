/* toy interpreter - tokenization */
#include <fmt.h>
#include <bio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>

#define NELEM(a) (sizeof(a)/sizeof(*a))

enum {
	IDLE,
	A_NUM,
	A_SYM
} lex_state;

void err(char *fmt, ...);
void die(char *fmt, ...);

int parse(void);

int isfun(char c);
int isnum(char *b, char *e);
int n(char *ne, char *nb);

Biobuf *input;
Biobuf *parsein;

/* Numbers:
	¯?[0-9]+(.[0-9]+)?
   Functions:
   	[*+-/]
   Symbols:
   	[a-zA-Z][a-zA-Z0-9]*
*/
int main(void) {
	int c;
	int chan[2];
	enum lex_state state;

	if(pipe2(chan, O_NONBLOCK))
		die("Pipe error\n");
	input = Bfdopen(0, O_RDONLY);
	parsein = Bfdopen(chan[0], O_RDONLY);

	state = IDLE;
	while((c=Bgetc(input))>0) {
		if (c=='\n') {parse();print("\n");}
		if (isfun(c)) switch(state) {
		case A_NUM:
		case A_SYM:
			state = IDLE;
			fprint(chan[1], "\n");
		case IDLE:
			fprint(chan[1], "FUN %c\n", c);
		}
		else if (isdigit(c)) switch(state) {
		case IDLE:
			state = A_NUM;
			fprint(chan[1], "NUM ");
		case A_NUM:
		case A_SYM:
			fprint(chan[1], "%c", c);
			break;
		}
		else if (isalpha(c)) switch(state) {
		case IDLE:
			state = A_SYM;
			fprint(chan[1], "SYM ");
		case A_SYM:
			fprint(chan[1], "%c", c);
			break;
		case A_NUM:
			err("Lexing error\n");
		}
		else if (isspace(c)) switch(state) {
		case A_SYM:
		case A_NUM:
			state = IDLE;
			fprint(chan[1], "\n");
		case IDLE: break;
		}
		else err("Invalid character %c\n", c);
	}
Cleanup:
	Bterm(input);
	return 0;
}

int parse(void) {
	char *s;
	while((s = Brdstr(parsein, '\n', 0))) {
		print(s);
		free(s);
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

/* [ 	]+[0-9]+[ 	]+ */
int isnum(char *b, char *e) {
	int x, p;
	for(x = 0, p = 0;b < e; b++) {
		if (!isdigit(*b) && !isspace(*b)) return 0;
		if (p && isdigit(*b)) x++;
		p=isspace(*b);
		
	}
	return x <= 2;
}

int n(char *ne, char *nb) {
	return strtol(ne, &nb, 0);
}
