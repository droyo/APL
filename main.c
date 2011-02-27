#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <unistd.h>
#include "apl.h"

int main(void) {
	Rune r;
	int chan[2];

	Biobuf *input;
	
	if(pipe2(chan, O_NONBLOCK)) {
		fprint(2, "Pipe error\n");
		exit(1);
	}
	if(init_parser()) {
		fprint(2, "Could not init parser\n");
		return 1;
	}
	input = Bfdopen(0, O_RDONLY);

	print("\t");
	while(!scan(input, chan[1])) {
		parse(chan[0]);
		print("\t");
	}

Cleanup:
	cleanup_parser();
	Bterm(input);
	return 0;
}
