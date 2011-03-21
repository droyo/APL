#include <utf.h>
#include <fmt.h>
#include <math.h>
#include <stdlib.h>
#include "apl.h"

static void disp_number(array *);

void disp(array *a) {
	switch(a->t) {
	case assign:   print("←");       break;
	case number:   disp_number(a);   break;
	case empty:    print("⍝");       break;
	case null:     print("∘");       break;
	case lparen:case rparen:case colon: break;
	case dydop:case monop:case function:
		if(a->f&primitive)
			print("%C", *(Rune*)aval(a));
		else
			print("%s", (char*)aval(a));break;
	default: print("%s", (char*)aval(a));
	}
}

static void disp_number(array *a) {
	int i;
	double *d = aval(a);
	for(i = 0; i < a->n; i++) {
		print(i?" ":"");
		if(d[i] == INFINITY)
			print("∞");
		else if(d[i] == -INFINITY)
			print("-∞");
		else print("%g",d[i]);
	}
}
