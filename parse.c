#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include "apl.h"

int parse(int end) {
	int i;
	char *s;
	Rune *r;
	double *d;

	for(;end >= 0; end--) switch(tok[end].t) {
		case number: 
			print("\tnum ");
			d = tok[end].m;
			for(i = 0; i < tok[end].n; i++)
				print("%g ", d[i]);
			print("\n");
			break;
		case string:
			s = tok[end].m;
			print("\tstr %s\n", s);
			break;
		case subcmd:
			s = tok[end].m;
			print("\tcmd %s\n", s);
			break;
		case identifier:
			s = tok[end].m;
			print("\tsym %s\n", s);
			break;
		case assign:
			r = tok[end].m;
			print("\tset (%C)\n", *r);
			break;
		case function:
			r = tok[end].m;
			print("\tfn %C\n", *r);
			break;
		default:
			r = tok[end].m;
			print("\tdel %C\n", *r);
			break;
	}
	return 1;
}
