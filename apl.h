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
		int num;
	};
} apl_array;

apl_array *apl_alloc_array(apl_array_type, int);
apl_array *apl_array_reshape(apl_array*, int, int*);

/* VM & bytecode */
typedef uint32_t apl_instruction;
typedef struct {
	apl_instruction *code;
	apl_array *data;
	size_t code_size;
	size_t data_size;
} apl_program;

void apl_exec(apl_program*);

enum apl_bytecodes {
	APL_OP_UNKNOWN,
	APL_OP_END,
	APL_OP_ADD,
	APL_OP_SUB,
	APL_OP_MUL,
	APL_OP_DIV,
	APL_OP_CONST,
	APL_OP_LOAD,
	APL_OP_STORE,
	
	APL_OP_COUNT
};

/* Parsing functions */
apl_program *apl_compile(char*);

/* Utility functions */
void *apl_emalloc(size_t size);
void *apl_strdup(void *buf, size_t size);
