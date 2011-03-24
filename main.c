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
	array *ans;
	quit = 0;
	
	try(e1,mem_init(),"Can't init memory");
	try(e2,!(input=Bfdopen(0,O_RDONLY)),
		"Cannot open input");
	try(e3,!(global_env=env_init()),"Can't init env");
	try(e4,const_init(global_env),"Can't init constants");
	try(e4,sample_init(global_env),"Can't init samples");
	try(e4,fmt_init(),"Can't init formatter:%r");

	while(!quit) {
		print("\t");
		mem_coll();
		ans = eval(global_env,scan(input));
		if(ans) print("%A\n",ans);
	}
	print("\nBye\n");

	e4:env_free(global_env);
	e3:Bterm(input);
	e2:mem_free();
	e1:return 0;
}
