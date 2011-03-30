#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include "apl.h"
#include "const.h"

int zero = 0;
array *zilde;
array *marker;

static Rune utfdyadicop[] = {
	UEACH,  UDOT,       UHOOT, UHOLLER, 
	UUNION, UINTERSECT, UDFNS, UWITHE, 0
};
static Rune utfmonadop[] = {
	UBSLASH, UBSLASHB, USLASH, 
	USLASHB, UMERGE,   USWAP, 0
};
static Rune utffunctions[] = {
	UBAR,    UCAT,     UCATBAR, UCIRC,   UDECODE,
	UDIV,    UDOMINO,  UDROP,   UENCODE, UEQUAL,
	UEXEC,   UFACT,    UFIND,   UFMT,    UFROM,
	UGEQ,    UGRDN,    UGREAT,  UGRUP,   UINDEX,
	UIOTA,   ULEQ,     ULESS,   ULINK,   ULOG,
	ULOGAND, ULOGNAND, ULOGNOR, ULOGOR,  ULTACK,
	UMATCH,  UMAX,     UMEMB,   UMIN,    UMINUS,
	UMIX,    UNEQUAL,  UPLUS,   UPOW,    UPROD,
	URAND,   UREVERSE, URHO,    UROTATE, URTACK,
	UTAKE,   UTILDE,   UTRANSP, 0
};
static enum codepoint utfextra[] = {
	ULAMP, UASSIGN, UBRANCH, UQQUAD
};
static int checksym(void *E, char *s, Rune r) {
	int i; Rune *c;
	array *a = get(E, s);
	if(!a) return 0;
	c = aval(a);
	for(i=0;i<a->n;i++)
		if(c[i] == r) return 1;
	return 0;
};
int isapldig(long x){return (x>='0' && x<='9')||x==UMACRON;}
int isapldel(long x){return x == '(' || x == ')' || x == ':';}
int isaplmop(long x){return checksym(G, "⎕pm", x);}
int isapldop(long x){return checksym(G, "⎕pd", x);}
int isaplfun(long x){return checksym(G, "⎕pf", x);}
int isaplchr(long x){return checksym(G, "⎕pe", x);}
int isaplop (long x){return isaplmop(x) || isapldop(x);}
int const_init(void *E) {
	int n;
	array *Func, *Dyop, *Moop, *Extra;
	
	n = NELEM(utffunctions) + 
			NELEM(utfmonadop) + 
			NELEM(utfdyadicop);
	
	if (!(zilde=anew(null,rdonly,0,0))) return -1;
	if (!(marker=anew(empty,rdonly,0,0))) return -1;
	if (!(Func=anew(string,rdonly,1,NELEM(utffunctions)))) return -1;
	if (!(Dyop=anew(string,rdonly,1,NELEM(utfdyadicop)))) return -1;
	if (!(Moop=anew(string,rdonly,1,NELEM(utfmonadop)))) return -1;
	if(!(Extra=anew(string,rdonly,1,NELEM(utfextra)))) return -1;
	
	runesprint(aval(Func), "%*R", NELEM(utffunctions), utffunctions);
	runesprint(aval(Dyop), "%*R", NELEM(utfdyadicop), utfdyadicop);
	runesprint(aval(Moop), "%*R", NELEM(utfmonadop), utfmonadop);
	runesprint(aval(Extra),"%*R", NELEM(utfextra), utfextra);
	if(!put(E,"⎕pf",Func)) return -1;
	if(!put(E,"⎕pd",Dyop)) return -1;
	if(!put(E,"⎕pm",Moop)) return -1;
	if(!put(E,"⎕pe",Extra))return -1;
	if(!put(E,"⍬",zilde))  return -1;

	return 0;
}
