#define NELEM(x)(sizeof(x)/sizeof(*x))
enum tag {
	number		= 0x0001,
	string		= 0x0002,
	symbol		= 0x0004,
	function	= 0x0008,
	subcmd		= 0x0010,
	lparen		= 0x0020,
	rparen		= 0x0040,
	lbracket	= 0x0080,
	rbracket	= 0x0100,
	lbrace		= 0x0200,
	rbrace		= 0x0400,
	assign		= 0x0800,
	colon		= 0x1000,
	semicolon	= 0x2000,
	diamond		= 0x4000,
	marker		= 0x8000
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
array* scan(void *v);
int parse(array *end);

void *init_env(void);
int init_scan(void);
void env_free(void);
void cleanup_scan(void);
