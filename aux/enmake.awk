/^\[/ {
	gsub(/[\[\]]/,"")
	print("int E"$1"(va_list);")
}
