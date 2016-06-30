include config.mk
CFLAGS += -Ilibutf

OBJ := main.o array.o machine.o util.o compile.o

dir := libutf
include $(dir)/Makefile 
dir := $(CURDIR)

all: options apl

options:
	@echo build options:
	@echo "CFLAGS = ${CFLAGS}"
	@echo "LDFLAGS = ${LDFLAGS}"
	@echo "CC = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -c ${CFLAGS} $< -o $@

$(CURDIR)/%.c: apl.h

apl: ${OBJ}
	@echo LD $@
	${CC} ${LDFLAGS} -o $@ ${OBJ}

clean:
	rm -f apl ${OBJ} 

.PHONY: clean all
