#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "apl.h"

array tok[256];

int scan_numeral(Biobuf *i);
int scan_literal(Biobuf *i);
int scan_special(Biobuf *i);
int scan_symbol(Biobuf *i);
int scan_delims(Biobuf *i);

static int top;
static int mem_ok(int x);
static int mem_add(Rune r);
static void mem_end(void);
static char *mem_str(Biobuf *b, Rune d);

static struct {
	long max;
	char *top;
	char *pool;
} mem;

char quotes[] = "\"'`";
char delims[] = "(){}[]♦:;";
char digits[] = "¯0123456789";
char special[] = "×+-¨÷≠"
	"⍫⍒⍋⌽⍉⊖⍟⍱⍲!⌹?⍵∊⍴~↑↓⍳○*←→"
	"⍥⍷⍬⍐⍗⍸⌷⍇⍈,+-⍺⌈⌊_∇∆∘'⍎⍕⋄"
	"⍙⌷≡≢⍞/\\⊢⍪⍤⊣⊂⊃∩∪⊥⊤|⍝⍀⌿.";

int fill(void) {
	int *shp;
	Rune r; chartorune(&r,"♦");
	tok[top].m = mem.top;
	if(mem_ok(sizeof *shp)) {
		shp = (int*)mem.top;
		mem.top += sizeof *shp;
	} else return 0;
	*shp = 1;
	tok[top].t = diamond;
	tok[top].n = mem_add(r); mem_end();
	tok[top].r = 1;
	top++;
}

int scan(void *v) {
	int e;
	Rune r;
	Biobuf *i = v;
	top = 0;
	mem.top = mem.pool;
	if (!fill()) return 0;
		
	while((r=Bgetrune(i))>0) {
		if(r == Beof || r == '\n') break;
		if(isspace(r)) continue;
		if(top>=NELEM(tok)) return -1;
		Bungetrune(i);
		
		if(utfrune(digits, r)) 
			e = scan_numeral(i);
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

int scan_numeral(Biobuf *i) {
	Rune r;
	int sign;
	int *shp;
	double *d;
	
	tok[top].n = 0;
	tok[top].t = number;
	tok[top].m = mem.top;
	tok[top].r = 1;
	if(mem_ok(sizeof *shp)) {
		shp = (int*)mem.top;
		mem.top += sizeof *shp;
	} else return -1;
	d = (double*)mem.top;
	
	while(mem_ok(sizeof *d)) {
		r = Bgetrune(i);
		if(isblank(r)) continue;
		if(!utfrune(digits, r)) break;
		sign = utfrune("¯",r)?-1:1;
		if(sign > 0) Bungetrune(i);
		Bgetd(i, d); *d *= sign;
		mem.top += sizeof (double);
		d = (double *)mem.top;
		tok[top].n++;
	}
	Bungetrune(i);
	*shp = tok[top].n;
	tok[top].r = tok[top].n > 1 ? 1 : 0;
	if(!tok[top].r) 
		memmove(tok[top].m, tok[top].m+sizeof *shp, sizeof *d * tok[top].n);
	return 0;
}

int scan_literal(Biobuf *i) {
	Rune q;
	char *s;
	int *shp;

	q = Bgetrune(i);
	tok[top].m = mem.top;
	tok[top].r = 1;
	if(mem_ok(sizeof *shp)) {
		shp = (int*)mem.top;
		mem.top += sizeof *shp;
	} else return -1;
	
	s = mem_str(i, q);
	tok[top].n = strlen(s);
	*shp = utflen((char*)tok[top].m);
	tok[top].t = (q == '`') ? subcmd : string;

	return 0;
}

int scan_special(Biobuf *i) {
	Rune r = Bgetrune(i);
	tok[top].m = mem.top;
	if (mem_add(r)<0) return -1; else mem_end();
	tok[top].t = utfrune("←", r) ? assign : function;
	tok[top].n = strlen(tok[top].m);
	tok[top].r = 0;
	return 0;
}

int scan_delims(Biobuf *i) {
	Rune r = Bgetrune(i);
	enum tag *t = &tok[top].t;
	tok[top].r = 0;
	tok[top].m = mem.top;
	if (mem_add(r)<0) return -1; else mem_end();
	tok[top].n = strlen(tok[top].m);
	switch(r) {
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
	return 0;
}

int scan_symbol(Biobuf *i) {
	Rune r;
	int *shp;
	tok[top].n = 0;
	tok[top].r = 1;
	tok[top].t = symbol;
	tok[top].m = mem.top;
	
	if(mem_ok(sizeof *shp)) {
		shp = (int*)mem.top;
		mem.top += sizeof *shp;
	} else return -1;
	
	while((r = Bgetrune(i))>0) {
		if (isspace(r)) break;
		if (utfrune(quotes, r)) break;
		if (utfrune(delims, r)) break;
		if (utfrune(special, r)) break;

		tok[top].n += mem_add(r);
	}
	Bungetrune(i);
	mem_end();
	*shp = utflen(tok[top].m);
	return 0;
}

static int mem_add(Rune r) {
	int n = 0;
	if(!mem_ok(runelen(r))) return -1;
	n += runetochar(mem.top, &r);
	mem.top += n;
	return n;
}
static void mem_end(void) {
	*mem.top = '\0';
	mem.top++;
}
static char *mem_str(Biobuf *b, Rune d) {
	int i;
	char *t = mem.top;
	char *s = Brdline(b, d);
	int n = Blinelen(b);
	s[n-1] = '\0';
	if(!mem_ok(n)) {
		fprint(2, "%r\n");
		return NULL;
	}
	for(i=0;i<n;i++) *mem.top++ = s[i];
	return t;
}
static int mem_ok(int x) {
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
