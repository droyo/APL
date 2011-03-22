#include <utf.h>
#include <fmt.h>
#include <math.h>
#include <stdarg.h>
#include "apl.h"

static int Afmt(Fmt*);
static int Afmtn(Fmt*,array*);

static int Afmt(Fmt *f) {
	array *a = va_arg(f->args, array*);
	switch(a->t) {
	case function: case dydop: case monop: case niladic:
		if(a->f&primitive)
			return fmtrune(f, *(Rune*)aval(a));
		else goto string;
	case number:
		return Afmtn(f,a);
	default:string: 
		return fmtstrcpy(f,aval(a));
	}
}
static int Afmtn(Fmt *f,array *a){
	int i; double *d = aval(a);
	for(i=0;i<a->n;i++) {
		fmtprint(f,i?" ":"");
		if(d[i]== INFINITY)fmtprint(f, "∞"); else
		if(d[i]==-INFINITY)fmtprint(f,"-∞"); else
		fmtprint(f,"%g", d[i]);
	}
	return 0;
}
int fmt_init(void) {
	return fmtinstall('A', Afmt);
}
