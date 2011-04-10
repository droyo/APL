#include <utf.h>
#include <fmt.h>
#include "const.h"
#include "apl.h"

int sample_init(void *E) {
	Rune *r; array **a;
	int *s,i; double *d;
	array *NN, *NNN, *NNNN;
	array *SS, *SSS, *SSSS;
	array *B, *BB, *BBB;

	if (!(NN=anew(TNUM,0,2,12))) return -1;
	if (!(NNN=anew(TNUM,0,3,32))) return -1;
	if (!(NNNN=anew(TNUM,0,4,16))) return -1;
	if (!(SS=anew(TSTR,0,2,12))) return -1;
	if (!(SSS=anew(TSTR,0,3,32))) return -1;
	if (!(SSSS=anew(TSTR,0,4,16))) return -1;
	if (!(B=anew(TBOX,0,0,1))) return -1;
	if (!(BB=anew(TBOX,0,0,1))) return -1;
	if (!(BBB=anew(TBOX,0,2,4))) return -1;

	s = ashp(NN); d = aval(NN);
	s[0] = 2; s[1] = 6;
	for(i=0;i<NN->n;i++) d[i] = i;
	if(!put(E, "⎕A", NN)) return -1;

	s = ashp(NNN); d = aval(NNN);
	s[0] = 2; s[1] = 4; s[2] = 4;
	for(i=0;i<NNN->n;i++) d[i] = i;
	if(!put(E, "⎕B", NNN)) return -1;

	s = ashp(NNNN); d = aval(NNNN);
	s[0] = s[1] = s[2] = s[3] = 2;
	for(i=0;i<NNNN->n;i++) d[i] = i;
	if(!put(E, "⎕C", NNNN)) return -1;

	s = ashp(SS); r = aval(SS);
	s[0] = 2; s[1] = 6;
	for(i=0;i<SS->n;i++) r[i] = UHOOT;
	if(!put(E, "⎕D", SS)) return -1;
	
	s = ashp(SSS); r= aval(SSS);
	s[0] = 2; s[1] = 4; s[2] = 4;
	for(i=0;i<SSS->n;i++) r[i] = UHOOT;
	if(!put(E, "⎕E", SSS)) return -1;
	
	s = ashp(SSSS); r = aval(SSSS);
	s[0] = s[1] = s[2] = s[3] = 2;
	for(i=0;i<SSSS->n;i++) r[i] = UHOOT;
	if(!put(E, "⎕F", SSSS)) return -1;

	a = aval(B); a[0] = get(E,"⎕C");
	if(!put(E, "⎕G", B)) return -1;

	a = aval(BB); a[0] = get(E, "⎕A");
	if(!put(E, "⎕H", BB)) return -1;

	a = aval(BBB); 
	s = ashp(BBB); s[0] = s[1] = 2;
	a[0] = a[3] = get(E,"⎕H"); 
	a[1] = a[2] = get(E,"⎕D");
	if(!put(E, "⎕I", BBB)) return -1;

	return 0;
}

