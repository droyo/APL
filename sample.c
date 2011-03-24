#include <utf.h>
#include <fmt.h>
#include "const.h"
#include "apl.h"

int sample_init(void *E) {
	Rune *r;
	int *s,i; double *d;
	array *NN, *NNN, *NNNN;
	array *SS, *SSS, *SSSS;

	if (!(NN=anew(number,0,2,12))) return -1;
	if (!(NNN=anew(number,0,3,32))) return -1;
	if (!(NNNN=anew(number,0,4,16))) return -1;
	if (!(SS=anew(string,0,2,12))) return -1;
	if (!(SSS=anew(string,0,3,32))) return -1;
	if (!(SSSS=anew(string,0,4,16))) return -1;

	s = ashp(NN); d = aval(NN);
	s[0] = 2; s[1] = 6;
	for(i=0;i<NN->n;i++) d[i] = i;
	if(!put(E, "A", NN)) return -1;

	s = ashp(NNN); d = aval(NNN);
	s[0] = 2; s[1] = 4; s[2] = 4;
	for(i=0;i<NNN->n;i++) d[i] = i;
	if(!put(E, "B", NNN)) return -1;

	s = ashp(NNNN); d = aval(NNNN);
	s[0] = s[1] = s[2] = s[3] = 2;
	for(i=0;i<NNNN->n;i++) d[i] = i;
	if(!put(E, "C", NNNN)) return -1;

	s = ashp(SS); r = aval(SS);
	s[0] = 2; s[1] = 6;
	for(i=0;i<SS->n;i++) r[i] = UHOOT;
	if(!put(E, "D", SS)) return -1;
	
	s = ashp(SSS); r= aval(SSS);
	s[0] = 2; s[1] = 4; s[2] = 4;
	for(i=0;i<SSS->n;i++) r[i] = UHOOT;
	if(!put(E, "E", SSS)) return -1;
	
	s = ashp(SSSS); r = aval(SSSS);
	s[0] = s[1] = s[2] = s[3] = 2;
	for(i=0;i<SSSS->n;i++) r[i] = UHOOT;
	if(!put(E, "F", SSSS)) return -1;

	return 0;
}
