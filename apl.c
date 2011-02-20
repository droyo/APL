/* toy interpreter - unicode */
#include <utf.h>
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
int lex(int fd);

int isfun(Rune r);
int isdelim(Rune r);
int isop(Rune r);

char delims[] = "(){}[]";
char fns[] = "⊃⊂,+-×÷⍴⌈⌊←→⍳♦⍝";
char ops[] = "¨/\\";

Biobuf *input;
Biobuf *parsein;

int lex(int fd) {
	Rune r;
	int state;

	state = IDLE;
	while((r=Bgetrune(input))>0) {
		if (r=='\n') {parse();print("\n");}
		if (isfun(r)) switch(state) {
		case A_NUM: case A_SYM:
			state = IDLE;
			fprint(fd, "\n");
		case IDLE:
			fprint(fd, "FUN %C\n", r);
		}
		else if (isop(r)) switch(state) {
		case A_NUM: case A_SYM:
			state = IDLE;
			fprint(fd, "\n");
		case IDLE:
			fprint(fd, "OP %C\n", r);
		}
		else if (isdelim(r)) switch(state) {
		case A_NUM: case A_SYM:
			state = IDLE;
			fprint(fd, "\n");
		case IDLE:
			fprint(fd, "DELIM %C\n", r);
		}
		else if (isdigit(r)) switch(state) {
		case IDLE:
			state = A_NUM;
			fprint(fd, "NUM ");
		case A_NUM: case A_SYM:
			fprint(fd, "%C", r);
			break;
		}
		else if (utfrune("¯",r)) switch(state) {
		case IDLE:
			state = A_NUM;
			fprint(fd, "NUM -");
			break;
		case A_SYM: case A_NUM:
			err("Lexing error\n");
		}
		else if (r == '.') switch(state) {
		case IDLE:
			if (!isdigit(Bgetrune(input))) {
				err("Symbol no begin with '.'\n");
			} else {
				state = A_NUM;
				fprint(fd, "NUM 0.");
			}
			Bungetrune(input);
			break;
		case A_NUM:
			if (!isdigit(Bgetrune(input))) 
				err("Numbers no end with decimal!\n");
			Bungetrune(input);
		case A_SYM:
			fprint(fd, "%C", r);
		}
		else if (isspace(r)) switch(state) {
		case A_SYM: case A_NUM:
			state = IDLE;
			fprint(fd, "\n");
		case IDLE: break;
		}
		else switch(state) {
		case IDLE:
			state = A_SYM;
			fprint(fd, "SYM ");
		case A_SYM:
			fprint(fd, "%C", r);
			break;
		case A_NUM:
			err("Lexing error\n");
		}
	}
}

int main(void) {
	Rune r;
	int chan[2];

	if(pipe2(chan, O_NONBLOCK))
		die("Pipe error\n");
	input = Bfdopen(0, O_RDONLY);
	parsein = Bfdopen(chan[0], O_RDONLY);

	lex(chan[1]);
Cleanup:
	Bterm(input);
	Bterm(parsein);
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

int isfun(Rune r) { return !!utfrune(fns, r); }
int isop(Rune r) { return !!utfrune(ops, r); }
int isdelim(Rune r) { return !!utfrune(delims, r); }

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
