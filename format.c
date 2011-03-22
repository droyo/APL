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
static int Afmts(Fmt*,array*);
static int Afmtb(Fmt*,array*);

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
	default:     return Afmts(f,a);
	}
	return 0;
}
static int Afmtn(Fmt *f,array *a){
	int i; double *d = aval(a);
	char buf[8]; int max,n;
	for(i=0,max=2;i<a->n;i++) {
		n=snprint(buf,sizeof buf,"%g",d[i]);
		max = max < n ? n : max;
	}
	for(i=0;i<a->n;i++) {
		fmtprint(f,i?" ":"");
		if(d[i]== INFINITY)
			fmtprint(f, "%*s", max, "∞");
		else if(d[i]==-INFINITY)
			fmtprint(f,"%*s", max, "-∞");
		else fmtprint(f,"%*g", max, d[i]);
	}
	return 0;
}
static int Afmts(Fmt *f, array *a) {
	return fmtstrcpy(f,aval(a));
}
static int Afmtb(Fmt *f, array *a) {
	return 0;
}
int fmt_init(void) {
	return fmtinstall('A', Afmt);
}
