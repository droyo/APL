#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include "apl.h"

int parse(int end) {
	for(;end >= 0; end--) switch(tok[end].t) {
		case number: 
			print("\tnum %g\n", tok[end].v.d);
			break;
		case string:
			print("\tstr %s\n", tok[end].v.s);
			break;
		case subcmd:
			print("\tcmd %s\n", tok[end].v.s);
			break;
		case identifier:
			print("\tsym %s\n", tok[end].v.s);
			break;
		case assign:
			print("\tset (%C)\n", tok[end].v.p);
			break;
		case function:
			print("\tfn %s\n", tok[end].v.s);
			break;
		default:
			print("\tdel %C\n", tok[end].v.s);
			break;
	}
	return 1;
}
