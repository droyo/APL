# An awk script processes this file to produce
# C source code that is handled by the functions
# in error.c

[unbound var] 
The variable `$var' does not refer to any function, operator, or value. In fact, I don't even know what `$var' means. What the hell kind of word is `$var', anyway?

[rdonly var]
`$var' is read-only. You can look, but you can't touch~

[malloc]
Tell your OS to give me moar memory. Or add RAM, 'cause I'm all out.


[syntax]
I know you think what you just wrote made sense, but it confused the hell out of me. Can we try this again?

[bind name val]
Could not bind `$name'. I can only remember so many things at once (don't laugh, I bet you can't remember more than 7!). 

[paren]
Unbalanced parenthesis

[lexline max]
Maximum line length of $max exceeded.

[lexmem]
Scanner memory exhausted. Either you put "War and Peace" on one line, or my creator was lazy and used stack allocation.