# An awk script processes this file to produce
# C source code that is handled by the functions
# in error.c

[unbound array*] 
The variable `%1' does not refer to any function, operator, or value. In
fact, I don't even know what `%1' means. What the hell kind of word is
`%1', anyway?

[rdonly array*]
`%1' is read-only. You can look, but you can't touch~

[malloc]
You've not enough memory. You must construct additional RAM modules.

[syntax]
I know you think what you just wrote made sense, but it confused the
hell out of me. Can we try this again?

[bind array*]
Could not bind `%1'. I can only remember so many things at once (don't
laugh, I bet you can't remember more than 7!).

[paren]
Balance your parenthesis!

[lexline int]
Maximum line length of %1 exceeded.

[lexmem]
Scanner memory exhausted. Either you put "War and Peace" on one line,
or my creator was lazy and used stack allocation.

[testfun array* int char*]
First array %1 then char %3 then array %1 then int %2 then char %3 then array "%1"
