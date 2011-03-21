#include <stdlib.h>
#include "apl.h"
#include "const.h"

int zero = 0;
array *zilde;
array *marker;

static enum codepoint utfdyadicop[] = {
	UEACH,  UDOT,       UHOOT, UHOLLER, 
	UUNION, UINTERSECT, UDFNS, UWITHE,
};
static enum codepoint utfmonadop[] = {
	UBSLASH, UBSLASHB, USLASH, 
	USLASHB, UMERGE,   USWAP
};
static enum codepoint utffunctions[] = {
	UBAR,    UCAT,     UCATBAR, UCIRC,   UDECODE,
	UDIV,    UDOMINO,  UDROP,   UENCODE, UEQUAL,
	UEXEC,   UFACT,    UFIND,   UFMT,    UFROM,
	UGEQ,    UGRDN,    UGREAT,  UGRUP,   UINDEX,
	UIOTA,   ULEQ,     ULESS,   ULINK,   ULOG,
	ULOGAND, ULOGNAND, ULOGNOR, ULOGOR,  ULTACK,
	UMATCH,  UMAX,     UMEMB,   UMIN,    UMINUS,
	UMIX,    UNEQUAL,  UPLUS,   UPOW,    UPROD,
	URAND,   UREVERSE, URHO,    UROTATE, URTACK,
	UTAKE,   UTILDE,   UTRANSP
};
static enum codepoint utfextra[] = {
	ULAMP, UASSIGN, UBRANCH, UQQUAD
};
int isapldig(long x) {
	return (x>='0' && x<='9')|| x==UMACRON;
}
int isapldel(long x) {
	return x == '(' || x == ')' || x == ':';
}
int isaplop(long x) {
	return isaplmop(x) || isapldop(x);
}
int isapldop(long x) {
	int i;for(i=0;i<NELEM(utfdyadicop);i++)
		if(x == utfdyadicop[i]) return 1;
	return 0;
}
int isaplmop(long x) {
	int i;for(i=0;i<NELEM(utfmonadop);i++)
		if(x == utfmonadop[i]) return 1;
	return 0;
}
int isaplfun(long x) {
	int i; for(i=0;i<NELEM(utffunctions);i++)
		if(x==utffunctions[i]) return 1;
	return 0;
}
int isaplchr(long x) {
	int i; for(i=0;i<NELEM(utfextra);i++)
		if(x==utfextra[i]) return 1;
	return 0;
}
int const_init(void) {
	if (!(zilde=anew(null,rdonly,0,0))) return -1;
	if (!(marker=anew(empty,rdonly,0,0))) return -1;
	return 0;
}
void const_free(void) {
	free(zilde);
	free(marker);
}
