#include <utf.h>
#include <fmt.h>
#include <math.h>
#include "apl.h"

void disp(array *a) {
	int i;
	double *d;
	switch(a->t) {
	case assign:
		print("←");
		break;
	case number: 
		d = aval(a);
		for(i = 0; i < a->n; i++) {
			print(i?" ":"");
			if(d[i] == INFINITY)
				print("∞");
			else if(d[i] == -INFINITY)
				print("-∞");
			else print("%g",d[i]);
		}
		break;
	case string:
		print("`%s'", (char*)aval(a));
		break;
	case empty: print("⍝"); break;
	case null: print("∘"); break;
	case primitive: case dydop: case monop:
		if(a->n==1) {
			print("%C", *(Rune*)a->m);
			break;
		}
	default: print("%s", (char*)aval(a)); break;
	case lparen: case rparen: case colon: break;
	}
}

