#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include "apl.h"
#include "const.h"

array *zilde;
array *marker;

static char utfdelim[] = "()[]";
static const Rune utfspecial[] = {
	UBAR,    UCAT,     UCATBAR, UCIRC,   UDECODE,
	UDIV,    UDOMINO,  UDROP,   UENCODE, UEQUAL,
	UEVAL,   UFACT,    UFIND,   UFMT,    UFROM,
	UGEQ,    UGRDN,    UGREAT,  UGRUP,   UINDEX,
	UIOTA,   ULEQ,     ULESS,   ULINK,   ULOG,
	ULOGAND, ULOGNAND, ULOGNOR, ULOGOR,  ULTACK,
	UMATCH,  UMAX,     UMEMB,   UMIN,    USUB,
	UTRANSP, UNEQUAL,  UADD,    UPOW,    UMUL,
	URAND,   UREV,     URHO,    UROTATE, URTACK,
	UTAKE,   UTILDE,   UWITHE,  UASSIGN, UBRANCH,
	UQQUAD,  USLASHB,  UMERGE,  USWAP,   UHOLLER, 
	UEACH,   UDOT,     UHOOT,   UBSLASH, UBSLASHB,
	USLASH,  UUNION,   UISECT,  UDFNS,   ULAMP,
	UALPHA,  UOMEGA,   UJOT
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
int isapldig(Rune x){return (x>='0' && x<='9')||x==UMACRON;}
int isapldel(Rune x){return checksym("⎕dl", x);}
int isaplchr(Rune x){return checksym("⎕pc", x);}

int const_init(void) {
	array *Aplch, *Delim;
	
	if (!(zilde=anew(TNIL,FRDO,0,0))) return -1;
	if (!(marker=anew(TEND,FRDO,0,0))) return -1;
	if (!(Aplch=anew(TSTR,FRDO,1,NELEM(utfspecial)))) return -1;
	if (!(Delim=astr(utfdelim))) return -1;
	
	runesprint(aval(Aplch),"%*R",NELEM(utfspecial), utfspecial);
	if(!put(S,"⎕pc",Aplch)) return -1;
	if(!put(S,"⎕dl",Delim)) return -1;
	if(!put(G,"⍬",zilde))   return -1;

	return 0;
}
