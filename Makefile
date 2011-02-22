BIN := apl
SRC := main.c eval.c scan.c
INC := apl.h
OBJ := $(SRC:.c=.o)

LDFLAGS = -lbio -lfmt -lutf

all: $(BIN)

$(BIN): $(OBJ)
	tcc -o $@ $^ $(LDFLAGS)

.c.o:
	tcc -c $<

$(OBJ) : $(INC)

clean:
	rm -f $(OBJ) $(BIN)
