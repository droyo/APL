#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "apl.h"

struct apl_machine {
	apl_array *sp;
	apl_array stack[APL_STACK_SIZE];
};

typedef struct {
	void *addr;
	unsigned char next;
} apl_jump_table;

void **apl_translate_prog(apl_jump_table*, apl_program*);

apl_machine *apl_alloc_machine(void) {
	apl_machine *m = apl_emalloc(sizeof *m);
	m->sp = m->stack;
	return m;
}

/*
This interpreter implements Direct Threading using the
GNU C "Labels as values" extension: 
	https://gcc.gnu.org/onlinedocs/gcc/Labels-as-Values.html

See "The Structure and Performance of Efficient Interpreters"
for more on direct threading:
	https://www.jilp.org/vol5/v5paper12.pdf
*/
#define NEXT(ip) goto **ip++
void apl_machine_run(apl_machine *m, apl_program *prog) {
	void **ip;
	m->sp = m->stack;
	apl_array *arg;
		apl_jump_table tbl[] = {
		[APL_OP_UNKNOWN]   = {.addr = &&apl_op_unknown, .next = 1},
		[APL_OP_END]   = {.addr = &&apl_op_end, .next = 1},
		[APL_OP_ADD]   = {.addr = &&apl_op_add, .next = 1},
		[APL_OP_SUB]   = {.addr = &&apl_op_sub, .next = 1},
		[APL_OP_MUL]   = {.addr = &&apl_op_mul, .next = 1},
		[APL_OP_DIV]   = {.addr = &&apl_op_div, .next = 1},
		[APL_OP_CONST] = {.addr = &&apl_op_const, .next = sizeof arg},
	};
	void **native = apl_translate_prog(tbl, prog);
	ip = native;
	NEXT(ip);

apl_op_add:
	puts("call add");
	NEXT(ip);

apl_op_sub:
	puts("call sub");
	NEXT(ip);

apl_op_mul:
	puts("call mul");
	NEXT(ip);

apl_op_div:
	puts("call div");
	NEXT(ip);

apl_op_const:
	arg = *ip;
	ip += sizeof(apl_array*);
	printf("call const %p\n", arg);
	NEXT(ip);

apl_op_unknown:
	puts("illegal bytecode");
apl_op_end:
	puts("done");
	free(native);
}

void **apl_translate_prog(apl_jump_table *tbl, apl_program *prog) {
	size_t i;
	size_t size;
	void **buf;
	apl_instruction *code;
	
	size = prog->op_count * sizeof(*buf);
	size += (prog->size - prog->op_count);

	buf = apl_emalloc(size);
	code = prog->code;
	for(i = 0; i < prog->size; i += tbl[code[i]].next) {
		buf[i] = tbl[code[i]].addr;
		printf("%x -> %p\n", code[i], buf[i]);
	}
	return buf;
}
