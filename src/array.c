#include <stddef.h>
#include "apl.h"

struct apl_array {
	int rank;
	int *dim; /* dim[rank] */
	apl_array_type type;
	union {
		char *string;
		int *integer;
		double *real;
		unsigned char *bool;
		struct apl_array *boxed;
	};
};

/* Creates a 0-size array of given rank */
apl_array *apl_alloc_array(apl_array_type type, int rank) {
	apl_array *new = apl_emalloc(sizeof *new);
	new->rank = rank;
	new->dim = apl_emalloc((sizeof new->dim[0]) * rank);
	new->type = type;
	return new;
}
