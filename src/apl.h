#define NELEM(x)(sizeof(x)/sizeof(*x))
#define MAX(a,b) ((a)>(b))?(a):(b)
#define MIN(a,b) ((a)<(b))?(a):(b)

enum tag {
	TNUM = 1,       TSTR = TNUM<<1,
	TSYM = TSTR<<1, TFUN = TSYM<<1,
	TDYA = TFUN<<1, TMON = TDYA<<1,
	TCLK = TMON<<1, TBOX = TCLK<<1,
	TREL = TBOX<<1, TRAW = TREL<<1,
	TRPR = TRAW<<1, TLPR = TRPR<<1,
	TEND = TLPR<<1, TNIL = TEND<<1,
	TLDF = TNIL<<1, TRDF = TLDF<<1,
	TSET = TRDF<<1, TCOL = TSET<<1
};

enum flag {
	FTMP = 0x01, FRDO = 0x02,
	FMAN = 0x04, FSIL = 0x08,
	FSYS = 0x10
};

#define ASIZE (sizeof(array))
typedef struct {
	enum tag t; enum flag f;
	unsigned char r, k, c;
	unsigned short gc;
	unsigned long n,z;
	char m[];
} array;
typedef struct {char k[64];array *a;} pair;

extern array *zilde;
extern array *marker;
extern char quit;

/* Hash table for var bindings */
array* env(array*);
array* put(array*,char*,array*);
array* get(array*,char*);

/* Array operations */
array* atmp(void*, enum tag, unsigned, unsigned);
array* anew(array*,enum tag, enum flag,unsigned, unsigned);
array* abox(array*,unsigned,enum flag,array**);
array* astr(array*,char*);
array* acln(array*,array*);
int*   ashp(array*);
void   aclr(array*);
void*  aval(array*);
char*  akey(array*,char*,int);
void*  aget(array*,long);
void*  amem(array*,long);
int    afull(array*);
array* agrow(array**,long);
void*  apush(array*,const void*);
long   asize(array*);
void*  afind(array*,void*);
int    aeach(array*,int(*)(void*,void*),void*);

/* Memory management */
void record(array*,array*);
void incref(array*,array*);
void decref(array*,array*);

/* Core interpreter */
array* scan(array*,void*);
array* eval(array*,array*);

/* Init,Teardown */
int   fmt_init(array*);
int   cst_init(array*);
int   mem_init(array*);
void  mem_coll(array*);
void  mem_free(array*);
void  env_free(array*);
