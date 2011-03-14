#include <string.h>
#include <stdlib.h>
#include "apl.h"

int type_sizes[] = {
	sizeof (double),	/* number */
	sizeof (char),		/* string */
	sizeof (char),		/* symbol */
	sizeof (array),		/* function */
	sizeof (char),		/* subcmd */
};
static int msize(array *a) {
	int s,t;
	for(s=0,t=a->t;t>>=1;s++);
	return a->n*(s>NELEM(type_sizes)?0:type_sizes[s]);
}
int copy(array *dst, array *src) {
	long s = sizeof(int)*src->r + msize(src);
	memcpy(dst, src, sizeof *dst);
	if(!(dst->m = malloc(s))) return 0;
	return !!memcpy(dst->m,src->m,s);
}
