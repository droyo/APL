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
	input = Bfdopen(0, O_RDONLY);

	while(!scan(input, chan[1]))
		eval(chan[0]);

Cleanup:
	Bterm(input);
	return 0;
}
