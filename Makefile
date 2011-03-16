CC := gcc
LD := gcc
BIN := apl
SRC := main.c parse.c scan.c array.c disp.c
INC := apl.h
OBJ := $(SRC:.c=.o)

CFLAGS = -Wall -I/usr/local/include -g
LDFLAGS = -lbio -lfmt -lutf -L/usr/local/lib 

all: $(BIN)

$(BIN): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(OBJ) : apl.h
parse.o : parse.h

clean:
	rm -f $(OBJ) $(BIN)
