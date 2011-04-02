#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "apl.h"

#define try(r,e,f,s) do{\
	if(f){fprint(2,s"\n");\
	r=1; goto e;}}while(0)

void* G;
char  quit;

int main(void) {
	quit = 0;
	int e = 0;
	Biobuf *in;
	array *ans, *mem, *buf, *tok;
	
	try(e,e1,mem_init(),"Can't init memory");
	try(e,e2,!(in=Bfdopen(0,O_RDONLY)),
		"Can't open input");
	try(e,e3,!(G=shadow(0)),"Can't init env");
	try(e,e4,!(buf=anew(boxed,0,1,256)),"Tokm");
	try(e,e4,!(mem=anew(byte,0,1,2048)),"Memm");
	try(e,e4,fmt_init(),"Can't init formatter:%r");
	try(e,e4,const_init(G),"Can't init constants");
	try(e,e4,sample_init(G),"Can't init samples");

	aclr(mem); aclr(buf);
	incref(mem); incref(buf);
	while(!quit) {
		print("\t");
		aclr(mem); aclr(buf); mem_coll();
		tok = scan(in,buf,mem);
		if(!tok) continue;
		ans = eval(G,tok);
		if(ans && !(ans->f & quiet))
			print("%A\n", ans);
		else if(ans)
			ans->f &= ~quiet;
	}
	print("\nBye\n");

	e4:env_free(G);
	e3:Bterm(in);
	e2:mem_free();
	e1:return e;
}
