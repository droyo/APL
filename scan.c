#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "apl.h"
#include "const.h"
#include "error.h"

const int zero = 0;
typedef struct{char *bot,*top,*end;} pool;
static pool mkpool(char *b, int n);

static void*  mem(pool*,long);
static array* parray(pool*,enum tag,unsigned,unsigned);
static void*  push(pool*,const void*,long);

static array* scan_numeral (pool*,Biobuf*);
static array* scan_literal (pool*,Biobuf*);
static array* scan_function(pool*,Biobuf*);
static array* scan_symbol  (pool*,Biobuf*);
static array* scan_delims  (pool*,Biobuf*);
static array* scan_operator(pool*,Biobuf*);
static array* scan_special (pool*,Biobuf*);

int scan(void *v, array **tok, int tn, char *buf, int bn) {
	Rune r; 
	array **t;
	int top = 0;
	Biobuf *i = v;
	tok[top++] = marker;
	pool p = mkpool(buf,bn);

	while((r=Bgetrune(i))>0) {
		if(top>tn) {
			Brdline(i,'\n');
			return eneg(Elexline,tn);
		}
		if(r == Beof || r == '\n') break;
		if(isspace(r)) continue;
		Bungetrune(i);
		t = tok+top;

		if(r==ULAMP){Brdline(i,'\n');break;}
		else if(isapldig(r))*t=scan_numeral (&p,i);
		else if(r == '\'')  *t=scan_literal (&p,i);
		else if(isapldel(r))*t=scan_delims  (&p,i);
		else if(isaplop(r)) *t=scan_operator(&p,i);
		else if(isaplfun(r))*t=scan_function(&p,i);
		else if(isaplchr(r))*t=scan_special (&p,i);
		else                *t=scan_symbol  (&p,i);
		
		if (!tok[top]) {
			Brdline(i,'\n');
			return -1;
		}
		top++;
	}
	return top-1;
}

static array* scan_numeral(pool *p, Biobuf *i) {
	Rune r; double d; char n[64];
	array *a = parray(p,number, 1, 0);
	if(!a) return NULL;
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
			push(p,&d, sizeof d);
			*ashp(a) = ++a->n;
		}
		if (r == '\t' || r == ' ') {
			j = -1; e = dot = 0;
			break;
		} else goto End;
	}
	End: Bungetrune(i);
	a->r = a->n > 1 ? 1 : 0;
	return a;
}

static array* scan_literal(pool *p,Biobuf *i) {
	Rune r,q;
	array *a = parray(p,string, 1, 0);
	if(!a) return NULL;
	a->n = 0;
	q = Bgetrune(i);
	
	for(r=Bgetrune(i);r!=q||(r=Bgetrune(i))==q; r=Bgetrune(i)) {
		if(r == '\n') return NULL;
		push(p,&r,sizeof r);
		a->n++;
	}
	Bungetrune(i);
	*ashp(a) = a->n;

	return a;
}

static array* scan_function(pool *p,Biobuf *i) {
	Rune r = Bgetrune(i);
	array *a = parray(p,function, 0, 0);
	if(!a) return NULL;
	a->f|=primitive; a->n = 1; push(p,&r, sizeof r);
	return a;
}

static array* scan_operator(pool *p,Biobuf *i) {
	array *a;
	int c; Rune r = Bgetrune(i);
	/* Make sure it's '.' the operator, and
	   not the start of a decimal number */
	if (r == '.') {
		Bungetrune(i);
		Bgetc(i); c = Bgetc(i);
		if(isdigit(c)) {
			Bungetc(i); Bungetc(i);
			return scan_numeral(p,i);
		} else Bungetc(i);
	}
	a=parray(p,isapldop(r)?dydop:monop,0,0);
	if(!a) return NULL;
	a->f|=primitive;a->n = 1; push(p,&r, sizeof r);
	return a;
}

static array* scan_delims(pool *p,Biobuf *i) {
	int c = Bgetc(i); enum tag t;
	switch(c) {
		case ':': t = colon;  break;
		case '[': t = ldfns;  break;
		case ']': t = rdfns;  break;
		case '(': t = lparen; break;
		case ')': t = rparen; break;
	}
	return parray(p,t,0,0);
}

static array* scan_special(pool *p,Biobuf *i) {
	Rune r = Bgetrune(i);
	array *a = parray(p,empty, 0, 0);
	if (!a) return NULL;
	switch(r) {
	case UASSIGN: a->t = assign; break;
	default:
		push(p,&r, sizeof r);
		a->t = symbol;
		a->n=1;
	}
	return a;
}

static array* scan_symbol(pool *p, Biobuf *i) {
	Rune r;
	array *a = parray(p,symbol,0,0);
	if(!a) return NULL;
	a->n = 0;
	
	while((r = Bgetrune(i))>0) {
		if(r!='.'&&isaplop(r)) break;
		else if(isspace(r))    break;
		else if(r == '\'')     break;
		else if(isapldel(r))   break;
		else if(isaplfun(r))   break;
		else if(isaplchr(r))   break;
		push(p,&r, sizeof r);
		a->n++;
	}
	Bungetrune(i);
	return a;
}

static array *parray(pool *p,enum tag t, unsigned r, unsigned n){
	void *m = mem(p,ASIZE);
	if(!m) return enil(Elexmem);
	array *a = atmp(m,t,r,n);
	int i = a->k;
	while(i--) {
		if(!push(p,&zero, sizeof (int)))
			return enil(Elexmem);
	}
	return a;
}
static void *push(pool *p,const void *v, long n) {
	void *r;
	if (p->top+n > p->end) return NULL;
	r = memcpy(p->top, v, n);
	p->top += n;
	return r;
}
static void *mem(pool *p, long n) {
	void *r = p->top;
	if (p->top+n > p->end) return NULL;
	p->top += n;
	return r;
}
static pool mkpool(char *buf, int n) {
	pool p; p.bot = p.top = buf; p.end = buf + n;
	return p;
}
