# Build dependencies
- awk
- make (GNU or BSD)
- C compiler (tcc and gcc are used for development)
- [libbio,libfmt,libutf](http://swtch.com/plan9port/unix/)

# Build instructions

	cd src
	make

You may need to tweak the CC,LD,CFLAGS and LDFLAGS
variables in the Makefile. Also, if your C compiler
does not support flexible array members like the example

	struct foo { int x; char m[] };

You'll need to tweak the declaration for array and ASIZE
in apl.h. The source is developed using gcc and tcc, on
both x86 and x86_64 platforms.

# An open-source, unicode APL interpreter

This project is my endeavour to create an open-source APL
interpreter that is useful for every-day use.  I want this
interpreter to be used much in the same way as 'awk' is used
in the Unix environment; for one-off scripts, to poke and
prod at data.  To this end, I have the following design
goals in mind:

### Tiny, simple core. 

More informative of this interpreter is the list of features it
WON'T have: no object system.  No foreign function interface.
No graphical interface.  No networking functionality.

The one feature it will have is the ability to spawn
processes and write to their stdin/stdout.  With this
feature, most other functionality can be implemented using
helper programs.

### Unicode all the way down.

APL caught a lot of flak for its use of non-ascii characters
that required a special encoding.  Many APL systems used their
own private encoding, making it difficult for users of different
APL systems to share programs.

These days Unicode is pretty wide-spread, and the encoding
problem is no longer a problem anymore.  This interpreter
uses unicode for everything.  Identifiers, primitives,
box-drawing characters, everything.  So

	nước ← 'USA'⊃'日本'⊃'Việt Nam'⊃'Россия' 
	右 ← ⊢ 
	左 ← ⊣

Are all examples of valid code.  There is no special IDE or
terminal that is required to input the characters; most
modern operating systems provide methods to input unicode
characters.  Documentation and keymaps will be provided with
the distribution for inputting the common APL characters,
and a dedicated terminal for each OS may (or may not) be
written.  I tend to do just fine just using rxvt-unicode or
9term on linux, but for windows a dedicated ide may be in
order.

### Pipes

As mentioned before, this interpreter has the ability to
spawn os commands and manipulate their stdin/stdout.  Using
this we can implement many features that are usually baked
into the core of the language or loaded as libraries, like:

- Matrix visualization 
- Function plotting 
- Networking (via `netcat` or similar)

### Compliance with 'A Dictionary of APL'

['A dictionary of APL'][1] was written by the creator of APL
himself and is a good formal standard to follow.  This
interpreter will conform to this standard where possible.
Notably, bracketed array indexing/assignment(a[n;]) will *not*
be in the language in favor of the 'from' ({) function and
'merge' (}) operator.  All functions will have the notion of
function rank as well.

I may diverge from this standard in a few areas.  For example
I'm still undecided on function definition; the Dictionary uses
the function ∇ with string arguments for the monadic and dyadic
cases.  I find this clumsy.  I like Dyalog's Dfns, but the '{}'
symbols they use for delimiters are already in use as
functions/operators.

## State of the project

Work in progress - rudimentary lexer and parser are complete.
Unified array data structure is implemented.  Basic memory
management based on ref-counting implemented.  Handy
error-reporting system is in place. Function definition syntax
using bracket delimiters is recognized.  Multi-dimensional
arrays are printed correctly, as are arbitrarily nested boxed
arrays using unicode box-drawing characters.

Most of the boring stuff is out of the way.  I am still planning
out the core execution loop and function dispatch/definition.
One-by-one standard functions will be added.  The project is not
currently at a state where outside help would be beneficial, or
a fork useful.  The lexer, while trivial, may be of interest as
it accepts utf-8 input. An earlier version of the lexer also
output tokens in a fairly portable streaming format.

[1]:(http://www.jsoftware.com/papers/APLDictionary.htm)
