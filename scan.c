#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include "apl.h"

int scan_number(Biobuf *i, int o);
int scan_literal(Biobuf *i, int o);
int scan_special(Biobuf *i, int o);
int scan_symbol(Biobuf *i, int o);
int scan_delims(Biobuf *i, int o);

int tag(int fd, const char c) {
	return write(fd, &c, sizeof c);
}

char quotes[] = "\"'`";
char delims[] = "(){}[]♦:;";
char special[] = "(){}[]⋄:;×+-¨"
	"⍫⍒⍋⌽⍉⊖⍟⍱⍲!⌹?⍵∊⍴~↑↓⍳○*←→"
	"⍥⍷⍬⍐⍗⍸⌷⍇⍈,+-⍺⌈⌊_∇∆∘'⍎⍕⋄"
	"⍙⌷≡≢⍞/\\⊢⍪⍤⊣⊂⊃∩∪⊥⊤|⍝⍀⌿.";

int scan(Biobuf *i, int o) {
	Rune r;
	while((r=Bgetrune(i))>0) {
		if(r == '\n') break;
		if(isspace(r)) continue;
		Bungetrune(i);
		
		if(isdigit(r)||utfrune("¯", r)) scan_number(i, o);
		else if(utfrune(quotes, r)) scan_literal(i, o);
		else if(utfrune(delims, r)) scan_delims(i, o);
		else if(utfrune(special, r)) scan_special(i, o);
		else scan_symbol(i, o);
	}
	return 0;
}

int scan_number(Biobuf *i, int o) {
	int sign;
	double d;
	
	sign = utfrune("¯", Bgetrune(i)) ? -1 : 1;
	if (sign > 0) Bungetrune(i);
	
	Bgetd(i, &d); d *= sign;
	tag(o, TNUM);
	write(o, &d, sizeof d);
	
	return 0;
}

int scan_literal(Biobuf *i, int o) {
	Rune q, r;
	
	r = 0;
	q = Bgetrune(i);

	tag(o, TSTR|((q=='`') * TCMD));
	for(r = Bgetrune(i); q != r; r=Bgetrune(i))
		fprint(o, "%C", r);
	tag(o, TSEP);
	
	return 0;
}

int scan_special(Biobuf *i, int o) {
	tag(o, TVAR);
	fprint(o, "%C", Bgetrune(i));
	tag(o, TSEP);
	return 0;
}

int scan_delims(Biobuf *i, int o) {
	tag(o, TDLM);
	fprint(o, "%C", Bgetrune(i));
	tag(o, TSEP);
	return 0;
}

int scan_symbol(Biobuf *i, int o) {
	Rune r;
	
	tag(o, TVAR);
	while((r=Bgetrune(i))>0) {
		if (isspace(r)) 
			goto End_Token;
		else if (utfrune(quotes, r)) 
			goto End_Token;
		else if(utfrune(special, r)) 
			goto End_Token;
		fprint(o, "%C", r);
	}
	return 0;
End_Token:
	Bungetrune(i);
	tag(o, TSEP);
	return 0;
}

