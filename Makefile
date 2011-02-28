CC := gcc
LD := gcc
BIN := apl
SRC := main.c parse.c scan.c
INC := apl.h
OBJ := $(SRC:.c=.o)

CFLAGS = -Wall
LDFLAGS = -lbio -lfmt -lutf

all: $(BIN)

$(BIN): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $<

$(OBJ) : $(INC)

clean:
	rm -f $(OBJ) $(BIN)
