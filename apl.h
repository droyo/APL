#define NELEM(x)(sizeof(x)/sizeof(*x))
enum tag { number, identifier, function,
	colon, diamond, lparen, rparen, lbracket, 
	rbracket, lbrace, rbrace, assign,
	string, subcmd, empty,  semicolon
};

union val { double d; Rune p; char *s; void *m; };
struct obj { enum tag t; union val v; };
extern struct obj tok[128];
int scan(Biobuf *in);
int parse(int end);

int init_scan(void);
void cleanup_scan(void);
