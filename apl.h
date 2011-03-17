#define NELEM(x)(sizeof(x)/sizeof(*x))
enum tag {
	number		= 0x0001,
	string		= 0x0002,
	symbol		= 0x0004,
	function	= 0x0008,
	subcmd		= 0x0010,
	doperator	= 0x0020,
	moperator	= 0x0040,
	niladic		= 0x0080,
	lparen		= 0x0100,
	rparen		= 0x0200,
	boxed		= 0x0400,
	assign		= 0x0800,
	colon		= 0x1000,
	empty		= 0x2000,
};

typedef struct {
	enum tag t;
	int r, c, n;
	char *m;
} array;
extern array zilde;
extern int quit;

/* Hash table for var bindings */
void*  shadow(void *, int);
array* put(void *, char*);
array* get(void *, char*);
void   del(void *, char*);

/* Array operations */
int copy(array *, array *);
array *clone(array*);
void *val(array *);

/* Core interpreter */
array** scan(void *);
array* parse(array **);
void disp(array *);

void *init_env(void);
void env_free(void);
