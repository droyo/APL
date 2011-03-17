#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "apl.h"
int quit;

int main(void) {
	Biobuf *input;
	quit = 0;
	
	if(!(input = Bfdopen(1, O_RDONLY))) {
		fprint(2, "Cannot open input file\n");
		exit(1);
	}
	while(!quit) {
		print("\t");
		parse(scan(input));
	}
	print("\nBye\n");
	Bterm(input);
	return 0;
}
