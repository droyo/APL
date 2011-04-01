#define NELEM(x)(sizeof(x)/sizeof(*x))
#define max(a,b) ((a)>(b))?(a):(b)
#define min(a,b) ((a)<(b))?(a):(b)

enum tag {
	number    = 0x0001, string   = 0x0002,
	symbol    = 0x0004, function = 0x0008,
	dydop     = 0x0010, monop    = 0x0020, 
	niladic   = 0x0040, boxed    = 0x0080, 
	lparen    = 0x0100, rparen   = 0x0200, 
	assign    = 0x0400, colon    = 0x0800, 
	empty     = 0x1000, null     = 0x2000,
	ldfns     = 0x4000, rdfns    = 0x8000
};

enum flag {
	tmpmem    = 0x01, rdonly = 0x02,
	managed   = 0x04, active = 0x08,
	primitive = 0x10, quiet  = 0x20
};

#define ASIZE (sizeof(array))
typedef struct {
	enum tag t; enum flag f;
	unsigned char r, k, c;
	unsigned short gc;
	unsigned long n,z;
	char m[];
} array;

extern array *zilde;
extern array *marker;
extern array *anon;
extern char quit;
extern void *G;

/* Hash table for var bindings */
void*  shadow(void *);
array* put(void *, char*, array*);
array* get(void *, char*);

/* Array operations */
long   asiz(array*);
array* acln(array*);
array* atmp(void*, enum tag, unsigned, unsigned);
array* anew(enum tag, enum flag, unsigned, unsigned);
int*   ashp(array*);
void*  aval(array*);
char*  akey(array*,char*,int);

/* Memory management */
void record(array*);
void incref(array*);
void decref(array*);

/* Core interpreter */
int      scan(void *,array**,int,char*,int);
array*   eval(void*,array **, array **);

/* Init,Teardown */
int   fmt_init(void);
int   const_init(void*);
int   sample_init(void*);
void* env_init(void);
void  env_free(void*);
int   mem_init(void);
void  mem_coll(void);
void  mem_free(void);
