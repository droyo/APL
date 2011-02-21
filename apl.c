/* toy interpreter - unicode */
#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <unistd.h>
#include "apl.h"

int scan(Biobuf *in, int out) {
	Rune r, n, quote;
	enum FSM state;

	quote = n = 0;
	state = SCANNING;
	
	while((r=Bgetrune(in))>0) {
		if(state == ERROR) return -1;

		if(scanliteral(&state, out, r, &quote)) goto Tokenized;
		if(utfrune("⍝", r)) {
			Brdline(in, '\n');
			return 0;
		}
		if(scannumeral(&state, out, r)) goto Tokenized;
		if(scandelimiter(&state, out, r)) goto Tokenized;
		if(scanprimitive(&state, out, r)) goto Tokenized;
		if(scanwhitespace(&state, out, r)) goto Tokenized;

		if (r == '.' && (n=Bgetrune(in))>0){
			Bungetrune(in);
			 if (isdigit(n)) switch (state) {
			 	case SCANNING:
			 		fprint(out, "NUMERAL 0.");
			 		break;
			 	case A_NUMBER:
			 		fprint(out, ".");
			 		break;
			 	default:
			 		fprint(2, "Space required before `.'\n");
			 		return -1;
			 }
			 if (utfrune(primitives, n)) switch(state) {
			 	default: fprint(out, "\n");
			 	case SCANNING:
			 		fprint(out, "OPERATOR .\n");
			 }
		} else switch(state) {
			case SCANNING: 
				fprint(out, "NAME %C", r);
				state = A_SYMBOL;
				break;
			case A_NUMBER: fprint(2, "Scan error"); break;
			default: break;
		}
	Tokenized: if(state == FINISHED) return 0;
	}
	return 1;
}

int eval(Biobuf *in) {
	char *s;
	while((s = Brdstr(in, '\n', 0)))
		print(s);
	free(s);
}

int main(void) {
	Rune r;
	int chan[2];

	Biobuf *input;
	Biobuf *parsein;
	
	if(pipe2(chan, O_NONBLOCK)) {
		fprint(2, "Pipe error\n");
		exit(1);
	}
	input = Bfdopen(0, O_RDONLY);
	parsein = Bfdopen(chan[0], O_RDONLY);

	while(!scan(input, chan[1])) {
		eval(parsein);
	}
Cleanup:
	Bterm(input);
	Bterm(parsein);
	return 0;
}

int scanliteral(enum FSM *state, int wr, Rune r, Rune *quote){
	if (r == *quote) {
		*quote = 0;
		*state = SCANNING;
		fprint(wr, "\n");
		return 1;
	}
	if (*state == A_LITERAL) {
		fprint(wr, "%C", r);
		return 1;
	}
	if (utfrune(quotemarks, r)) {
		if (*state == SCANNING) {
			*quote = r;
			*state = A_LITERAL;
			switch(r) {
				case '"':fprint(wr, "DOUBLEQUOTE "); break;
				case '`':fprint(wr, "BACKQUOTE "); break;
				default: fprint(wr, "SINGLEQUOTE "); break;
			}
			return 1;
		} else {
			fprint(2, "need space before %C\n", r);
			*state = ERROR;
			return -1;
		}
	}
	return 0;
}

int scannumeral(enum FSM *state, int wr, Rune r){
	if (!isdigit(r) && !utfrune("¯", r)) return 0;
	if (isdigit(r)) switch(*state) {
	case SCANNING: 
		fprint(wr, "NUMERAL ");
		*state = A_NUMBER;
	default: fprint(wr, "%C", r);
	}
	else if (utfrune("¯", r)) switch(*state) {
	case SCANNING:
		*state = A_NUMBER;
		fprint(wr, "NUMERAL -");
		break;
	default: 
		*state = ERROR;
		fprint(2, "Scanner error\n");
		return -1;
	}
	return 1;
}

int scandelimiter(enum FSM *state, int wr, Rune r){
	if (!utfrune(delimiters, r)) return 0;
	switch(*state) {
	default: 
		fprint(wr, "\n");
	case SCANNING:
		fprint(wr, "DELIMITER %C\n", r);
		*state = SCANNING;
		break;
	case A_LITERAL:
		fprint(wr, "%C", r);
	}
	return 1;
}

int scanprimitive(enum FSM *state, int wr, Rune r){
	if (!utfrune(primitives, r)) return 0;
	switch(*state) {
	default: 
		fprint(wr, "\n");
	case SCANNING:
		fprint(wr, "PRIMITIVE %C\n", r);
		*state = SCANNING;
		break;
	case A_LITERAL:
		fprint(wr, "%C", r);
	}
	return 1;
}

int scanwhitespace(enum FSM *state, int wr, Rune r){
	if (!isspace(r)) return 0;
	
	switch(*state) {
	default:
		fprint(wr, "\n");
	case SCANNING:
		*state = r == '\n' ? FINISHED:SCANNING;
		break;
	case A_LITERAL:
		fprint(wr, "%C", r);
	}
	return 1;
}
