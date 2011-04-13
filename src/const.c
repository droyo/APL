#include <utf.h>
#include <fmt.h>
#include <stdlib.h>
#include "apl.h"
#include "const.h"

array *zilde  = NULL;
array *marker = NULL;


int cst_init(void *E) {
	array *aplch, *delim, *digit;
	char digits[] = "¯0123456789";
	char primitives[] = 
		"|,⍪○⊥÷⌹↓⊤=⍎!⍷⍕{≥⍒>⍋⍸⍳≤<⊃⍟^⍲⍱"
		"∨⊣≡⌈∊⌊-⍉≠+*×?⌽⍴⊖⊢↑~⍩←→⍞⌿}⊂⍥¨"
		".⍤\\⍀/∪∩∇⍝⍺⍵∘";
	
	if (!(zilde=anew(E,TNIL,FRDO,0,0)))  return -1;
	if (!(marker=anew(E,TEND,FRDO,0,0))) return -1;
	if (!(delim=astr(E,"()[]:")))        return -1;
	if (!(digit=astr(E,digits)))         return -1;
	if (!(aplch=astr(E,primitives)))     return -1;

	if(!put(E,"⎕DELIM",delim)) return -1;
	if(!put(E,"⎕PRIM",aplch))  return -1;
	if(!put(E,"⎕DIGIT",digit)) return -1;
	if(!put(E,"⍬",zilde))      return -1;

	return 0;
}
