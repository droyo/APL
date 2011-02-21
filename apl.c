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
		if(quote) goto Send;

		if(scanliteral(&state, out, r, &quote)) goto Tokenized;
		if(scannumeral(&state, out, r)) goto Tokenized;
		if(scandelimiter(&state, out, r)) goto Tokenized;
		if(scanprimitive(&state, out, r)) goto Tokenized;
		if(scanwhitespace(&state, out, r)) goto Tokenized;

		if (r == '.' && (n=Bgetrune(in))>0){
			Bungetrune(in);
			 if (isdigit(n)) switch (state) {
			 	case SCANNING:
			 		fprint(out, "NUMBER 0.");
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
			 		fprint(out, "OP .\n");
			 }
		} else switch(state) {
			case SCANNING: 
				fprint(out, "NAME %C", r);
				state = A_SYMBOL;
				break;
			case A_NUMBER: fprint(2, "Scan error"); break;
			case A_SYMBOL:
			case A_LITERAL: Send: fprint(out, "%C", r);
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
	if (!utfrune(quotemarks, r)) return 0;
	
	switch(*state) {
	case SCANNING: 
		if(r == '"') fprint(wr, "DOUBLEQUOTE ");
		if(r == '`') fprint(wr, "BACKQUOTE ");
		else fprint(wr, "SINGLEQUOTE ");
		*state = A_LITERAL;
		break;
	case A_LITERAL: 
		if(r==*quote) {
			fprint(wr, "\n");
			*state = SCANNING;
		} else {
			fprint(wr, "%C", r);
		}
		break;
	default:
		*state = ERROR;
		fprint(2, "Space required before `%C'\n", r);
	}
	return 1;
}

int scannumeral(enum FSM *state, int wr, Rune r){
	if (!isdigit(r) && !utfrune("¯", r)) return 0;
	if (isdigit(r)) switch(*state) {
	case SCANNING: 
		fprint(wr, "NUMBER ");
		*state = A_NUMBER;
	default: fprint(wr, "%C", r);
	}
	else if (utfrune("¯", r)) switch(*state) {
	case SCANNING:
		*state = A_NUMBER;
		fprint(wr, "NUMBER -");
	default: 
		*state = ERROR;
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
		fprint(wr, "DELIM %C\n", r);
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
