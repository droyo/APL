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
static int Afmtn(Fmt*,int,array*);
static int Afmts(Fmt*,int,array*);
static int Afmtb(Fmt*,int,array*);

/* Numeric, String, and Boxed arrays */
static int N   (Fmt*,double*,int);
static int Nx1 (Fmt*,double*,int,int);
static int NxM (Fmt*,int,double*,int,int,int);
static int NxMx(Fmt*,int,double*,int,int*,int);

static int Sx1 (Fmt*,Rune*,int);
static int SxM (Fmt*,int,Rune*,int,int);
static int SxMx(Fmt*,int,Rune*,int*,int);

static int B   (Fmt*,int,array*);
static int Bx1 (Fmt*,int,array*,int);
static int BxM (Fmt*,int,array*,int,int);
static int BxMx(Fmt*,int,array*,int*,int);

static int getw(array*);
static int geti(Fmt*);
static int llen(Rune*);
static int frame(Fmt*,int,Rune,Rune);
static int any(Rune**,int);

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
	case string: return Afmts(f,i,a);
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

static int Afmtn(Fmt *f,int indent,array *a) {
	int *s = ashp(a), w = getw(a);
	switch(a->r) {
	case 0:  return N(f,aval(a),0);
	case 1:  return Nx1(f,aval(a),w,s[0]);
	case 2:  return NxM(f,indent,aval(a),w,s[0],s[1]);
	default: return NxMx(f,indent,aval(a),w,s,a->r);
	}
}
static int Afmts(Fmt *f,int indent,array *a) {
	int *s = ashp(a);
	switch(a->r) {
	case 0:  return -1;
	case 1:	 return Sx1(f,aval(a),s[0]);
	case 2:  return SxM(f,indent,aval(a),s[0],s[1]);
	default: return SxMx(f,indent,aval(a),s,a->r);
	}
}
static int Afmtb(Fmt *f,int indent,array *a) {
	int *s = ashp(a);
	switch(a->r) {
	case 0:  return B(f,indent,aval(a));
	case 1:  return Bx1(f,indent,aval(a),s[0]);
	case 2:  return BxM(f,indent,aval(a),s[0],s[1]);
	default: return BxMx(f,indent,aval(a),s,a->r);
	}
}

static int N(Fmt *f, double *d, int w) {
	if(*d == INFINITY) return fmtprint(f, "%*s", w, "∞");
	if(*d ==-INFINITY) return fmtprint(f, "%*s", w,"-∞");
	return fmtprint(f,"%*g", w, *d);
}
static int B(Fmt *f, int ind, array *a) {
	Rune *s; int i;
	if(!(s = runesmprint("%*c%A", ind,0,a)))
		return -1;
	if(frame(f,llen(s),UL,UR)) goto Error;
	if(fmtprint(f,"\n%*c%C",ind,0,VE)) return -1;
	for(i=0;s[i];i++) {
		if(s[i]=='\n'&&fmtprint(f,"%C\n%*c%C",VE,ind,0,VE))
			goto Error;
		else if(fmtprint(f,"%C", s[i]))
			goto Error;
	}
	if(frame(f,i?i-1:i,DL,DR)) goto Error;
	free(s);
	return 0;
	
	Error: free(s);
	return -1;
}
static int Nx1(Fmt *f, double *d, int w, int n) {
	int i;for(i=0;i<n;i++) {
		if(fmtprint(f,i?" ":"")) return -1;
		else if(N(f,d+i,w)) return -1;
	}
	return 0;
}

