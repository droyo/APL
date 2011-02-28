enum tag { number, identifier, semicolon, 
	colon, diamond, lparen, rparen, lbracket, 
	rbracket, lbrace, rbrace, assign,
	string, subcmd
};

struct token {
	enum tag tag;
	union {
		Rune sym;
		char *str;
		double num;
	};
};

int scan(Biobuf *in, int out);
int parse(int fd);
int disp(void *obj);

int init_scanner(void);
void cleanup_scanner(void);
