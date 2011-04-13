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
static array* scan_symbol  (array*,array*,array*,Biobuf*);
static array* scan_delims  (array*,Biobuf*);
static array* scan_special (array*,Biobuf*);

array *scan(array *E,array *tok,array *buf,void *v) {
	Rune r; 
	array *a;
	Biobuf *i = v;
	array *del = get(E, "⎕DELIM");
	array *dig = get(E, "⎕DIGIT");
	array *pri = get(E, "⎕PRICH");

	if(!apush(tok,&marker))
		return enil(Elexline,tok->z);

	while((r=Bgetrune(i))>0) {
		if(afull(buf)) {
			Brdline(i,'\n');
			return enil(Elexmem,buf->z);
		}
		if(r == Beof || r == '\n') break;
		if(isspace(r)) continue;
		Bungetrune(i);

		if(r==ULAMP){Brdline(i,'\n');break;}
		else if(afind(dig,&r))a=scan_numeral(buf,i);
		else if(r == '\'')    a=scan_literal(buf,i);
		else if(afind(del,&r))a=scan_delims (buf,i);
		else if(afind(pri,&r))a=scan_special(buf,i);
		else          a=scan_symbol (del,pri,buf,i);
		
		if (!a) {
			Brdline(i,'\n');
			if(afull(buf))
				return enil(Elexmem,buf->z);
			else
				return enil(Elexline,tok->z);
		}
		if(!apush(tok,&a)) 
			return enil(Elexline,tok->z);
		else put(E,"⎕SCTOK", tok);
	}
	return tok;
}

static array* scan_numeral(array *p, Biobuf *i) {
	Rune r; double d; char n[64];
	array *a = parray(p,TNUM, 1, 0);
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
	array *a = parray(p,TSTR,1,0);
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
		case ':': t = TCOL; break;
		case '[': t = TLDF; break;
		case ']': t = TRDF; break;
		case '(': t = TLPR; break;
		case ')': t = TRPR; break;
	}
	return parray(p,t,0,0);
}

static array* scan_special(array *p,Biobuf *i) {
	Rune r = Bgetrune(i);
	array *a = parray(p,TSYM, 0, 0);
	if (!a) return NULL;
	switch(r) {
	case UASSIGN: a->t = TSET; break;
	case '.':
	Bungetrune(i);Bgetc(i);
	if(isdigit(Bgetc(i))) {
		Bungetc(i); Bungetc(i);
		return scan_numeral(p,i);
	}
	default:
		push(p,&r, sizeof r);
		a->z=a->n=1;
	}
	return a;
}

static array* scan_symbol(array *d,array *c,array *p,Biobuf *i) {
	Rune r;
	array *a = parray(p,TSYM,0,0);
	if(!a) return NULL;
	a->r = 1;
	
	while((r = Bgetrune(i))>0) {
		if(afind(c,&r)&&r!='.') break;
		else if(isspace(r))     break;
		else if(r == '\'')      break;
		else if(afind(d,&r))    break;
		push(p,&r, sizeof r);
		a->n++;
	}
	*ashp(a) = a->z = a->n;
	Bungetrune(i);
	return a;
}

static array *parray(array *p,enum tag t, unsigned r, unsigned n){
	void *m = amem(p,ASIZE);
	if(!m) return NULL;
	array *a = atmp(m,t,r,n);
	int i = a->k; a->z = a->n;
	while(i--) {
		if(!push(p,&zero, sizeof (int)))
			return NULL;
	}
	return a;
}
static void *push(array *p,const void *v, long n) {
	void *r;
	if(!(r = amem(p,n))) return NULL;
	else memcpy(r,v,n);  return r;
}
