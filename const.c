#include <stdlib.h>
#include "apl.h"

int zero = 0;
array *zilde;
array *marker;

int const_init(void) {
	if (!(zilde=anew(null,rdonly,0,0))) return -1;
	if (!(marker=anew(empty,rdonly,0,0))) return -1;
	return 0;
}
void const_free(void) {
	free(zilde);
}
