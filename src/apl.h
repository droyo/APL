typedef struct apl_machine apl_machine;
typedef struct apl_array apl_array;

typedef struct {
	void (*op)(apl_machine*, apl_array*);
	apl_array *operand; /* For literals */
} apl_instruction;

typedef enum apl_array_type {
	APL_TYPE_INT = 0,
	APL_TYPE_STR = 1,
	APL_TYPE_REAL = 2,
	APL_TYPE_BOOL = 3,
	APL_FUNCTION = 4
} apl_array_type;

/* Utility functions */
void *apl_emalloc(size_t size);

apl_machine *apl_alloc_machine(void);
void apl_machine_run(apl_machine*, apl_instruction*);
void apl_machine_next(apl_machine *m);

/* builtin VM operations */
void apl_fn_exit(apl_machine *, apl_array *);
void apl_fn_push(apl_machine *, apl_array *);
void apl_fn_add(apl_machine *, apl_array *);
void apl_fn_print(apl_machine *, apl_array *);
