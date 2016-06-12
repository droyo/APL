#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "apl.h"

/* apl_emalloc never returns an error. zeroes returned memory */
void *apl_emalloc(size_t size) {
	void *m;
	m = malloc(size);
	if (m == NULL) {
		perror("failed to allocate memory");
	}
	return memset(m, 0, size);
}
