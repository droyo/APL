typedef struct {
	int dir;
	array **top;
	array **bot;
} stack;

typedef struct {
	long c[4];
	array* (*f)(array*,array**,int,int);
	long b, e;
} rule;

array* monad(array*,array**, int,int);
array* dyad (array*, array**, int,int);
array* moper(array*, array**, int,int);
array* doper(array*, array**, int,int);
array* bind (array*, array**, int,int);
array* punc (array*, array**, int,int);

array *lookup(array*,array *);
array *parse (array*,stack*,stack*,int);
array *mkfun (array*,stack*);
int exec(array*,stack*);
int apply(array*,rule*,stack*);

static array *pop(stack*);
static array *top(stack*);
static array *nth(stack*, int);
static void push(stack*,array*);
static int count(stack*);
static stack mkstack(array**,char);
