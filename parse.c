#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include "apl.h"

int prline(int fd) {
	enum Tag c = 0;
	while(read(fd,&c,sizeof(char)) > 0 && c != TSEP) 
		write(1, &c, sizeof(char));
	print("\n");
}

int parse(int fd) {
	char c;
	double d;
	
	while(read(fd, &c, sizeof c)>0) {
		if (c == TSEP) continue;
		switch(c & TCLS) {
		case TNUM: 
			read(fd, &d, sizeof d);
			print("Number %g\n", d);
			break;
		case TCMD: 
			print("Command sub ");
			prline(fd);break;
		case TVAR:
			print("Variable ");
			prline(fd);break;
		case TSTR:
			print("String ");
			prline(fd);break;
		}
	}
	return 0;
}
