#include <utf.h>
#include <fmt.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "apl.h"

enum boxdrawing {
  HO  = 0x2500, VE  = 0x2502,
  UR  = 0x2510, DR  = 0x2518,
  UL  = 0x250C, DL  = 0x2514
};
static int Afmt (Fmt*);
static int Afmtn(Fmt*,array*);
static int Afmts(Fmt*,array*);
static int Afmtb(Fmt*,array*);

/* Numeric, String, and Boxed arrays */
static int N   (Fmt*,double*,int);
static int Nx1 (Fmt*,double*,int,int);
static int NxM (Fmt*,double*,int,int,int);
static int NxMx(Fmt*,double*,int,int*,int);

static int Sx1 (Fmt*,Rune*,int);
static int SxM (Fmt*,Rune*,int,int);
static int SxMx(Fmt*,Rune*,int*,int);

static int B   (Fmt*,array**);
static int Bx1 (Fmt*,array**,int);
static int BxM (Fmt*,array**,int,int);
static int BxMx(Fmt*,array**,int*,int);

static int getw(array*);
static int llen(Rune*);
static int frame(Fmt*,int,Rune,Rune);
static int any(Rune**,int);
static int fmt_boxrow(Fmt*,Rune**,int*,int);
static Rune* rfind(Rune*, Rune);

int fmt_init(void) {
	return fmtinstall('A', Afmt);
}

