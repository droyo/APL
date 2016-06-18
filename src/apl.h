typedef unsigned char apl_instruction;
typedef struct {
	apl_instruction *code;
	size_t size;
	size_t op_count;
} apl_program;

typedef enum apl_array_type {
	APL_TYPE_REAL,
	APL_TYPE_CHAR,
	APL_TYPE_COUNT
} apl_array_type;

typedef struct apl_array {
	int rank;
	int *dim; /* dim[rank] */
	apl_array_type type;
	union {
		void *data;
		char *string;
		double *real;
	};
} apl_array;

apl_array *apl_alloc_array(apl_array_type, int);
apl_array *apl_array_reshape(apl_array*, int, int*);


/* VM & bytecode */
#define APL_STACK_SIZE 300
typedef struct apl_machine apl_machine;
apl_machine *apl_alloc_machine(void);
void apl_machine_run(apl_machine*, apl_program*);

enum apl_bytecodes {
	APL_OP_UNKNOWN,
	APL_OP_END,
	APL_OP_ADD,
	APL_OP_SUB,
	APL_OP_MUL,
	APL_OP_DIV,
	APL_OP_CONST
};

/* Utility functions */
void *apl_emalloc(size_t size);
void *apl_strdup(void *buf, size_t size);
