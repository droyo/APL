/* Stuff useful debug functions here. Not compiled in; paste them 
 * into the relevant source file and call them */

void showdbg(stack *l, stack *r) {
	array *a;
	print("[");
	for(a=l->bot;a <= r->bot;a++) {
		if(a > l->top && a < r->top) {
			print(" ");
			continue;
		}
		if(a == r->top) print("♦");
		disp(a);
	} print("]\n");
}
void top4(stack *s) {
	int i; for (i=0;i<4;i++) {
		disp(nth(s,i));
		print(",");
	}print("\n");
}
