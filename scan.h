extern char quotemarks[];
extern char delimiters[];
extern char primitives[];

int scan(Biobuf *in, int out);
int scanliteral(enum FSM*, int, Rune, Rune*);
int scannumeral(enum FSM*, int, Rune, char *);
int scandelimiter(enum FSM*, int, Rune);
int scanprimitive(enum FSM*, int, Rune);
int scanwhitespace(enum FSM*, int, Rune);

enum FSM {
	SCANNING,
	FINISHED,
	P_NUM,
	A_NUMBER,
	A_SYMBOL,
	A_LITERAL,
	ERROR
};
