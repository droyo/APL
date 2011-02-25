#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include "apl.h"

/* Careful here of UTF8 */
int prline(int fd, int delim) {
	char c;
	
	for(c=0; c!=TSEP; read(fd,&c,sizeof c))
		write(1, &c, sizeof c);
	print("\n");
}

int eval(int fd) {
	char c;
	char *s;
	double d;
	
	while(read(fd, &c, sizeof c)>0) {
		if (c == TSEP) continue;
		if ((c & TCLASS) == TPRIMITIVE) {
			print("Special ");
			switch(c &TPRITYPE) {
			case TDELIMITER:
				print("Delim ");
				switch(c &TDATA) {
					case DOPENBRACE: print("{"); break;
					case DOPENPAREN: print("("); break;
					case DOPENBRACK: print("["); break;
					case DCLOSEBRACE: print("}"); break;
					case DCLOSEPAREN: print(")"); break;
					case DCLOSEBRACK: print("]"); break;
				}
				print("\n");
				break;
			case TFUNCTION:
				print("Type: function\n");
				break;
			}
		} else switch(c & TVALTYPE) {
			case TNUMBER: 
				read(fd, &d, sizeof d);
				print("Literal number %g\n", d);
				break;
			case TQEXEC: 
				print("Command sub ");
				prline(fd,TSEP);break;
			case TNAME:
				print("Variable ");
				prline(fd,TSEP);break;
			default:
				print("String ", s);
				prline(fd, TSEP);
		}
	}
	return 0;
}
