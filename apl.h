#define NELEM(x)(sizeof(x)/sizeof(*x))
enum tag {
	empty		= 0x0000,
	number		= 0x0001,
	string		= 0x0002,
	identifier	= 0x0004,
	function	= 0x0008,
	colon		= 0x0010,
	lparen		= 0x0020,
	rparen		= 0x0040,
	lbracket	= 0x0080,
	rbracket	= 0x0100,
	rbrace		= 0x0200,
	lbrace		= 0x0400,
	assign		= 0x0800,
	string		= 0x1000,
	subcmd		= 0x2000,
	semicolon	= 0x4000,
	diamond		= 0x8000
};

typedef struct {
	enum tag t;
	int r, ref, n;
	void *m;
} array;

extern array tok[128];
int scan(Biobuf *in);
int parse(int end);

int init_scan(void);
void cleanup_scan(void);
