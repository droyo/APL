#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "apl.h"

int main() {
	apl_array data[] = {};
	/* y ← 1 + 1 */
	apl_instruction code[] = {
		(APL_OP_CONST<<24)|(1 << 16)|(1 << 0),
		(APL_OP_CONST<<24)|(2 << 16)|(1 << 0),
		(APL_OP_ADD  <<24)|(3 << 16)|(1 << 8)|(2 << 0),
		(APL_OP_STORE<<24)|(3 << 16)|(1 << 0),
		(APL_OP_END<<24)
	};
	apl_program prog = {
		.code = code,
		.code_size = sizeof code,
		.data_size = sizeof data
	};
	apl_exec(&prog);
}
