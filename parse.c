#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include "apl.h"
/* Strategy - Change the stream of tokens into an
	execution stack ⍬ */

#define NELEM(x) ((sizeof (x)) / (sizeof (*x)))
struct token tok;
int next(int fd);

int parse(int fd) {
	while(next(fd)) switch(tok.tag) {
		case number: 
			print("num %g\n", tok.num);
			break;
		case string:
			print("str %s\n", tok.str);
			break;
		case subcmd:
			print("cmd %s\n", tok.str);
			break;
		case identifier:
			print("sym %s\n", tok.str);
			break;
		case assign:
			print("set (%C)\n", tok.sym);
			break;
		case function:
			print("fn %s\n", tok.str);
			break;
		default:
			print("del %C\n", tok.sym);
			break;
	}
	return 1;
}

int next(int fd) {
	return read(fd, &tok, sizeof tok)>0;
}
