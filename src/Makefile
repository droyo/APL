CC := gcc
LD := gcc
AWK := awk
BIN := apl

SRC := \
	main.c eval.c scan.c \
	array.c format.c env.c \
	memory.c const.c error.c \
	error-msg.c

INC := apl.h eval.h error.h \
       error-enm.h const.h 

AUTO:= error-enm.h const.h error-msg.c
OBJ := $(SRC:.c=.o)

CFLAGS = -Wall -I/usr/local/include -g
LDFLAGS = -lbio -lfmt -lutf -L/usr/local/lib 

all: $(BIN)

$(BIN): $(OBJ)
	@echo LD $@
	@$(LD) -o $@ $^ $(LDFLAGS)

$(OBJ) : apl.h error.h const.h
error.h: error-enm.h
eval.o : eval.h

.c.o:
	@echo CC $<
	@$(CC) $(CFLAGS) -c $<

../aux/utf: ../aux/utf.c
	@echo MK $@
	@$(CC) -o $@ $^ $(LDFLAGS)

const.h: ../aux/utf symbols.utf
	@echo MK $@
	@../aux/utf < symbols.utf > $@

error-enm.h: error.msg ../aux/enmake.awk
	@echo MK $@
	@$(AWK) -f ../aux/enmake.awk $< > $@

error-msg.c: error.msg ../aux/efmake.awk
	@echo MK $@
	@$(AWK) -f ../aux/efmake.awk $< > $@

clean:
	@echo Cleaning...
	@rm -f $(OBJ) $(BIN) $(AUTO)

.PHONY: clean all
