#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "apl.h"

#define STACK_SIZE 300

struct apl_machine {
	apl_instruction *ip;
	apl_array *sp;
	apl_array *stack;
};

typedef struct {
	void *addr;
	unsigned char next;
} jump_table;

apl_instruction *apl_translate_prog(jump_table*, apl_instruction*, size_t);

apl_machine *apl_alloc_machine(void) {
	apl_machine *m = apl_emalloc(sizeof *m);
	m->stack = apl_emalloc(sizeof m->stack[0] * STACK_SIZE);
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
#define NEXT(m) goto **(m->ip++);
void apl_machine_run(apl_machine *m, apl_instruction *prog, size_t size) {
	apl_array *arg;
	/*
	Most examples of direct threading that you will find on the
	internet use a lot of global variables. To avoid global state,
	we to transform the symbolic opcodes in *prog to
	the addresses contained in this code block.
	*/
	jump_table tbl[] = {
		[APL_OP_END]   = {.addr = &&apl_op_end, .next = 1},
		[APL_OP_ADD]   = {.addr = &&apl_op_add, .next = 1},
		[APL_OP_SUB]   = {.addr = &&apl_op_sub, .next = 1},
		[APL_OP_MUL]   = {.addr = &&apl_op_mul, .next = 1},
		[APL_OP_DIV]   = {.addr = &&apl_op_div, .next = 1},
		[APL_OP_CONST] = {.addr = &&apl_op_const, .next = 2},
	};

	apl_instruction *native = apl_translate_prog(tbl, prog, size);
	m->ip = native;
	NEXT(m);

apl_op_add:
	puts("call add");
	NEXT(m);

apl_op_sub:
	puts("call sub");
	NEXT(m);

apl_op_mul:
	puts("call mul");
	NEXT(m);

apl_op_div:
	puts("call div");
	NEXT(m);

apl_op_const:
	arg = *m->ip++;
	printf("call const %p\n", arg);
	NEXT(m);

apl_op_end:
	free(native);
}

apl_instruction *
apl_translate_prog(jump_table *tbl, apl_instruction *prog, size_t size) {
	int i;
	int count = (int)(size / sizeof *prog);
	apl_instruction *native = memcpy(apl_emalloc(size), prog, size);
	for(i = 0; i < count; i += tbl[(intptr_t)prog[i]].next) {
		native[i] = tbl[(intptr_t)prog[i]].addr;
	}
	return native;
}
