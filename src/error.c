#include "apl.h"
#include <utf.h>
#include <fmt.h>
#include <stdarg.h>
#include <stdlib.h>

int errfmt(char *fmt, ...) {
	int r; va_list ap;
	fprint(2, "!");
	va_start(ap, fmt);
	r = vfprint(2,fmt,ap);
	va_end(ap);
	return r;
}

void esay(int(*f)(va_list),...) {
	va_list ap; va_start(ap,f);
	f(ap); va_end(ap);
}
void *enil(int(*f)(va_list),...) {
	va_list ap; va_start(ap,f);
	f(ap); va_end(ap);
	return NULL;
}

int eneg(int(*f)(va_list),...) {
	va_list ap; va_start(ap,f);
	f(ap); va_end(ap);
	return -1;
}

void *ezil(int(*f)(va_list),...) {
	va_list ap; va_start(ap,f);
	f(ap); va_end(ap);
	return zilde;
}

void *edie(int(*f)(va_list),...) {
	va_list ap; va_start(ap,f);
	f(ap); va_end(ap);
	exit(1);
}
