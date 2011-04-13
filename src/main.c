#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "apl.h"

#define try(r,e,f,s) do{\
	if(f){fprint(2,"!"s"\n");\
	r=1; goto e;}}while(0)

int main(void) {
	Biobuf *in;
	int e = 0, quit = 0;
	array *ans, *mem, *buf, *E;
	
	try(e,e1,!(in=Bfdopen(0,O_RDONLY)),
		"Can't open input");
	try(e,e2,!(E=env(NULL)),"init env");
	try(e,e3,mem_init(E),"init memory");
	try(e,e4,!(buf=anew(E,TBOX,FSYS,1,128)),"Tokm");
	try(e,e5,!(mem=anew(E,TRAW,FSYS,1,2048)),"Mem");
	try(e,e6,fmt_init(E),"init formatter");
	try(e,e6,cst_init(E),"init constants");

	while(!quit) {
		print("\t");
		aclr(mem); aclr(buf); mem_coll(E);

		if(!scan(E,buf,mem,in)) continue;
		if(!(ans=eval(E,buf)))  continue;
		if(!(ans->f & FSIL))    print("%A\n", ans);
		else                    ans->f &= ~FSIL;
	}

	e6:free(mem);
	e5:free(buf);
	e4:mem_free(E);
	e3:env_free(E);
	e2:Bterm(in);
	e1:return e;
}
