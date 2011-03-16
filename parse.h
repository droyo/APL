typedef struct {
	array *top;
	array *bot;
} stack;

typedef struct {
	long c[4];
	array (*f)(void*,array*,array*);
	long b, e;
} rule;

array monad(void *env, array*, array*);
array dyad(void *env, array*, array*);
array oper(void *env, array*, array*);
array set(void *env, array*, array*);
array punc(void *env, array*, array*);

array *eval(array *);
int exec(stack *);
int apply(rule *, stack *);

static array *pop(stack *);
static array *top(stack *);
static array *nth(stack *, int);
static void push(stack *, array*);
static int count(stack *);
static stack mkstack(array *);
