BEGIN {
	cfunc="int E%s(va_list ap) {\n"\
		"\tint r;\n"\
		"%s"\
		"\tr = errfmt(\"%s\"%s);\n"\
		"\treturn r;\n}\n"
	print("#include \"apl.h\"")
	print("#include <stdarg.h>")
	print("#include \"error.h\"")
	print("#include \"error-enm.h\"")
	print("extern int errfmt(char*,...);");
	print("extern char *err_txt[];");
	fmt["array*"]	= "%A"
	fmt["int"]		= "%d"
	fmt["char*"]	= "%s"
	fmt["Rune*"]	= "%S"
}
function print_error(x) {
	s = body[x]
	a = ""
	gsub("\"", "\\\"", s)
	while(match(s,/%[0-9]+/)) {
		n = substr(s,RSTART,RLENGTH)
		idx=substr(n,2)
		t = fmt[typ[x,idx]]
		a=sprintf("%s,%s",a,var[x,idx])
		sub(n,t,s)
	}
	printf(cfunc,x,dec[x],s,a)
}
/^#/ { next; }
/^\[/ {
	gsub(/[\[\]]/,"")
	e = $1
	numarg[e] = NF-1 
	for(i=2;i<=NF;i++) {
		var[e,i-1]=" a"i-1
		typ[e,i-1]=$i
		x="va_arg(ap,"$i");\n"
		dec[e]=dec[e]"\t"$i var[e,i-1] "=" x
		arg=sprintf("%s,%s",arg,var[e,i-1])
	}
	next
}
!e{ next; }

e {
	body[e] = body[e]" "$0
}

END {
	for(e in numarg) print_error(e)
}

