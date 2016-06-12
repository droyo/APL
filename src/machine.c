#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include "apl.h"

/* Initial size, not a limit */
#define STACK_SIZE 300

struct apl_machine {
	apl_instruction *ip;  /* next instruction */
	apl_array **sp;       /* top of stack */
	apl_array **stack;    /* bottom of stack */
	size_t stack_size;    /* capacity of stack */
};

apl_machine *apl_alloc_machine(void) {
	apl_machine *new = apl_emalloc(sizeof *new);
	new->stack = apl_emalloc((sizeof new->sp[0]) * STACK_SIZE);
	new->sp = new->stack;
	new->stack_size = STACK_SIZE;
	return new;
}

void apl_machine_run(apl_machine *m, apl_instruction *prog) {
	m->ip = prog;
	apl_machine_next(m);
}

void apl_machine_next(apl_machine *m) {
	(m->ip++)->op(m, NULL);
}

void apl_fn_exit(apl_machine *m, apl_array *oper) {
	(void)(m);
	(void)(oper);
	printf("done\n");
	return;
}

void apl_fn_push(apl_machine *m, apl_array *oper) {
	*(m->sp++) = oper;
	apl_machine_next(m);
}

void apl_fn_print(apl_machine *m, apl_array *oper) {
	(void)m;
	(void)oper;
	printf("print\n");
	apl_machine_next(m);
}

void apl_fn_add(apl_machine *m, apl_array *oper) {
	(void)m;
	(void)oper;
	apl_machine_next(m);
}
