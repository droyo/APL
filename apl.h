extern const char TSEP;
enum token_fields {
	TCLASS = 128,
	TVALTYPE = 96,
	TPRITYPE = 64
};
enum token_types {
	TPRIMITIVE = 128,
	TDELIMITER = 64,
	TFUNCTION = 0,
	TDATA = 63,
	TVALUE = 0,
	TNUMBER = 0,
	TSTRING = 32,
	TQEXEC = 64,
	TNAME = 96
};
enum delim_types {
	DOPENBRACE,
	DOPENPAREN,
	DOPENBRACK,
	DCLOSEBRACE,
	DCLOSEPAREN,
	DCLOSEBRACK
};

int scan(Biobuf *in, int out);
int eval(int fd);
int disp(void);