static int Afmt(Fmt *f) {
	array *a = va_arg(f->args, array*);
	switch(a->t) {
	case function: case dydop: case monop: case niladic:
		if(a->f&primitive)
			return fmtrune(f, *(Rune*)aval(a));
		else break;
	case number: return Afmtn(f,a);
	case boxed:  return Afmtb(f,a);
	case string: return Afmts(f,a);
	case symbol: return Sx1(f,aval(a),a->n);
	case null:   return fmtprint(f,"∘");
	case empty:  return fmtprint(f,"⍝");
	case assign: return fmtprint(f,"←");
	case colon:  return fmtprint(f,":");
	case lparen: return fmtprint(f,"(");
	case rparen: return fmtprint(f,")");
	default:     return fmtrune(f,*(Rune*)aval(a));
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
static int Afmts(Fmt *f,array *a) {
	int *s = ashp(a);
	switch(a->r) {
	case 0:  return -1;
	case 1:	 return Sx1(f,aval(a),s[0]);
	case 2:  return SxM(f,aval(a),s[0],s[1]);
	default: return SxMx(f,aval(a),s,a->r);
	}
}
static int Afmtb(Fmt *f,array *a) {
	int *s = ashp(a);
	switch(a->r) {
	case 0:  return B(f,aval(a));
	case 1:  return Bx1(f,aval(a),s[0]);
	case 2:  return BxM(f,aval(a),s[0],s[1]);
	default: return BxMx(f,aval(a),s,a->r);
	}
}

static int N(Fmt *f, double *d, int w) {
	if(*d == INFINITY) return fmtprint(f, "%*s", w, "∞");
	if(*d ==-INFINITY) return fmtprint(f, "%*s", w,"-∞");
	return fmtprint(f,"%*g", w, *d);
}
static int B(Fmt *f,array **a) {
	Rune *s,*t,*l,*n; 
	int w, e=-1;
	if(!(s = runesmprint("%A",*a)))
		return -1;
	else { t = s;  w = llen(s);}
	if(frame(f,w,UL,UR)) goto Error;

	while(*t) {
		if(fmtrune(f,'\n')) goto Error;
		l = rfind(t,'\n');
		n = *l? l + 1 : l; *l = 0;
		if(fmtprint(f,"%C%S%C",VE,t,VE))
			goto Error;
		t = n;
	}
	if(fmtrune(f,'\n'))  goto Error;
	if(frame(f,w,DL,DR)) goto Error;
	e = 0;
Error:
	free(s);
	return e;
}
static int Nx1(Fmt *f, double *d, int w, int n) {
	int i;for(i=0;i<n;i++) {
		if(fmtprint(f,i?" ":"")) return -1;
		else if(N(f,d+i,w)) return -1;
	}
	return 0;
}

static int Bx1(Fmt *f, array **a, int n) {
	Rune **u,**t; int i,k,*w,e = -1;
	if(!(w = malloc(sizeof *w * n))) return -1;
	if(!(u = malloc(sizeof *u * n))) goto Error1;
	if(!(t = malloc(sizeof *t * n))) goto Error2;
	
	for(k=0;k<n;k++){
		if(!(u[k] = runesmprint("%A",a[k]))) goto Error3;
		else { t[k] = u[k]; w[k] = llen(u[k]); }
	}
	for(i=0;i<n;i++)if (frame(f,w[i],UL,UR)) goto Error3;
	while(any(t,n)) if (fmt_boxrow(f,t,w,n)) goto Error3;
	if(fmtrune(f,'\n'))                      goto Error3;
	for(i=0;i<n;i++)if (frame(f,w[i],DL,DR)) goto Error3;
	e = 0;
	Error3: while(--k>=0) free(u[k]);
	        free(t);
	Error2: free(u); 
	Error1: free(w);
	return e;
}
static int Sx1(Fmt *f, Rune *s, int n) {
	int i; for(i=0;i<n;i++)
		if(fmtrune(f,s[i])) return -1;
	return 0;
}

static int NxM(Fmt *f, double *d, int w, int m, int n) {
	int i; for(i=0;i<m;i++) {
		if(Nx1(f, d+i*n,w,n)) return -1;
		if(fmtstrcpy(f,i<m?"\n":""))
			return -1;
	}
	return 0;
}

static int SxM(Fmt *f, Rune *s, int m, int n) {
	int i; for(i=0; i<m; i++) {
		if(Sx1(f, s+i*n,n)) return -1;
		if(fmtstrcpy(f,i<m?"\n":""))
			return -1;
	}
	return 0;
}

static int BxM(Fmt *f, array **a, int m, int n) {
	int i; for(i=0;i<m;i++) {
		if(Bx1(f, a+i*n,n)) return -1;
		if(fmtstrcpy(f,i<m?"\n":""))
			return -1;
	}
	return 0;
}

static int NxMx(Fmt *f,double *d, int w, int *s, int r) {
	int i, o;
	if(r == 2) return NxM(f,d,w,s[0],s[1]);
	for(i=o=1;i<r;i++) o *= s[i];
	for(i=0;i<s[0];i++) {
		if(NxMx(f,d+(o*i),w,s+1,r-1)) 
			return -1;
		else if(fmtstrcpy(f,i<s[0]-1?"\n":"")) 
			return -1;
	}
	return 0;
}

static int BxMx(Fmt *f, array **a, int *s, int r) {
	int i, o;
	if(r == 2) return BxM(f,a,s[0],s[1]);
	for(i=o=1;i<r;i++) o *= s[i];
	for(i=0;i<s[0];i++) {
		if(BxMx(f,a+(o*i),s+1,r-1))
			return -1;
		else if(fmtstrcpy(f,i<s[0]-1?"\n":""))
			return -1;
	}
	return 0;
}

static int SxMx(Fmt *f, Rune *s, int *sh, int r) {
	int i, o;
	if(r == 2) return SxM(f,s,sh[0],sh[1]);
	for(i=o=1;i<r;i++)o *= sh[i];
	for(i=0;i<sh[0];i++) {
		if(SxMx(f,s+(o*i),sh+1,r-1))
			return -1;
		else if(fmtstrcpy(f,i<sh[0]-1?"\n":""))
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
static Rune* rfind(Rune *s, Rune r) {
	int i; for(i=0;s[i];i++)
		if(s[i] == r) break;
	return s+i;
}
static int llen(Rune *s) {
	return rfind(s,'\n') - s - 1;
}
static int frame(Fmt *f, int n, Rune b, Rune e) {
	int i;
	if(fmtrune(f,b)) return -1;
	for(i=0;i<=n;i++)if(fmtrune(f,HO)) return -1;
	return fmtrune(f,e);
}
static int any(Rune **r, int n) {
	int i; for(i=0;i<n;i++) if(*r[i]) return 1;
	return 0;
}
static int fmt_boxrow(Fmt *f, Rune **blocks, int *w, int n) {
	int i; Rune *s, *e;
	for(i=0;i<n;i++) {
		s = blocks[i];
		e = rfind(s,'\n');
		if(!i && fmtrune(f,'\n')) return -1;
		if(*e) { blocks[i] = e+1; *e = 0; }
		else blocks[i] = e;
		if(fmtprint(f,"%C%*S%C",VE,w[i]+1,s,VE)) return -1;
	}
	return 0;
}

