# An awk script processes this file to produce
# C function definitions that print the errors
# with the given arguments. To generate, run
# 'make error-msg.c'

[unbound array*] 
Unbound variable "%1"

[rdonly array*]
"%1" is read-only. You can look, but you can't
touch~

[syntax]
That doesn't make sense.

[bind array*]
Could not bind "%1". I can only remember so many
things at once (don't laugh, I bet you can't
remember more than 7!).

[paren]
Not balancing your parenthesis is like not
finishing your

[lexline int]
Maximum line length of %1 exceeded.

[lexmem int]
Scanner memory exhausted (max %1 bytes).

[lexerr]
Garbled input.

[nomem]
You must construct additional RAM modules.
