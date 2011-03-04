CC := tcc
LD := tcc
BIN := apl
SRC := main.c parse.c scan.c
INC := apl.h
OBJ := $(SRC:.c=.o)

CFLAGS = -Wall -I/usr/local/include
LDFLAGS = -lbio -lfmt -lutf -L/usr/local/lib 

all: $(BIN)

$(BIN): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(OBJ) : $(INC)

clean:
	rm -f $(OBJ) $(BIN)
