/^\[/ {
	gsub(/[\[\]]/,"")
	print("int E"$1"();")
}
