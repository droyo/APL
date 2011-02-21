enum FSM {
	SCANNING,
	FINISHED,
	A_NUMBER,
	A_SYMBOL,
	A_LITERAL,
	ERROR
};

char quotemarks[] = "\"'`";
char delimiters[] = "(){}[]";
char primitives[] = "¨¯<≤=≥>≠∨^×÷"
	"⌶⍫⍒⍋⌽⍉⊖⍟⍱⍲!⌹"
	"?⍵∊⍴~↑↓⍳○*←→"
	"⍥⍷⍬⍐⍗⍸⌷⍇⍈,+-"
	"⍺⌈⌊_∇∆∘'⎕⍎⍕⋄"
	"⍙⌷≡≢⍞/\\⊢⍪⍤"
	"⊣⊂⊃∩∪⊥⊤|⍝⍀⌿";

int scan(Biobuf *in, int out);
int eval(Biobuf *in);
int disp(void);

int scanliteral(enum FSM*, int, Rune, Rune*);
int scannumeral(enum FSM*, int, Rune);
int scandelimiter(enum FSM*, int, Rune);
int scanprimitive(enum FSM*, int, Rune);
int scanwhitespace(enum FSM*, int, Rune);
