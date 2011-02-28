#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include <unistd.h>
#include "apl.h"

static struct token tok;
int gettok(int fd) {
	return read(fd, &tok, sizeof tok) > 0;
}

int parse(int fd) {
	while(gettok(fd)) switch(tok.tag) {
		case number: 
			print("N %g\n", tok.num);
			break;
		case string:
			print("S %s\n", tok.str);
			break;
		case subcmd:
			print("C %s\n", tok.str);
			break;
		case identifier:
			print("I %s\n", tok.str);
			break;
		case assign:
			print("A (%C)\n", tok.sym);
			break;
		default:
			print("D %C\n", tok.sym);
			break;
	}
	return 0;
}
