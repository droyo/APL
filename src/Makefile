CC := gcc
LD := gcc
BIN := apl
AWK := awk
SRC := \
	main.c eval.c scan.c \
	array.c format.c env.c \
	memory.c const.c sample.c \
	error.c error-msg.c

INC := apl.h eval.h error.h \
       error-enm.h const.h codepoints.h

AUTO:= error-enm.h codepoints.h error-msg.c

OBJ := $(SRC:.c=.o)

CFLAGS = -Wall -I/usr/local/include -g
LDFLAGS = -lbio -lfmt -lutf -L/usr/local/lib 

all: $(BIN)

$(BIN): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

../aux/utf: ../aux/utf.c
	$(CC) -o $@ $^ $(LDFLAGS)

const.h: codepoints.h

codepoints.h: ../aux/utf symbols.utf
	../aux/utf < symbols.utf > $@

error.h: error-enm.h

error-enm.h: error.msg ../aux/enmake.awk
	$(AWK) -f ../aux/enmake.awk $< > $@

error-msg.c: error.msg ../aux/efmake.awk
	$(AWK) -f ../aux/efmake.awk $< > $@

$(OBJ) : apl.h error.h const.h

.c.o:
	$(CC) $(CFLAGS) -c $<

eval.o  : eval.h

clean:
	rm -f $(OBJ) $(BIN) $(AUTO)

.PHONY: clean all
