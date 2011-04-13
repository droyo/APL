#define NELEM(x)(sizeof(x)/sizeof(*x))
#define MAX(a,b) ((a)>(b))?(a):(b)
#define MIN(a,b) ((a)<(b))?(a):(b)

enum tag {
	TNUM = 0x00001, TSTR = 0x00002,
	TSYM = 0x00004, TFUN = 0x00008,
	TDYA = 0x00010, TMON = 0x00020, 
	TCLK = 0x00040, TBOX = 0x00080, 
	TRAW = 0x00100, TLPR = 0x00200, 
	TRPR = 0x00400, TCOL = 0x00800, 
	TEND = 0x01000, TNIL = 0x02000,
	TLDF = 0x04000, TRDF = 0x08000,
	TSET = 0x10000
};

enum flag {
	FTMP = 0x1,  FRDO = 0x02,
	FMAN = 0x04, FSIL = 0x08
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
extern void *S;
extern void *G;

/* Hash table for var bindings */
array* env(array*);
array* put(array*,char*,array*);
array* get(array*,char*);

/* Array operations */
array* acln(array*);
void   aclr(array*);
array* atmp(void*, enum tag, unsigned, unsigned);
array* anew(enum tag, enum flag, unsigned, unsigned);
int*   ashp(array*);
void*  aval(array*);
char*  akey(array*,char*,int);
void*  aget(array*,long);
array* abox(unsigned,array**);
array* astr(char*);
void*  amem(array*,long);
int    afull(array*);
array* agrow(array**,long);
void*  apush(array**,const void*);
long   asize(array*);

/* Memory management */
void record(array*);
void incref(array*);
void decref(array*);

/* Core interpreter */
array* scan(void*,array**,array**);
array* eval(array*,array*);

/* Init,Teardown */
int   fmt_init(void);
int   const_init(void);
int   sample_init(void*);
int   mem_init(void);
void  mem_coll(void);
void  mem_free(void);
