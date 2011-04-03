#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include "apl.h"
#include "const.h"

array *zilde;
array *marker;

static char utfdelim[] = "()[]";
static const Rune utfdyadicop[] = {
	UEACH,  UDOT,   UHOOT, UHOLLER, 
	UUNION, UISECT, UDFNS, UWITHE
};
static const Rune utfmonadop[] = {
	UBSLASH, UBSLASHB, USLASH, 
	USLASHB, UMERGE,   USWAP
};
static const Rune utffunctions[] = {
	UBAR,    UCAT,     UCATBAR, UCIRC,   UDECODE,
	UDIV,    UDOMINO,  UDROP,   UENCODE, UEQUAL,
	UEXEC,   UFACT,    UFIND,   UFMT,    UFROM,
	UGEQ,    UGRDN,    UGREAT,  UGRUP,   UINDEX,
	UIOTA,   ULEQ,     ULESS,   ULINK,   ULOG,
	ULOGAND, ULOGNAND, ULOGNOR, ULOGOR,  ULTACK,
	UMATCH,  UMAX,     UMEMB,   UMIN,    UMINUS,
	UTRANSP, UNEQUAL,  UPLUS,   UPOW,    UPROD,
	URAND,   UREVERSE, URHO,    UROTATE, URTACK,
	UTAKE,   UTILDE
};
static const Rune utfextra[] = {
	ULAMP, UASSIGN, UBRANCH, UQQUAD
};
	
static int checksym(char *s, Rune r) {
	int i; Rune *c;
	array *a = get(S,s);
	if(!a) return 0;
	c = aval(a);
	for(i=0;i<a->n;i++)
		if(c[i] == r) return 1;
	return 0;
};
int isapldig(long x){return (x>='0' && x<='9')||x==UMACRON;}
int isapldel(long x){return checksym("⎕dl", x);}
int isaplmop(long x){return checksym("⎕pm", x);}
int isapldop(long x){return checksym("⎕pd", x);}
int isaplfun(long x){return checksym("⎕pf", x);}
int isaplchr(long x){return checksym("⎕pe", x);}
int isaplop (long x){return isaplmop(x) || isapldop(x);}

int const_init(void) {
	array *Func, *Dyop, *Moop, *Extra, *Delim;
	
	if (!(zilde=anew(null,rdonly,0,0))) return -1;
	if (!(marker=anew(empty,rdonly,0,0))) return -1;
	if (!(Func=anew(string,rdonly,1,NELEM(utffunctions)))) return -1;
	if (!(Dyop=anew(string,rdonly,1,NELEM(utfdyadicop)))) return -1;
	if (!(Moop=anew(string,rdonly,1,NELEM(utfmonadop)))) return -1;
	if(!(Extra=anew(string,rdonly,1,NELEM(utfextra)))) return -1;
	if(!(Delim=astr(utfdelim))) return -1;
	
	runesprint(aval(Func), "%*R", NELEM(utffunctions), utffunctions);
	runesprint(aval(Dyop), "%*R", NELEM(utfdyadicop), utfdyadicop);
	runesprint(aval(Moop), "%*R", NELEM(utfmonadop), utfmonadop);
	runesprint(aval(Extra),"%*R", NELEM(utfextra), utfextra);
	if(!put(S,"⎕pf",Func)) return -1;
	if(!put(S,"⎕pd",Dyop)) return -1;
	if(!put(S,"⎕pm",Moop)) return -1;
	if(!put(S,"⎕pe",Extra))return -1;
	if(!put(S,"⎕dl",Delim))return -1;
	if(!put(G,"⍬",zilde))  return -1;

	return 0;
}
