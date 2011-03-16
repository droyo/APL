typedef struct {
	array *top;
	array *bot;
} stack;

typedef struct {
	long c[4];
	array (*f)(void*,array**,int,int);
	long b, e;
} rule;

array monad(void *env, array**, int,int);
array dyad(void *env, array**, int,int);
array moper(void *env, array**, int,int);
array doper(void *env, array**, int,int);
array set(void *env, array**, int,int);
array punc(void *env, array**, int,int);

array *eval(array *);
int exec(stack *);
int apply(rule *, stack *);

static array *pop(stack *);
static array *top(stack *);
static array *nth(stack *, int);
static void push(stack *, array*);
static int count(stack *);
static stack mkstack(array *);
