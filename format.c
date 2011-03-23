#include <utf.h>
#include <fmt.h>
#include <math.h>
#include <stdarg.h>
#include "apl.h"

enum boxdrawing {
  HO  = 0x2500, VE  = 0x2502, UR  = 0x2510, DR  = 0x2518,
  UL  = 0x250C, DL  = 0x2514, LJ  = 0x2524, RJ  = 0x251C,
  UJ  = 0x2534, DJ  = 0x252C, HHO = 0x2550, VVE = 0x2551,
  UUR = 0x2557, DDR = 0x255D, UUL = 0x2554, DDL = 0x255A
};
static int Afmt(Fmt*);
static int Afmtn(Fmt*,array*);
static int Afmtb(Fmt*,array*);

static int N   (Fmt*,double*,int);
static int Nx1 (Fmt*,double*,int,int);
static int NxM (Fmt*,double*,int,int,int);
static int NxMx(Fmt*,double*,int,int*,int);
static int getw(array*);

static int Afmt(Fmt *f) {
	array *a = va_arg(f->args, array*);
	switch(a->t) {
	case function: case dydop: case monop: case niladic:
		if(a->f&primitive)
			return fmtrune(f, *(Rune*)aval(a));
		else break;
	case number: return Afmtn(f,a);
	case boxed:  return Afmtb(f,a);
	case null:   return fmtprint(f,"∘");
	case empty:  return fmtprint(f,"⍝");
	case assign: return fmtprint(f,"←");
	case colon:  return fmtprint(f,":");
	case lparen: return fmtprint(f,"(");
	case rparen: return fmtprint(f,")");
	default:     return fmtstrcpy(f,aval(a));
	}
	return 0;
}
static int Afmtn(Fmt *f,array *a) {
	int *s = ashp(a), w = getw(a);
	switch(a->r) {
	case 0:  return N(f,aval(a),0);
	case 1:  return Nx1(f,aval(a),w,s[0]);
	case 2:  return NxM(f,aval(a),w,s[0],s[1]);
	default: return NxMx(f,aval(a),w,s,a->r);
	}
}

static int N(Fmt *f, double *d, int w) {
	if(*d == INFINITY) return fmtprint(f, "%*s", w, "∞");
	if(*d ==-INFINITY) return fmtprint(f, "%*s", w,"-∞");
	return fmtprint(f,"%*g", w, *d);
}

static int Nx1(Fmt *f, double *d, int w, int n) {
	int i;for(i=0;i<n;i++) {
		if(fmtprint(f,i?" ":"")<0) return -1;
		else if(N(f,d+i,w)<0) return -1;
	}
	return 0;
}

static int NxM(Fmt *f, double *d, int w, int m, int n) {
	int i; for(i=0;i<m;i++) {
		if(Nx1(f, d+i*n,w,n)<0) return -1;
		fmtprint(f,i<m?"\n":"");
	}
	return 0;
}

static int NxMx(Fmt *f, double *d, int w, int *s, int r) {
	int i, o;
	if(r == 2) return NxM(f,d,w,s[0],s[1]);
	for(i=o=1;i<r;i++) o *= s[i];
	for(i=0;i<s[0];i++) {
		if(NxMx(f,d+(o*i),w,s+1,r-1)<0) 
			return -1;
		else if(fmtprint(f,i<s[0]-1?"\n":"")) 
			return -1;
	}
	return 0;
}

static int getw(array *a) {
	int i,m,n; char buf[8];
	double *d = aval(a);
	for(i=0,m=0;i<a->n;i++)
		m=m<(n=snprint(buf,8,"%g",d[i]))?n:m;
	return m;
}
		
static int Afmtb(Fmt *f, array *a) {
	return 0;
}
int fmt_init(void) {
	return fmtinstall('A', Afmt);
}
