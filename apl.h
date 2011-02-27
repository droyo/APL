enum Tag {
	TSEP = 0xFE,
	TCLS = 0xC0,
	TVAR = 0x00,
	TSTR = 0x40,
	TNUM = 0x80,
	TCMD = 0xC0
};

int scan(Biobuf *in, int out);
int parse(int fd);
int disp(void *obj);
