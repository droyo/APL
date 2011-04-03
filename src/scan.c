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
static array* parray(array*,enum tag,unsigned,unsigned);
static void*  push(array*,const void*,long);
static array* scan_numeral (array*,Biobuf*);
static array* scan_literal (array*,Biobuf*);
static array* scan_symbol  (array*,Biobuf*);
static array* scan_delims  (array*,Biobuf*);
static array* scan_special (array*,Biobuf*);

array *scan(void *v, array **tok, array **buf) {
	Rune r; 
	array *a;
	Biobuf *i = v;

	if(!apush(tok,&marker)) return enil(Elexmem);

	while((r=Bgetrune(i))>0) {
		if(afull(*buf)) {
			Brdline(i,'\n');
			return enil(Elexmem);
		}
		if(r == Beof || r == '\n') break;
		if(isspace(r)) continue;
		Bungetrune(i);

		if(r==ULAMP){Brdline(i,'\n');break;}
		else if(isapldig(r)) a=scan_numeral (*buf,i);
		else if(r == '\'')   a=scan_literal (*buf,i);
		else if(isapldel(r)) a=scan_delims  (*buf,i);
		else if(isaplch(r))  a=scan_special (*buf,i);
		else                 a=scan_symbol  (*buf,i);
		
		if (!a) {
			Brdline(i,'\n');
			return NULL;
		}
		if(!apush(tok,&a)) return enil(Elexmem);
	}
	return *tok;
}

static array* scan_numeral(array *p, Biobuf *i) {
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
			*ashp(a) = a->z = ++a->n;
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

static array* scan_literal(array *p,Biobuf *i) {
	Rune r,q;
	array *a = parray(p,string, 1, 0);
	if(!a) return NULL;
	q = Bgetrune(i);
	
	for(r=Bgetrune(i);r!=q||(r=Bgetrune(i))==q;r=Bgetrune(i)) {
		if(r == '\n') return NULL;
		push(p,&r,sizeof r);
		a->n++;
	}
	*ashp(a) = a->z = a->n;
	Bungetrune(i);

	return a;
}

static array* scan_delims(array *p,Biobuf *i) {
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

static array* scan_special(array *p,Biobuf *i) {
	Rune r = Bgetrune(i);
	array *a = parray(p,empty, 0, 0);
	if (!a) return NULL;
	switch(r) {
	case UASSIGN: a->t = assign; break;
	case '.':
	Bungetrune(i);Bgetc(i);
	if(isdigit(Bgetc(i))) {
		Bungetc(i); Bungetc(i);
		return scan_numeral(p,i);
	}
	default:
		push(p,&r, sizeof r);
		a->t = symbol;
		a->z=a->n=1;
	}
	return a;
}

static array* scan_symbol(array *p, Biobuf *i) {
	Rune r;
	array *a = parray(p,symbol,0,0);
	if(!a) return NULL;
	a->r = 1;
	
	while((r = Bgetrune(i))>0) {
		if(isaplch(r)&&r!='.') break;
		else if(isspace(r))    break;
		else if(r == '\'')     break;
		else if(isapldel(r))   break;
		push(p,&r, sizeof r);
		a->n++;
	}
	*ashp(a) = a->z = a->n;
	Bungetrune(i);
	return a;
}

static array *parray(array *p,enum tag t, unsigned r, unsigned n){
	void *m = amem(p,ASIZE);
	if(!m) return enil(Elexmem);
	array *a = atmp(m,t,r,n);
	int i = a->k; a->z = a->n;
	while(i--) {
		if(!push(p,&zero, sizeof (int)))
			return enil(Elexmem);
	}
	return a;
}
static void *push(array *p,const void *v, long n) {
	void *r;
	if(!(r = amem(p,n))) return NULL;
	else memcpy(r,v,n);  return r;
}
