#define NELEM(x)(sizeof(x)/sizeof(*x))
#define max(a,b) ((a)>(b))?(a):(b)
#define min(a,b) ((a)<(b))?(a):(b)

enum tag {
	number  = 0x00001, string   = 0x00002,
	symbol  = 0x00004, function = 0x00008,
	dydop   = 0x00010, monop    = 0x00020, 
	niladic = 0x00040, boxed    = 0x00080, 
	byte    = 0x00100, lparen   = 0x00200, 
	rparen  = 0x00400, colon    = 0x00800, 
	empty   = 0x01000, null     = 0x02000,
	ldfns   = 0x04000, rdfns    = 0x08000,
	assign  = 0x10000
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
extern char quit;
extern void *G;

/* Hash table for var bindings */
void*  shadow(void *);
array* put(void *, char*, array*);
array* get(void *, char*);

/* Array operations */
long   asiz(array*);
array* acln(array*);
void   aclr(array*);
array* atmp(void*, enum tag, unsigned, unsigned);
array* anew(enum tag, enum flag, unsigned, unsigned);
int*   ashp(array*);
void*  aval(array*);
char*  akey(array*,char*,int);
array* afun(char*,unsigned,array**);
array* abox(unsigned,array**);
void*  amem(array*,long);
int    afull(array*);

/* Memory management */
void record(array*);
void incref(array*);
void decref(array*);

/* Core interpreter */
array* scan(void*,array*,array*);
array* eval(void*,array*);

/* Init,Teardown */
int   fmt_init(void);
int   const_init(void*);
int   sample_init(void*);
void* env_init(void);
void  env_free(void*);
int   mem_init(void);
void  mem_coll(void);
void  mem_free(void);
