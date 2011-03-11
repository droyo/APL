#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include "apl.h"

int parse(int end) {
	int i;
	for(;end >= 0; end--) switch(tok[end].t) {
		case number: 
			print("\tnum ");
			for(i = 0; i < tok[end].n; i++)
				print("%g ", *((double*)tok[end].m + i));
			break;
		case string:
			print("\tstr %s\n", tok[end].m);
			break;
		case subcmd:
			print("\tcmd %s\n", tok[end].m);
			break;
		case identifier:
			print("\tsym %s\n", tok[end].m);
			break;
		case assign:
			print("\tset (%C)\n", *tok[end].m);
			break;
		case function:
			print("\tfn %C\n", *tok[end].m);
			break;
		default:
			print("\tdel %C\n", *tok[end].m);
			break;
	}
	return 1;
}
