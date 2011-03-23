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
	
	try(a,mem_init(),"Can't init memory");
	try(b,!(input=Bfdopen(0,O_RDONLY)),
		"Cannot open input");
	try(c,!(global_env=env_init()),"Can't init env");
	try(d,const_init(global_env),"Can't init constants");
	try(e,fmt_init(),"Can't init formatter:%r");

	while(!quit) {
		print("\t");
		mem_coll();
		ans = eval(global_env,scan(input));
		if(ans) print("%A\n",ans);
	}
	print("\nBye\n");

	e:const_free();
	d:env_free(global_env);
	c:Bterm(input);
	b:mem_free();
	a:return 0;
}
