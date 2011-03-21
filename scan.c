#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "apl.h"
#include "const.h"

static array *tok[128];
static array **ret[2];
static struct {char bot[1024],*top;} pool;

static void*  mem(long n);
static array* parray(enum tag, unsigned, unsigned);
static void*  push(void *p, long n);
static void*  pop(long n);
static void   reset_mem(void);

static array* scan_numeral (Biobuf *i);
static array* scan_literal (Biobuf *i);
static array* scan_function(Biobuf *i);
static array* scan_symbol  (Biobuf *i);
static array* scan_delims  (Biobuf *i);
static array* scan_operator(Biobuf *i);
static array* scan_special (Biobuf *i);

array*** scan(void *v) {
	Rune r; 
	array **t;
	int top = 0;
	Biobuf *i = v;
	tok[top++] = marker;
	reset_mem();

	while((r=Bgetrune(i))>0) {
		if(top>NELEM(tok)) return NULL;
		t = tok+top;
		if(r == Beof || r == '\n') break;
		if(isspace(r)) continue;
		Bungetrune(i);

		if(r==ULAMP){Brdline(i,'\n');break;}
		else if(isapldig(r))*t=scan_numeral (i);
		else if(r == '\'')  *t=scan_literal (i);
		else if(isapldel(r))*t=scan_delims  (i);
		else if(isaplop(r)) *t=scan_operator(i);
		else if(isaplfun(r))*t=scan_function(i);
		else if(isaplchr(r))*t=scan_special (i);
		else                *t=scan_symbol  (i);
		
		if (!tok[top]) return NULL;
		top++;
	}
	ret[0] = tok;
	ret[1] = tok+top-1;
	return ret;
}

static array* scan_numeral(Biobuf *i) {
	Rune r; double d; char n[64];
	array *a = parray(number, 1, 0);
	int j, e=0, dot=0;

	for(j=0;j<NELEM(n);j++)switch(r=Bgetrune(i)) {
	case UMACRON:if (j<2||n[j-1]=='e')n[j]='-';break;
	case '.': if (!dot) dot = n[j] = '.'; break;
	case 'e': if (!e) e = n[j] = 'e'; break;
	case '0':case '1':case '2':case '3':case '4':
	case '5':case '6':case '7':case '8':case '9':
		n[j] = r; break;
	default:
		if (j>0) {
			n[j] = '\0';
			if(!strcmp(n, "-")) d = INFINITY; else
			if(!strcmp(n,"--")) d =-INFINITY;
			else d = strtod(n, 0);
			push(&d, sizeof d);
			*ashp(a) = ++a->n;
		}
		if (r == '\t' || r == ' ') {
			j = -1; e = dot = 0;
			break;
		} else goto End;
	}
	End: Bungetrune(i);
	a->r = a->n > 1 ? 1 : 0;
	if(!a->r) {
		pop(sizeof d + sizeof(int));
		push(&d, sizeof d);
	}
	return a;
}

static array* scan_literal(Biobuf *i) {
	char q, c;
	array *a = parray(string, 1, 0);

	q = Bgetc(i);
	for(c=Bgetc(i);c!=q||(c=Bgetc(i))==q; c=Bgetc(i)) {
		if(c == '\n') return NULL;
		push(&c,1);
	}
	Bungetc(i); push(&zero,1);
	a->n = strlen(aval(a))+1;
	*ashp(a) = utflen(aval(a));

	return a;
}

static array* scan_function(Biobuf *i) {
	Rune r = Bgetrune(i);
	array *a = parray(primitive, 0, 0);
	a->n = 1; push(&r, sizeof r);
	return a;
}

static array* scan_operator(Biobuf *i) {
	array *a;
	int c; Rune r = Bgetrune(i);
	/* Make sure it's '.' the operator, and
	   not the start of a decimal number */
	if (r == '.') {
		Bungetrune(i);
		Bgetc(i); c = Bgetc(i);
		if(isdigit(c)) {
			Bungetc(i); Bungetc(i);
			return scan_numeral(i);
		} else Bungetc(i);
	}
	a=parray(isapldop(r)?dydop:monop,0,0);
	a->n = 1; push(&r, sizeof r);
	return a;
}

static array* scan_delims(Biobuf *i) {
	int c = Bgetc(i); enum tag t;
	switch(c) {
		case '(': t = lparen;	break;
		case ')': t = rparen;	break;
		case ':': t = colon;	break;
	}
	return parray(t,0,0);
}

static array* scan_special(Biobuf *i) {
	Rune r = Bgetrune(i);
	array *a = parray(empty, 0, 0);
	switch(r) {
	case UASSIGN: a->t = assign; break;
	default: 
		a->t = symbol;
		a->n=runetochar(mem(runelen(r)), &r);
	}
	return a;
}

static array* scan_symbol(Biobuf *i) {
	Rune r;
	array *a = parray(symbol,0,0);
	
	while((r = Bgetrune(i))>0) {
		if(r!='.'&&isaplop(r)) break;
		else if(isspace(r))    break;
		else if(r == '\'')     break;
		else if(isapldel(r))   break;
		else if(isaplfun(r))   break;
		else if(isaplchr(r))   break;
		a->n+=runetochar(mem(runelen(r)), &r);
	}
	push(&zero, 1); a->n++;
	Bungetrune(i);
	return a;
}

static array *parray(enum tag t, unsigned r, unsigned n){
	array *a = atmp(mem(ASIZE),t,r,n);
	while(r--) push(&zero, sizeof (int));
	return a;
}
static void *push(void *p, long n) {
	void *v;
	if ((pool.top + n) > pool.bot + NELEM(pool.bot))
		return NULL;
	v = memcpy(pool.top, p, n);
	pool.top += n;
	return v;
}
static void *mem(long n) {
	void *v = pool.top;
	if ((pool.top + n) > pool.bot + NELEM(pool.bot))
		return NULL;
	pool.top += n;
	return v;
}
static void *pop(long n) {
	if (pool.top - n < pool.bot) return NULL;
	return pool.top -= n;
}
static void reset_mem(void) { pool.top = pool.bot; }
