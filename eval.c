#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <string.h>
#include <errno.h>
#include "apl.h"

int slurp(int fd, char *buf, int n, int sep) {
	int i;
	for(i=0;i<n&&read(fd,buf,sizeof *buf)>0; i++, buf++)
		if (*buf == sep) return *buf = 0;
	return 1;
}


int eval(int in) {
	char c;
	double d;
	char buf[512];
	
	while(read(in, &c, sizeof c) > 0) {
		if (c == TSEP) continue;
		if ((c & TCLASS) == TPRIMITIVE) {
			print("Class: primitive\n");
			switch(c &TPRITYPE) {
			case TDELIMITER:
				print("Type: delimiter\n");
				print("Delim: ");
				switch(c &TDATA) {
					case DOPENBRACE: print("{\n"); break;
					case DOPENPAREN: print("(\n"); break;
					case DOPENBRACK: print("[\n"); break;
					case DCLOSEBRACE: print("}\n"); break;
					case DCLOSEPAREN: print(")\n"); break;
					case DCLOSEBRACK: print("]\n"); break;
				}
				break;
			case TFUNCTION:
				print("Type: function\n");
				break;
			}
		} else if ((c & TCLASS) == TVALUE) {
			print("Class: value\n");
			print("Type: ");
			switch(c & TVALTYPE) {
			case TSTRING: 
				slurp(in, buf, sizeof buf, TSEP);
				print("String literal\n");
				print("Val: %s\n", buf);
				break;
			case TQEXEC: 
				slurp(in, buf, sizeof buf, TSEP);
				print("Command sub\n");
				print("Val: %s\n", buf);
				break;
			case TNAME: 
				slurp(in, buf, sizeof buf, TSEP);
				print("Variable name\n");
				print("Val: %s\n", buf);
				break;
			case TNUMBER: 
				print("Literal number\n");
				read(in, &d, sizeof d);
				print("Val: %g\n", d);
				break;
			}
		}
		print("\n");
	}
	if (errno == EAGAIN)
		return 0;
	else
		return 1;
}
