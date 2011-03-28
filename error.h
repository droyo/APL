void*  enil (int,...);
int    eneg (int,...);
array* ezil (int,...);
int    ezro (int ,...);


struct emsg {
	int type;
	char *msg;
};
extern const struct emsg error_messages;

