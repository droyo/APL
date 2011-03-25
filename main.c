#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "apl.h"
#define try(e,f,s) do{if(f){fprint(2,s"\n");goto e;}}while(0)

int quit;
void *G;

int main(void) {
	int x;
	Biobuf *input;
	char buf[1024];
	array *tok[128], **r[2], *ans;
	quit = 0;
	
	try(e1,mem_init(),"Can't init memory");
	try(e2,!(input=Bfdopen(0,O_RDONLY)),
		"Cannot open input");
	try(e3,!(G=shadow(0)),"Can't init env");
	try(e4,const_init(G),"Can't init constants");
	try(e4,sample_init(G),"Can't init samples");
	try(e4,fmt_init(),"Can't init formatter:%r");

	while(!quit) {
		print("\t");
		mem_coll();
		x = scan(input,tok,NELEM(tok),buf,NELEM(buf));
		if(x < 0) {
			fprint(2, "Lex error\n");
			continue;
		}
		r[0] = tok; r[1] = tok + x;
		if((ans=eval(G,r))) print("%A\n",ans);
	}
	print("\nBye\n");

	e4:env_free(G);
	e3:Bterm(input);
	e2:mem_free();
	e1:return 0;
}
