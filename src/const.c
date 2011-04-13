#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include "apl.h"
#include "const.h"

array *zilde  = NULL;
array *marker = NULL;

static const Rune prim[] = {
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
	UALPHA,  UOMEGA,   UJOT,0
};

int cst_init(array *E) {
	array *aplch, *delim, *digit;
	
	if (!(zilde=anew(E,TNIL,FRDO,0,0)))  return -1;
	if (!(marker=anew(E,TEND,FRDO,0,0))) return -1;
	if (!(delim=astr(E,"()[]:")))        return -1;
	if (!(digit=astr(E,"¯0123456789")))  return -1;
	if (!(aplch=anew(E,TSTR,FRDO,1,NELEM(prim))))
		return -1;
	else runesprint(aval(aplch),"%*R",NELEM(prim), prim);

	if(!put(E,"⎕PRICH",aplch)) return -1;
	if(!put(E,"⎕DELIM",delim)) return -1;
	if(!put(E,"⎕DIGIT",digit)) return -1;
	if(!put(E,"⍬",zilde))      return -1;

	return 0;
}
