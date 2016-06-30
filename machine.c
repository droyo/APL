#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "apl.h"

#define STACK_SIZE 300

typedef struct {
	apl_instruction *pc; /* The current instruction */
	apl_array *r[256]; /* 256 general-purpose registers */
} machine;

/*
Bytecode layout:

32-bit fixed-width instruction.

Standard form: 8/8/8/8 OP/DST/X/Y
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
|----opcode-----|-----dst-----|-------x-------|--------y--------|

LOAD/STORE/CONST form: 8/8/16 OP/REG/ADDR
 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
|----opcode-----|-----reg-----|-------------addr----------------|
*/
uint32_t op  (apl_instruction x) { return (x & 0xff000000) >> 24; }
uint32_t dst (apl_instruction x) { return (x & 0x00ff0000) >> 16; }
uint32_t reg (apl_instruction x) { return (x & 0x00ff0000) >> 16; }
uint32_t arg1(apl_instruction x) { return (x & 0x0000ff00) >> 8; }
uint32_t arg2(apl_instruction x) { return (x & 0x000000ff) >> 0; }
uint32_t addr(apl_instruction x) { return (x & 0x0000ffff) >> 0; }

void apl_exec(apl_program *prog) {
	apl_instruction inst;
	machine m;
	memset(&m, 0, sizeof m);
	m.pc = prog->code;
	for(;;) {
		inst = *m.pc;
		switch(op(inst)) {
		case APL_OP_ADD:
			printf("ADD r%u r%u → r%u\n",
				arg1(inst), arg2(inst), dst(inst));
			break;
		case APL_OP_SUB:
			printf("SUB r%u r%u → r%u\n",
				arg1(inst), arg2(inst), dst(inst));
			break;
		case APL_OP_MUL:
			printf("MUL r%u r%u → r%u\n",
				arg1(inst), arg2(inst), dst(inst));
			break;
		case APL_OP_DIV:
			printf("DIV r%u r%u → r%u\n",
				arg1(inst), arg2(inst), dst(inst));
			break;
		case APL_OP_CONST:
			printf("CONST 0x%x → r%u\n",
				addr(inst), reg(inst));
			break;
		case APL_OP_LOAD:
			printf("LOAD 0x%x → r%u\n",
				addr(inst), reg(inst));
			break;
		case APL_OP_STORE:
			printf("STORE r%u → 0x%x\n",
				reg(inst), addr(inst));
			break;
		case APL_OP_END:
			puts("END");
			return;
		default:
			printf("unknown code 0x%x\n", op(inst));
			printf("known codes are:\n");
			apl_instruction i;
			for(i = 0; i < APL_OP_COUNT; i++) {
				printf("\t0x%x\n", i);
			}
			return;
		}
		m.pc++;
	}
}
