BIN := apl
SRC := main.c eval.c scan.c
INC := apl.h scan.h
OBJ := $(SRC:.c=.o)

LDFLAGS = -lbio -lfmt -lutf

all: $(BIN)

$(BIN): $(OBJ)
	tcc -o $@ $^ $(LDFLAGS)

.c.o:
	tcc -c $<

scan.o : scan.h apl.h
eval.o : apl.h
main.o : $(INC)

clean:
	rm -f $(OBJ) $(BIN)
