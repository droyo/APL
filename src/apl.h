#define NELEM(x)(sizeof(x)/sizeof(*x))
#define MAX(a,b) ((a)>(b))?(a):(b)
#define MIN(a,b) ((a)<(b))?(a):(b)

typedef unsigned int uint;
typedef unsigned long ulong;
typedef unsigned short ushort;
typedef unsigned char uchar;
typedef unsigned long long uvlong;
typedef long long vlong;

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
	uchar r, k, c;
	ulong n,z;
	char m[];
} array;
typedef struct {char k[64];array *a;} pair;

extern array *zilde;
extern array *marker;

/* Hash table for var bindings */
array* env(array*);
array* put(array*,char*,array*);
array* get(array*,char*);

/* Array operations */
array* anew(array*,enum tag, enum flag,uint,uint);
array* atmp(void*, enum tag, uint,uint);
array* abox(array*,uint,enum flag,array**);
array* astr(array*,char*);
array* acln(array*,enum flag,array*);
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
array* scan(array*,array*,array*,void*);
array* eval(array*,array*);

/* Init,Teardown */
int   fmt_init(array*);
int   cst_init(array*);
int   mem_init(array*);
void  mem_coll(array*);
void  mem_free(array*);
void  env_free(array*);
