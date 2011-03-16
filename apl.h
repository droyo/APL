#define NELEM(x)(sizeof(x)/sizeof(*x))
enum tag {
	number		= 0x00001,
	string		= 0x00002,
	symbol		= 0x00004,
	function	= 0x00008,
	subcmd		= 0x00010,
	doperator	= 0x00020,
	moperator	= 0x00040,
	niladic		= 0x00080,
	lparen		= 0x00100,
	rparen		= 0x00200,
	lbracket	= 0x00400,
	rbracket	= 0x00800,
	lbrace		= 0x01000,
	rbrace		= 0x02000,
	primitive	= 0x04000,
	colon		= 0x08000,
	semicolon	= 0x10000,
	diamond		= 0x20000,
	assign		= 0x40000,
	empty		= 0x80000
};

typedef struct {
	enum tag t;
	int r, c, n;
	char *m;
} array;

/* Hash table for var bindings */
void*  shadow(void *, int);
array* put(void *, char*);
array* get(void *, char*);
void   del(void *, char*);

/* Array operations */
int copy(array *, array *);
void *val(array *);

/* Core interpreter */
array* scan(void *);
int parse(array *);
void disp(array *);

void *init_env(void);
int init_scan(void);
void env_free(void);
void cleanup_scan(void);
