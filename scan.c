/* toy interpreter - unicode */
#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <string.h>
#include "apl.h"
#include "scan.h"

char quotemarks[] = "\"'`";
char delimiters[] = "(){}[]⋄:";
char primitives[] = "¨¯<≤=≥>≠∨^×÷"
	"⌶⍫⍒⍋⌽⍉⊖⍟⍱⍲!⌹"
	"?⍵∊⍴~↑↓⍳○*←→"
	"⍥⍷⍬⍐⍗⍸⌷⍇⍈,+-"
	"⍺⌈⌊_∇∆∘'⎕⍎⍕⋄"
	"⍙⌷≡≢⍞/\\⊢⍪⍤"
	"⊣⊂⊃∩∪⊥⊤|⍝⍀⌿";

enum { max_digits = 32 };

int scan(Biobuf *in, int out) {
	int i;
	unsigned char h;
	Rune r, n, quote;
	enum FSM state;
	char nbuf[max_digits];
	double d;
	
	quote = n = h = 0;
	state = SCANNING;
	memset(nbuf, 0, sizeof d);
	
	while((r=Bgetrune(in))>0) {
		if(state == ERROR) return -1;

		if(scanliteral(&state, out, r, &quote)) goto Tokenized;
		if(utfrune("⍝", r)) { /* toss comments */
			Brdline(in, '\n');
			return 0;
		}
		if(scannumeral(&state, out, r, nbuf)) goto Tokenized;
		if(scandelimiter(&state, out, r)) goto Tokenized;
		if(scanprimitive(&state, out, r)) goto Tokenized;
		if(scanwhitespace(&state, out, r)) goto Tokenized;

		if (r == '.' && (n=Bgetrune(in))>0){
			Bungetrune(in);
			if (isdigit(n)) switch (state) {
				case A_LITERAL:
			 		fprint(out, "%C", r);
					break;
				case SCANNING:
					h = TVALUE | TNUMBER;
					write(out, &h, sizeof h);
					*nbuf = '.';
					state = A_NUMBER;
					break;
				case A_NUMBER:
					i = strlen(nbuf);
					if (i < max_digits - 1) nbuf[i] = '.';
					else state = ERROR;
					break;
				default:
					fprint(2, "Space required before `.'\n");
					return -1;
			}
			else if (utfrune(primitives, n)) switch(state) {
				case A_NUMBER:
					state = P_NUM;
					goto Tokenized;
				case A_LITERAL: 
					fprint(out, "%C", r);
				case A_SYMBOL:
					h = TSEP; write(out, &h, sizeof h);
				case SCANNING:
					h = TPRIMITIVE | TFUNCTION;
					write(out, &h, sizeof h);
			 }
		} else switch(state) {
			case SCANNING:
				h = TVALUE | TNAME;
				write(out, &h, sizeof h);
				fprint(out, "%C", r);
				state = A_SYMBOL;
				break;
			case A_NUMBER: 
				fprint(2, "Scan error\n"); break;
			default:
				fprint(out, "%C", r);
				break;
		}
	Tokenized: 
		if(state == P_NUM) {
			Bungetrune(in);
			d = strtod(nbuf, NULL);
			write(out, &d, sizeof d);
			memset(nbuf, 0, sizeof nbuf);
			state = SCANNING;
		}
		if(state == FINISHED) return 0;
	}
	return 1;
}

int scanliteral(enum FSM *state, int wr, Rune r, Rune *quote){
	unsigned char h = 0;
	if (r == *quote) {
		*quote = 0;
		*state = SCANNING;
		h = TSEP; 
		write(wr, &h, sizeof h);
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
			h = TVALUE | (r=='`') ? TQEXEC : TSTRING;
			write(wr, &h, sizeof h);
			return 1;
		} else {
			fprint(2, "need space before %C\n", r);
			*state = ERROR;
			return -1;
		}
	}
	return 0;
}

int scannumeral(enum FSM *state, int wr, Rune r, char *buf){
	int i;
	unsigned char h = 0;
	
	if (!isdigit(r) && !utfrune("¯", r)) return 0;
	if (isdigit(r)) switch(*state) {
	case A_NUMBER:
		i = strlen(buf);
		if (i < max_digits - 1) buf[i] = r;
		else *state = ERROR;
		break;
	case SCANNING: 
		*state = A_NUMBER;
		h = TVALUE | TNUMBER;
		write(wr, &h, sizeof h);
		*buf = r;
		break;
	default: fprint(wr, "%C", r);
	}
	else if (utfrune("¯", r)) switch(*state) {
	case SCANNING:
		*state = A_NUMBER;
		h = TVALUE | TNUMBER;
		write(wr, &h, sizeof h);
		*buf = '-';
		break;
	default: 
		*state = ERROR;
		fprint(2, "Scanner error\n");
		return -1;
	}
	return 1;
}

int scandelimiter(enum FSM *state, int wr, Rune r){
	unsigned char h = 0;
	if (!utfrune(delimiters, r)) return 0;
	switch(*state) {
	case A_NUMBER:
		*state = P_NUM;
		return 1;
	case A_SYMBOL:
		h = TSEP; write(wr, &h, sizeof h);
	case SCANNING:
		h = TPRIMITIVE | TDELIMITER;
		switch((unsigned char) r) {
			case '{' : h |= DOPENBRACE; break;
			case '(' : h |= DOPENPAREN; break;
			case '[' : h |= DOPENBRACK; break;
			case '}' : h |= DCLOSEBRACE; break;
			case ')' : h |= DCLOSEPAREN; break;
			case ']' : h |= DCLOSEBRACK; break;
		}
		write(wr, &h, sizeof h);
		*state = SCANNING;
		break;
	case A_LITERAL:
		fprint(wr, "%C", r);
	}
	return 1;
}

int scanprimitive(enum FSM *state, int wr, Rune r){
	unsigned char h = 0;
	if (!utfrune(primitives, r)) return 0;
	switch(*state) {
	case A_NUMBER:
		*state = P_NUM;
		return 1;
	case A_SYMBOL:
		h = TSEP; write(wr, &h, sizeof h);		
	case SCANNING:
		h = TPRIMITIVE | TFUNCTION;
		write(wr, &h, sizeof h);
		*state = SCANNING;
		break;
	case A_LITERAL:
		fprint(wr, "%C", r);
	}
	return 1;
}

int scanwhitespace(enum FSM *state, int wr, Rune r){
	unsigned char h = 0;
	if (!isspace(r)) return 0;
	
	switch(*state) {
	case A_NUMBER:
		*state = P_NUM;
		return 1;
	case A_SYMBOL:
		h = TSEP; write(wr, &h, sizeof h);
	case SCANNING:
		*state = r == '\n' ? FINISHED:SCANNING;
		break;
	case A_LITERAL:
		fprint(wr, "%C", r);
	}
	return 1;
}
