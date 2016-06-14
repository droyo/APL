#include <stddef.h>
#include "apl.h"

int main() {
	apl_machine *m = apl_alloc_machine();
	apl_instruction prog[] = {
		(apl_instruction)APL_OP_ADD,
		(apl_instruction)APL_OP_SUB,
		(apl_instruction)APL_OP_MUL,
		(apl_instruction)APL_OP_CONST,
		(apl_instruction)apl_alloc_array(APL_TYPE_REAL, 0),
		(apl_instruction)APL_OP_END
	};
	
	apl_machine_run(m, prog, sizeof prog);
}