static int Bx1(Fmt *f, int ind, array *a, int n) {
	Rune **u,**t,*s; int i,j,k,*w,r = -1;
	if(!(u = malloc(sizeof *u * n))) return -1;
	if(!(t = malloc(sizeof *t * n))) goto Error1;
	if(!(w = malloc(sizeof *w * n))) goto Error2;
	
	for(k=0;k<n;k++){
		if(!(u[k] = runesmprint("%A",a+k))) 
			goto Error3;
		else { t[k] = u[k]; w[k] = llen(u[k]); }
	}
	for(i=0;i<n;i++)
		if(frame(f,w[i],UL,UR))     goto Error3;
	if(fmtprint(f,"\n%*c",ind,0)) goto Error3;
	
	while(any(t,n)) for(i=0;i<n;i++) {
		s = t[i];
		if(!i && fmtprint(f,"%*c",ind,0))   goto Error3;
		if(!*s&&w[i]<0) {
			if(frame(f,-w[i],DL,DR)) goto Error3;
			else w[i] = -w[i];
		}
		if(!*s&&fmtprint(f,"%*c",w[i]+2,0)) goto Error3;
		for(j=0;s[j];j++) switch(s[j+1]) {
			case '\n': 
				t[i] = s+j+2;
				s[j+1] = 0;
				if(fmtprint(f,"%C%S%C",VE,s,VE))goto Error3;
				break;
			case '\0':
				t[i] = s+j+1;
				if(fmtprint(f,"%C%S%C",VE,s,VE))goto Error3;
				w[i] = -w[i];
				break;
			default: break;
		}
		if(i==n-1 && fmtstrcpy(f,"\n"))     goto Error3;
	}
	r = 0;
	Error3: while(--k>=0) free(u[k]);
	        free(w);
	Error2: free(u); 
	Error1: free(t);
	return r;
}
static int Sx1(Fmt *f, Rune *s, int n) {
	int i; for(i=0;i<n;i++)
		if(fmtrune(f,s[i])) return -1;
	return 0;
}

static int NxM(Fmt *f, int ind, double *d, int w, int m, int n) {
	int i; for(i=0;i<m;i++) {
		if(Nx1(f, d+i*n,w,n)) return -1;
		if(fmtprint(f,i<m?"\n%*C":"",ind,0))
			return -1;
	}
	return 0;
}

static int SxM(Fmt *f, int ind, Rune *s, int m, int n) {
	int i; for(i=0; i<m; i++) {
		if(Sx1(f, s+i*n,n)) return -1;
		if(fmtprint(f,i<m?"\n%*C":"",ind,0))
			return -1;
	}
	return 0;
}

static int BxM(Fmt *f, int ind, array *a, int m, int n) {
	return 0;
}

static int BxMx(Fmt *f, int ind, array *a, int *s, int r) {
	return 0;
}

static int NxMx(Fmt *f, int ind,double *d, int w, int *s, int r) {
	int i, o;
	if(r == 2) return NxM(f,ind,d,w,s[0],s[1]);
	for(i=o=1;i<r;i++) o *= s[i];
	for(i=0;i<s[0];i++) {
		if(NxMx(f,ind,d+(o*i),w,s+1,r-1)) 
			return -1;
		else if(fmtprint(f,i<s[0]-1?"\n%*c":"",ind,0)) 
			return -1;
	}
	return 0;
}
static int SxMx(Fmt *f, int ind, Rune *s, int *sh, int r) {
	int i, o;
	if(r == 2) return SxM(f,ind,s,sh[0],sh[1]);
	for(i=o=1;i<r;i++)o *= sh[i];
	for(i=0;i<sh[0];i++) {
		if(SxMx(f,ind,s+(o*i),sh+1,r-1))
			return -1;
		else if(fmtprint(f,i<sh[0]-1?"\n%*c":"",ind,0))
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
static int llen(Rune *s) {
	int i; for(i=0;s[i];i++) 
		if(s[i] == '\n') break;
	return i-1;
}
static int frame(Fmt *f, int n, Rune b, Rune e) {
	int i;
	if(fmtrune(f,b)) return -1;
	for(i=0;i<n;i++)if(fmtrune(f,HO)) return -1;
	return fmtrune(f,e);
}
static int any(Rune **r, int n) {
	int i; for(i=0;i<n;i++) if(*r[i]) return 1;
	return 0;
}
