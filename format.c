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
static int Afmtn(Fmt*,int,array*);
static int Afmtb(Fmt*,int,array*);

static int N   (Fmt*,double*,int);
static int Nx1 (Fmt*,double*,int,int);
static int NxM (Fmt*,int,double*,int,int,int);
static int NxMx(Fmt*,int,double*,int,int*,int);

static int getw(array*);
static int geti(Fmt*);
static int llen(char*);

int fmt_init(void) {
	return fmtinstall('A', Afmt);
}
static int Afmt(Fmt *f) {
	int i = geti(f);
	array *a = va_arg(f->args, array*);
	switch(a->t) {
	case function: case dydop: case monop: case niladic:
		if(a->f&primitive)
			return fmtrune(f, *(Rune*)aval(a));
		else break;
	case number: return Afmtn(f,i,a);
	case boxed:  return Afmtb(f,i,a);
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

static int Afmtb(Fmt *f, int indent, array *a) {
	char **s; int i,j;
	array **x = aval(a);
	if(!(s = malloc(sizeof *s * a->n))) return -1;
	for(j=0;j<a->n;j++) {
		if(!(s[j] = smprint("%A",x[j])))
			goto Free;
	}
	Free: while(--j>=0) free(s[j]);
	return -1;
}

static int Afmtn(Fmt *f,int indent,array *a) {
	int *s = ashp(a), w = getw(a);
	switch(a->r) {
	case 0:  return N(f,aval(a),0);
	case 1:  return Nx1(f,aval(a),w,s[0]);
	case 2:  return NxM(f,indent,aval(a),w,s[0],s[1]);
	default: return NxMx(f,indent,aval(a),w,s,a->r);
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

static int NxM(Fmt *f, int ind, double *d, int w, int m, int n) {
	int i; for(i=0;i<m;i++) {
		if(Nx1(f, d+i*n,w,n)<0) return -1;
		if(fmtprint(f,i<m?"\n%*C":"",ind,0))
			return -1;
	}
	return 0;
}

static int NxMx(Fmt *f, int ind,double *d, int w, int *s, int r) {
	int i, o;
	if(r == 2) return NxM(f,ind,d,w,s[0],s[1]);
	for(i=o=1;i<r;i++) o *= s[i];
	for(i=0;i<s[0];i++) {
		if(NxMx(f,ind,d+(o*i),w,s+1,r-1)<0) 
			return -1;
		else if(fmtprint(f,i<s[0]-1?"\n%*c":"",ind,0)) 
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
static int geti(Fmt *f) {
	return 1 + f->to - f->start;
}
static int llen(char *s) {
	int i; for(i=0;s[i];i++)
		if(s[i] == '\n') return i-1;
	return i-1;
}
