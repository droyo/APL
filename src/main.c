#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include "apl.h"

#define try(f,s) do{if(f){fprint(2,"!"s"\n");return 1; }}while(0)

int main(void) {
	void *E = NULL;
	Biobuf *in;
	int quit = 0;
	array *ans, *mem, *buf;
	
	try(!(in=Bfdopen(0,O_RDONLY)),"Can't open input");
	try(!(E=env(NULL)),"init env");
	try(!(buf=anew(E,TBOX,FSYS,1,128)),"Tokm");
	try(!(mem=anew(E,TRAW,FSYS,1,2048)),"Mem");
	try(fmt_init(E),"init formatter");
	try(cst_init(E),"init constants");

	while(!quit) {
		print("\t");
		aclr(mem); aclr(buf);

		if(!scan(E,buf,mem,in)) continue;
		if(!(ans=eval(E,buf)))  continue;
		if(!(ans->f & FSIL))    print("%A\n", ans);
		else                    ans->f &= ~FSIL;
	}

	Bterm(in);
	return 0;
}
