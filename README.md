# An open-source, unicode APL interpreter

This project is my endeavour to create an open-source APL
interpreter that is useful for every-day use.  I want this
interpreter to be used much in the same way as 'awk' is used
in the Unix environment; for one-off scripts, to poke and
prod at data.

# Build

	make

If you want to tweak compiler/linker flags, they are in
the file `config.mk`. This project has no dependencies
other than the C standard library. The source is developed
using gcc and tcc on Linux, but should compile on other
platforms.

# Install

	make install

By default this will create /usr/local/bin/apl. Modify `config.mk`
to change the install location.

### Tiny, simple core. 

More informative of this interpreter is the list of features it WON'T
have: no object system.  No foreign function interface.  No graphical
interface.  No networking functionality.

The one feature it will have is the ability to spawn processes and write
to their stdin/stdout.  With this feature, most other functionality can
be implemented using helper programs.

### Unicode all the way down.

APL caught a lot of flak for its use of non-ascii characters that required
a special encoding.  Many APL systems used their own private encoding,
making it difficult for users of different APL systems to share programs.

These days Unicode is pretty wide-spread, and nearly all software
supports the UTF-8 encoding. This interpreter uses the utf-8 encoding
for everything.  Identifiers, primitives, box-drawing characters,
everything.  So

	nước ← 'USA'⊃'日本'⊃'Việt Nam'⊃'Россия' 
	右 ← ⊢ 
	左 ← ⊣

Are all examples of valid code. There is no special IDE or terminal that
is required to input the characters; most modern operating systems provide
methods to input unicode characters. The shell script `doc/genxmodmap`
can be used to generate an `.Xmodmap` file that provides the APL Union
keyboard layout when the Alt key is pressed.

### Pipes

As mentioned before, this interpreter has the ability to spawn os commands
and manipulate their stdin/stdout.  Using this we can implement many
features that are usually baked into the core of the language or loaded
as libraries, like:

- Matrix visualization 
- Function plotting 
- Networking (via `netcat` or similar)

### Compliance with 'A Dictionary of APL'

['A dictionary of APL'][1] was written by the creator of APL himself
and is a good formal standard to follow.  This interpreter will
conform to this standard where possible.  Notably, bracketed array
indexing/assignment(a[n;]) will *not* be in the language in favor of the
'from' ({) function and 'merge' (}) operator.  All functions will have
the notion of function rank as well.

[1]:(http://www.jsoftware.com/papers/APLDictionary.htm)
