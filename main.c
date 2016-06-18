#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include "apl.h"

int main() {
	apl_machine *m = apl_alloc_machine();
	apl_instruction code[] = {
		APL_OP_ADD,
		APL_OP_SUB,
		APL_OP_MUL,
		APL_OP_CONST,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		APL_OP_END
	};
	printf("%ld\n", sizeof(apl_array*));
	apl_program prog = {
		.code = code,
		.size = sizeof code,
		.op_count = sizeof code - sizeof(apl_array*)
	};
	
	apl_machine_run(m, &prog);
}
