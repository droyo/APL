enum Tag {
	TSEP = 0xFE,
	TCLS = 0xC0,
	TVAR = 0x00,
	TSTR = 0x40,
	TCMD = 0x20,
	TDLM = 0xC0,
	TNUM = 0x80
};

int scan(Biobuf *in, int out);
int parse(int fd);
int disp(void *obj);

int init_eval(void);
int init_parser(void);

void cleanup_parser(void);
void cleanup_eval(void);
