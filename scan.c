#include <utf.h>
#include <fmt.h>
#include <bio.h>
#include "apl.h"

int scan_num(Biobuf *i, int o);
int scan_literal(Biobuf *i, int o);
int scan_special(Biobuf *i, int o);
int scan_id(Biobuf *i, int o);
int info(int fd,char c){return write(fd,&c,sizeof c);}

enum { single_quote = 39 };
enum { max_digits = 32 };
char quotes[] = "\"'`";
char special[] = "(){}[]⋄:;×+-"
	"⍫⍒⍋⌽⍉⊖⍟⍱⍲!⌹?⍵∊⍴~↑↓⍳○*←→"
	"⍥⍷⍬⍐⍗⍸⌷⍇⍈,+-⍺⌈⌊_∇∆∘'⍎⍕⋄"
	"⍙⌷≡≢⍞/\\⊢⍪⍤⊣⊂⊃∩∪⊥⊤|⍝⍀⌿.";

int scan(Biobuf *i, int o) {
	Rune r;
	while((r=Bgetrune(i))>0) {
		if(r == '\n') break;
		if(isspace(r)) continue;
		Bungetrune(i);
		
		if(isdigit(r)||utfrune("¯", r)) scan_num(i, o);
		else if(utfrune(quotes, r)) scan_literal(i, o);
		else if(utfrune(special, r)) scan_special(i, o);
		else scan_id(i, o);
	}
	return 0;
}

int scan_num(Biobuf *i, int o) {
	int sign;
	double d;
	
	sign = utfrune("¯", Bgetrune(i)) ? -1 : 1;
	if (sign > 0) Bungetrune(i);
	
	Bgetd(i, &d); d *= sign;
	info(o, TVALUE|TNUMBER);
	write(o, &d, sizeof d);
	
	return 0;
}

int scan_literal(Biobuf *i, int o) {
	Rune q, r;
	
	r = 0;
	q = Bgetrune(i);

	info(o, TVALUE|(q=='`'?TQEXEC:TSTRING));
	for(r = Bgetrune(i); q != r; r=Bgetrune(i))
		fprint(o, "%C", r);
	info(o, TSEP);
	
	return 0;
}

int scan_special(Biobuf *i, int o) {
	Bgetrune(i);
	info(o, TPRIMITIVE|TDELIMITER);
	return 0;
}

int scan_id(Biobuf *i, int o) {
	Rune r;
	
	info(o, TVALUE|TNAME);
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
	info(o, TSEP);
	return 0;
}

