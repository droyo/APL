all:
	tcc -o apl apl.c -lbio -lfmt -lutf 

clean:
	rm -f apl
