#include "apl.h"
#include "func.h"

/* Acess the nth k-cell of a */
void *kcell(array *a, int k, int n) {
	if(a->r < k) return aval(a);
	int o = kcellsiz(a,k);
	int p = kcellsiz(a,k-a->r);
	return (n<p)? aget(a,n*o) : NULL;
}

/* Number of elements in a k-cell of a */
long kcellsiz(array *a, int k) {
	int o,*s=ashp(a),i=k<0?-k:a->r-k;
	if(a->r <= i) return a->n;
	for(o=1;i<a->r;i++) o*=s[i];
	return o;
}

/* Shape of a k-cell of a + number of elements */
long kcellshp(array *a, int k, int *x) {
	int o,*s=ashp(a),i=k<0?-k:a->r-k;
	for(o=1;i<a->r;i++) o*=x[i]=s[i];
	return o;
}
