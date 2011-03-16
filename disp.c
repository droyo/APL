#include <utf.h>
#include <fmt.h>
#include "apl.h"

void disp(array *a) {
	int i;
	double *d;
	switch(a->t) {
	case number: 
		d = val(a);
		for(i = 0; i < a->n; i++)
			print("%s%g",i?" ":"", d[i]);
		break;
	case string:
		print("`%s'", (char*)val(a));
		break;
	case subcmd:
		print("$(%s)", (char*)val(a));
		break;
	case empty:
		print("§"); 
		break;
	default:
		print("%s", (char*)val(a));
	}
}

