#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "apl.h"
int quit;
void *global_env;
#define try(e,f,s) do{if(f){fprint(2,s"\n");goto e;}}while(0)

int main(void) {
	Biobuf *input;
	array *result;
	quit = 0;
	
	try(a,mem_init(),"Can't init memory");
	try(b,const_init(),"Can't init constants");
	try(c,!(input=Bfdopen(0,O_RDONLY)),
		"Cannot open input");
	try(d,!(global_env=env_init()),
		"Cannot init environment");

	while(!quit) {
		print("\t");
		fmt_reset();
		mem_coll();
		result = eval(global_env,scan(input));
		if(result) 
			print("%s(%d)\n",fmt(result),result->c);
	}
	print("\nBye\n");

	  env_free(global_env);
	d:Bterm(input);
	c:const_free();
	b:mem_free();
	a:return 0;
}
