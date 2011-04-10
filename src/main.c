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
void* S;
char  quit;

int main(void) {
	quit = 0;
	int e = 0;
	Biobuf *in;
	array *ans, *mem, *buf, *tok;
	
	try(e,e1,mem_init(),"Can't init memory");
	try(e,e2,!(in=Bfdopen(0,O_RDONLY)),
		"Can't open input");
	try(e,e3,!(G=env(NULL)),"Can't init env");
	try(e,e3,!(S=env(NULL)),"Can't init sysenv");
	try(e,e3,!(buf=anew(TBOX,0,1,128)),"Tokm");
	try(e,e3,!(mem=anew(TRAW,0,1,2048)),"Memm");
	try(e,e3,fmt_init(),"Can't init formatter:%r");
	try(e,e3,const_init(),"Can't init constants");

	aclr(mem); aclr(buf);
	try(e,e3,!put(S,"⎕tokbuf",buf),"Can't bind token buffer");
	try(e,e3,!put(S,"⎕membuf",mem),"Can't bind lex mem buffer");
	try(e,e3,!put(S,"⎕G",G),"Can't bind global env table");
	try(e,e3,!put(S,"⎕S",S),"Can't bind global sys table");

	while(!quit) {
		print("\t");
		aclr(mem); aclr(buf); mem_coll();
		tok = scan(in,&buf,&mem);
		if(!tok) continue;
		ans = eval(G,tok);
		if(ans && !(ans->f & FSIL))
			print("%A\n", ans);
		else if(ans)
			ans->f &= ~FSIL;
	}
	print("\nBye\n");

	e3:Bterm(in);
	e2:mem_free();
	e1:return e;
}
