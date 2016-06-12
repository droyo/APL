include config.mk

SRC := apl.c lex.c array.c machine.c util.c
OBJ := ${SRC:.c=.o}

all: options apl

options:
	@echo build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $<

${OBJ}: apl.h

apl: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f apl ${OBJ}

.PHONY: clean all
