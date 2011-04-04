typedef struct {
	array *stack;
	array *settings;
	array* (*f)(array*,array*,array*);
} aplfunc;

#define APLFN(name)\
	array* name(array *E, array *a, array *w)

#define MONADIC (a->t == null)
#define DYADIC (!MONADIC)

void *kcell(array*,int,int);
long  kcellsiz(array*,int);
long  kcellshp(array*,int,int*);
