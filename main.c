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
	array *result;
	quit = 0;
	
	if(mem_init()) {
		fprint(2, "Cannot init memory manager.\n");
		exit(1);
	}
	if(const_init()) {
		fprint(2, "Cannot init constants\n");
		exit(1);
	}
	if(!(input = Bfdopen(1, O_RDONLY))) {
		fprint(2, "Cannot open input file\n");
		exit(1);
	}
	if(!(global_env = env_init())) {
		fprint(2, "Cannot init environment\n");
		exit(1);
	}
	while(!quit) {
		print("\t");
		result = eval(global_env,scan(input));
		if(result) {
			disp(result); print("(%d)\n", result->c);
			mem_coll();
		}
	}
	print("\nBye\n");
	Bterm(input);
	env_free(global_env);
	const_free();
	mem_free();
	return 0;
}
