/* Print out a list of unicode codepoints for
 * primitive functions */
#include <utf.h>
#include <fmt.h>
#include <bio.h>

int main(void) {
	Rune r; char *s; int n = 0;
	Biobuf *i = Bfdopen(0,O_RDONLY);
	print("enum codepoints {");
	while((r=Bgetrune(i))>0 && r != '\n') {
		s = Brdline(i,'\n');
		if(!Blinelen(i)) break;
		s[Blinelen(i)-1] = '\0';
		*s = 'U';
		print("%s\n\t%-8s = 0x%X",n?",":"", s,r);
		n = 1;
	}
	print("\n};\n");
	Bterm(i);
	return 0;
}
