#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include "apl.h"

static array *top;
static array tok[128];
static struct {
	char pool[512];
	char *top;
} mem;
static void *push(void *p, long n);
static void *alloc(long n);
static void *pop(long n);

int scan_numeral(Biobuf *i);
int scan_literal(Biobuf *i);
int scan_special(Biobuf *i);
int scan_symbol(Biobuf *i);
int scan_delims(Biobuf *i);
int scan_operator(Biobuf *i);

char quotes[] = "`'";
char delims[] = "(){}[]♦:;⍝";
char digits[] = "¯0123456789";
char operators[] = "\\⍀/⌿¨∘.⍤";
char special[] = 
	"~!@#$%^&*_-=+<≤=≥>≠∨^×÷"
	"⍞⌶⍫⍒⍋⌽⍉⊖⍟⍱⍲⌹?⍵∊⍴↑↓⍳○*←→⊢"
	"⍷⍐⍗⍸⌷⍇⍈⊣⍺⌈⌊∇∆⎕⍎⍕⌷⊃⊂∩∪⊥⊤|⍂⌻⍪";

array* scan(void *v) {
	int e;
	Rune r;
	Biobuf *i = v;
	top = tok;
	top++->t = empty;
	mem.top = mem.pool;
		
	while((r=Bgetrune(i))>0) {
		if(r == Beof || r == '\n') break;
		if(isspace(r)) continue;
		if(top-tok>NELEM(tok)) return NULL;
		Bungetrune(i);
		
		if(r == 0x2190)
			top->t = assign;
		else if (r == 0x235D) {
			Brdline(i,'\n');
			break;
		}else if(utfrune(digits, r))
			e = scan_numeral(i);
		else if(utfrune(quotes, r)) 
			e = scan_literal(i);
		else if(utfrune(delims, r)) 
			e = scan_delims(i);
		else if(utfrune(special, r)) 
			e = scan_special(i);
		else if(utfrune(operators, r))
			e = scan_operator(i);
		else 
			e = scan_symbol(i);
		if (e < 0) 
			return NULL;
		top++;
	}
	return top-1;
}

int scan_numeral(Biobuf *i) {
	Rune r;
	int *shape, j, e=0, dot=0;
	char digits[64];
	double d;
	top->n = 0;
	top->t = number;
	top->r = 1;
	top->m = mem.top;
	shape = alloc(sizeof *shape);

	for(j=0; j<NELEM(digits);j++) switch(r = Bgetrune(i)) {
		case 0xAF:
			if (j < 2 || digits[j-1] == 'e')
				digits[j] = '-';
			break;
		case '.':
			if (!dot) dot = digits[j] = '.';
			break;
		case 'e':
			if (!e) e = digits[j] = 'e';
			break;
		case '0':case '1':case '2':case '3':case '4':
		case '5':case '6':case '7':case '8': case '9':
			digits[j] = r;
			break;
		default:
			if (j>0) {
				digits[j] = '\0';
				if(!strcmp(digits, "-")) 
					d = INFINITY;
				else if(!strcmp(digits, "--"))
					d = -INFINITY;
				else 
					d = strtod(digits, 0);
				push(&d, sizeof d);
				*shape = ++top->n;
			}
			if (r == '\t' || r == ' ') {
				j = -1; e = dot = 0;
				break;
			} else goto End;
	}
End: Bungetrune(i);
	top->r = top->n > 1 ? 1 : 0;
	if(!top->r) {
		pop(sizeof d + sizeof *shape);
		push(&d, sizeof d);
	}
	return 0;
}

int scan_literal(Biobuf *i) {
	int end = '\0';
	char q, c;
	int *shape;

	q = Bgetc(i);
	top->t = (q == '`') ? subcmd : string;
	top->m = mem.top;
	top->r = 1;
	shape = alloc(sizeof *shape);
	for(c=Bgetc(i);c!=q||(c=Bgetc(i))==q; c=Bgetc(i)) {
		if(c == '\n') return -1;
		push(&c,1);
	}
	Bungetc(i); push(&end,1);
	top->n = strlen((char*)top->m+sizeof *shape);
	*shape = utflen((char*)top->m+sizeof *shape);

	return 0;
}

int scan_special(Biobuf *i) {
	int end = '\0';
	Rune r = Bgetrune(i);
	top->r = 0;
	top->t = function;
	top->m = alloc(runelen(r));
	runetochar(top->m, &r); push(&end,1);
	top->n = strlen(top->m);
	return 0;
}

int scan_operator(Biobuf *i) {
	int end = '\0';
	Rune r = Bgetrune(i);
	top->m = alloc(runelen(r));
	runetochar(top->m, &r); push(&end,1);
	if(r=='.' || utfrune("⍤",r))
		top->t = doperator;
	else if(utfrune("∘",r)) {
		if(Bgetc(i) == '.')
			top->t = function;
		else top->t = doperator;
		Bungetc(i);
	} else top->t = moperator;
	top->n = strlen(top->m);
	return 0;
}

int scan_delims(Biobuf *i) {
	int end = '\0';
	Rune r = Bgetrune(i);
	top->r = 0;
	top->m = alloc(runelen(r));
	runetochar(top->m, &r); push(&end, 1);
	top->n = strlen(top->m);
	switch(r) {
		case '(': top->t = lparen;		break;
		case ')': top->t = rparen;		break;
		case '{': top->t = lbrace;		break;
		case '}': top->t = rbrace;		break;
		case ';': top->t = semicolon;	break;
		case ':': top->t = colon;		break;
		case 0x2666: top->t = diamond;	break;
	}
	return 0;
}

int scan_symbol(Biobuf *i) {
	Rune r;
	char *s;
	int end = '\0';
	top->n = 0;
	top->r = 0;
	top->t = symbol;
	top->m = mem.top;
	
	while((r = Bgetrune(i))>0) {
		if (isspace(r)) break;
		if (r!='.'&&utfrune(operators, r)) break;
		if (utfrune(quotes, r)) break;
		if (utfrune(delims, r)) break;
		if (utfrune(special, r)) break;
		
		s = alloc(runelen(r));
		runetochar(s, &r);
	}
	push(&end, 1);
	Bungetrune(i);
	return 0;
}

static void *push(void *p, long n) {
	void *v;
	if ((mem.top + n) > mem.pool + NELEM(mem.pool))
		return NULL;
	v = memcpy(mem.top, p, n);
	mem.top += n;
	return v;
}

static void *alloc(long n) {
	void *v = mem.top;
	if ((mem.top + n) > mem.pool + NELEM(mem.pool))
		return NULL;
	mem.top += n;
	return v;
}

static void *pop(long n) {
	if (mem.top - n < mem.pool) return NULL;
	else return mem.top -= n;
}
