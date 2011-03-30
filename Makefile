CC := gcc
LD := gcc
BIN := apl
AWK := awk
SRC := \
	main.c eval.c scan.c \
	array.c format.c env.c \
	memory.c const.c sample.c \
	error.c error-msg.c

INC := apl.h eval.h error.h error-enm.h
OBJ := $(SRC:.c=.o)

CFLAGS = -Wall -I/usr/local/include -g
LDFLAGS = -lbio -lfmt -lutf -L/usr/local/lib 

all: $(BIN)

$(BIN): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

error.h: error-enm.h

error-enm.h: error.msg aux/enmake.awk
	$(AWK) -f aux/enmake.awk $< > $@

error-msg.c: error.msg aux/efmake.awk
	$(AWK) -f aux/efmake.awk $< > $@

.c.o:
	$(CC) $(CFLAGS) -c $<

$(OBJ) : apl.h error.h
eval.o : eval.h
error-msg.o error.o : error.h error-enm.h

clean:
	rm -f $(OBJ) $(BIN) error-msg.c error-enm.h
