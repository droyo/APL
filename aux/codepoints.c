/* Print out a list of unicode codepoints for
 * primitive functions */
#include <utf.h>
#include <fmt.h>
#include <bio.h>

int main(void) {
	Rune r;
	Biobuf *i = Bfdopen(0,O_RDONLY);
	while((r=Bgetrune(i))>0) {
		if(isspace(r)) continue;
		print("\t%C = 0x%X\n",r,r);
	}
	Bterm(i);
	return 0;
}

