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
		d = val(a);
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
		print("`%s'", (char*)val(a));
		break;
	case subcmd:
		print("$(%s)", (char*)val(a));
		break;
	case empty:
		print("⍝"); 
		break;
	default:
		print("%s", (char*)val(a));
	}
}

