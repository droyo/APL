#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "apl.h"
int quit;
void *global_env;

int main(void) {
	Biobuf *input;
	quit = 0;
	
	if(!(input = Bfdopen(1, O_RDONLY))) {
		fprint(2, "Cannot open input file\n");
		exit(1);
	}
	if(!(global_env = init_env())) {
		fprint(2, "Cannot init environment\n");
		exit(1);
	}
	while(!quit) {
		print("\t");
		parse(global_env,scan(input));
	}
	print("\nBye\n");
	env_free(global_env);
	Bterm(input);
	return 0;
}
