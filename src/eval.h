typedef struct {
	int dir;
	array **top;
	array **bot;
} stack;

typedef struct {
	long c[4];
	array* (*f)(void*,array**,int,int);
	long b, e;
} rule;

array* monad(void*,array**, int,int);
array* dyad (void*, array**, int,int);
array* moper(void*, array**, int,int);
array* doper(void*, array**, int,int);
array* bind (void*, array**, int,int);
array* punc (void*, array**, int,int);

array *lookup(void*,array *);
array *parse (void*,stack*,stack*,int);
int exec(void*,stack*);
int apply(void*,rule*,stack*);

static array *pop(stack*);
static array *top(stack*);
static array *nth(stack*, int);
static void push(stack*,array*);
static int count(stack*);
static stack mkstack(array**,char);
