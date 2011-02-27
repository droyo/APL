#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include <stdlib.h>
#include "apl.h"

static struct {
	char *pool;
	unsigned long top;
	unsigned long max;
} mem;

int init_parser(void) {
	mem.top = 0;
	mem.max = 512;
	if(!(mem.pool = malloc(mem.max))) {
		fprint(2, "Init parser: %r\n");
		return -1;
	}
	return 0;
}

void cleanup_parser(void) {
	free(mem.pool);
}

static int mem_ok(void) {
	void *ptr;
	if(mem.max - mem.top < 3) {
		ptr = realloc(mem.pool, mem.max + 512);
		if(!ptr) {
			fprint(2, "Error: %r\n");
			return 0;
		}
		mem.pool = ptr;
	}
	return 1;
}

static void mem_reset(void) {
	mem.top = 0;
	mem.pool[0] = '\0';
}

char *slurp(int fd) {
	char *s;
	enum Tag c;

	s = mem.pool + mem.top;
	for(c=0;mem_ok() && read(fd,&c,1)>0;mem.top++) {
		if(c == TSEP) break;
		mem.pool[mem.top] = c;
	}
	mem.pool[mem.top++] = '\0';
	return s;
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
		case TVAR:
			print("Variable %s\n", slurp(fd));
			break;
		case TSTR:
			if((c &~TSTR) == TCMD)
				print("Command sub ");
			else
				print("String ");
			print("%s\n", slurp(fd));break;
		case TDLM:
			print("Delimiter %s\n", slurp(fd));
			break;
		}
	}
	mem_reset();
	return 0;
}
