#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "apl.h"

int top;
struct obj tok[128];

int scan_number(Biobuf *i);
int scan_literal(Biobuf *i);
int scan_special(Biobuf *i);
int scan_symbol(Biobuf *i);
int scan_delims(Biobuf *i);

int mem_ok(int x);
int mem_add(Rune r);
void mem_end(void);
char *mem_str(Biobuf *b, Rune d);
void mem_dbg(void);

static struct {
	long max;
	char  *top;
	char *pool;
} mem;

char quotes[] = "\"'`";
char delims[] = "(){}[]♦:;";
char digits[] = "¯0123456789";
char special[] = "×+-¨÷≠"
	"⍫⍒⍋⌽⍉⊖⍟⍱⍲!⌹?⍵∊⍴~↑↓⍳○*←→"
	"⍥⍷⍬⍐⍗⍸⌷⍇⍈,+-⍺⌈⌊_∇∆∘'⍎⍕⋄"
	"⍙⌷≡≢⍞/\\⊢⍪⍤⊣⊂⊃∩∪⊥⊤|⍝⍀⌿.";

int scan(Biobuf *i) {
	int e;
	Rune r;
	top = 0;
	while((r=Bgetrune(i))>0) {
		if(r == '\n') break;
		if(isspace(r)) continue;
		if(top>=NELEM(tok)) return -1;
		Bungetrune(i);
		
		if(utfrune(digits, r)) 
			e = scan_number(i);
		else if(utfrune(quotes, r)) 
			e = scan_literal(i);
		else if(utfrune(delims, r)) 
			e = scan_delims(i);
		else if(utfrune(special, r)) 
			e = scan_special(i);
		else 
			e = scan_symbol(i);
		
		if (e < 0) return -1;
		top++;
	}
	return top-1;
}

int scan_number(Biobuf *i) {
	int sign;
	
	sign = utfrune("¯", Bgetrune(i)) ? -1 : 1;
	if (sign > 0) Bungetrune(i);
	
	Bgetd(i, &tok[top].v.d); 
	tok[top].v.d *= sign;
	tok[top].t = number;
	
	return 0;
}

int scan_literal(Biobuf *i) {
	Rune q;
	
	q = Bgetrune(i);
	tok[top].v.s = mem_str(i, q);
	tok[top].t = (q == '`') ? subcmd : string;

	return 0;
}

int scan_special(Biobuf *i) {
	Rune r;
	r = Bgetrune(i);
	if (utfrune("←", r)) {
		tok[top].t = assign;
		tok[top].v.p = r;
		return 0;
	}
	if (r == '?') {
		mem_dbg();
		return 1;
	}
	tok[top].t = function;
	tok[top].v.s = mem.top;
	mem_add(r); mem_end();
	return 0;
}

int scan_delims(Biobuf *i) {
	Rune r;
	enum tag *t = &tok[top].t;
	switch(r = Bgetrune(i)) {
		case '(': *t = lparen;
		case ')': *t = rparen;
		case '[': *t = lbracket;
		case ']': *t = rbracket;
		case '{': *t = lbrace;
		case '}': *t = rbrace;
		case ';': *t = semicolon;
		case ':': *t = colon;
		case 0x2666: *t = diamond;
	}
	tok[top].v.p = r;
	return 0;
}

int scan_symbol(Biobuf *i) {
	Rune r;
	tok[top].t = identifier;
	tok[top].v.s = mem.top;
	while((r = Bgetrune(i))>0) {
		if (isspace(r)) break;
		if (utfrune(quotes, r)) break;
		if (utfrune(delims, r)) break;
		if (utfrune(special, r)) break;

		mem_add(r);
	}
	Bungetrune(i);
	mem_end();
	return 0;
}

int mem_add(Rune r) {
	if(!mem_ok(runelen(r))) return -1;
	mem.top += runetochar(mem.top, &r);
	return 0;
}
void mem_end(void) {
	*mem.top = '\0';
	mem.top++;
}
char *mem_str(Biobuf *b, Rune d) {
	int i;
	char *t = mem.top;
	char *s = Brdline(b, d);
	int n = Blinelen(b);
	s[n-1] = '\0';
	if(!mem_ok(n)) return NULL;
	for(i=0;i<n;i++) *mem.top++ = s[i];
	return t;
}
int mem_ok(int x) {
	char *p;
	long n = mem.top - mem.pool;
	if(n > mem.max - (x+1)) {
		if(!(p = realloc(mem.pool, mem.max+512)))
			return 0;
		mem.max += 512;
		mem.pool = p;
		mem.top = mem.pool + n;
	} return 1;
}
void mem_dbg(void) {
	char *c;
	print("(%d/%d)|",mem.top-mem.pool, mem.max);
	for(c = mem.pool; c < mem.top; c++) {
		if(!*c) print("|");
		write(1, c, sizeof(char));
	}
	print("\n");
}
int init_scan(void) {
	mem.max = 512;
	if(!(mem.pool = malloc(mem.max))) {
		fprint(2, "Error: %r\n");
		return -1;
	}
	mem.top = mem.pool;
	return 0;
}
void cleanup_scan(void) {
	free(mem.pool);
}
