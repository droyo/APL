#include <stddef.h>
#include "apl.h"

int main() {
	apl_array *one = apl_emalloc(APL_TYPE_INT);
	apl_array *two = apl_emalloc(APL_TYPE_INT);
	apl_instruction prog[] = {
		{.op = &apl_fn_push, .operand = one},
		{.op = &apl_fn_push, .operand = two},
		{.op = &apl_fn_add, .operand = NULL},
		{.op = &apl_fn_print, .operand = NULL},
		{.op = &apl_fn_exit, .operand = NULL}
	};

	apl_machine *m = apl_alloc_machine();
	apl_machine_run(m, prog);
}
