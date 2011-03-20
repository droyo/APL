#define NELEM(x)(sizeof(x)/sizeof(*x))
enum tag {
	number		= 0x0001,
	string		= 0x0002,
	symbol		= 0x0004,
	function	= 0x0008,
	primitive	= 0x0010,
	dydop		= 0x0020,
	monop		= 0x0040,
	niladic		= 0x0080,
	boxed		= 0x0100,
	lparen		= 0x0200,
	rparen		= 0x0400,
	assign		= 0x0800,
	colon		= 0x1000,
	empty		= 0x2000,
	null		= 0x3000
};

enum flag {
	tmpmem		= 0x01,
	rdonly		= 0x02,
	managed		= 0x04
};

typedef struct {
	long gc;
	enum tag t;
	enum flag f;
	unsigned r, n, c;
	char m[];
} array;
#define ASIZE (sizeof(array))

extern array *zilde;
extern array *marker;
extern int quit;
extern int zero;
extern void *global_env;

/* Hash table for var bindings */
void*  shadow(void *, int);
array* put(void *, char*, array*);
array* get(void *, char*);

/* Array operations */
long   asize(array*);
array *aclone(array*);
array *atmp(void*, enum tag, unsigned, unsigned);
array *anew(enum tag, enum flag, unsigned, unsigned);
int   *ashp(array*);
void  *aval(array *);

/* Memory management */
void record(array*);
void incref(array*);
void decref(array*);

/* Core interpreter */
array*** scan(void *);
array* eval(void*,array ***);
void disp(array *);

/* Init/Teardown */
int const_init(void);
void const_free(void);
void *env_init(void);
void env_free(void*);
int mem_init(void);
void mem_coll(void);
void mem_free(void);
