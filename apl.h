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
	boxed		= 0x0100,
	lparen		= 0x0200,
	rparen		= 0x0400,
	assign		= 0x0800,
	colon		= 0x1000,
	primitive	= 0x2000,
	empty		= 0x4000,
};

enum flag {
	tmpmem		= 0x01,
};

typedef struct {
	enum tag t;
	enum flag f;
	int r, c, n;
	char *m;
} array;
extern array zilde;
extern int quit;
extern void *global_env;

/* Hash table for var bindings */
void*  shadow(void *, int);
array* put(void *, char*, array*);
array* get(void *, char*);
void   del(void *, char*);

/* Array operations */
int acopy(array *, array *);
array *aclone(array*);
void *aval(array *);
void afree(array *);

/* Core interpreter */
array** scan(void *);
array* eval(void*,array **);
void disp(array *);

void *env_init(void);
void env_free(void*);
