#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "apl.h"
int quit;
int main(void) {
	int chan[2];
	Biobuf *input;
	quit = 0;
	
	if(pipe(chan)) {
		fprint(2, "Pipe error\n");
		exit(1);
	} else {
		fcntl(chan[0], F_SETFL, O_NONBLOCK);
		fcntl(chan[1], F_SETFL, O_NONBLOCK);
	}
	input = Bfdopen(0, O_RDONLY);

	while(!quit) {
		print("\t");
		parse(scan(input));
	}
	print("\nBye\n");
	Bterm(input);
	return 0;
}
