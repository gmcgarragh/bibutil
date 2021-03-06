DESCRIPTION
------------
bibutil is a bibtex file reader, parser, and writer that puts the contents into a simple data structure.  The idea is to be able to read the file, make systematic changes, and write it back out with the changes.  Currently the output form is fixed but this can be made configurable.  My primary purpose was just to make my .bib file consistent.  There is only a few changes that are now possible but more changes can be easily added as the main parts, reading, parsing, and writing, are in place and are relatively clean.  New optional actions can be added to main() for whatever you want to do.  Lex is used for the lexicographical analyser generation.  YACC is used for the grammar analyser generation.  In this case the bibtex language is simple enough that YACC doesn't get in the way as it often can.


INSTALLATION
------------
The source maybe obtained by git cloning from github.com with:

git clone https://github.com/gmcgarragh/bibutil.git

The main code is C89 compliant and is dependent only on the C standard library, flex, and yacc which should be apart of any Linux distribution.  The code should compile with any modern C compiler and probably with most C++ compilers.  The build system requires GNU Make, with which the following steps will compile the code:

1) Copy make.inc.example to make.inc.
2) Edit make.inc according to the comments within.
3) Run make.

The default setup in make.inc.example is for GCC and should work on any Linux or MacOS systems with programming tools installed and probably most modern UNIX systems.

For other platforms/environments, such as MS Windows Visual C++, it is up to user to set up the build within that platform/environment.

After the build the binary will be located in the same directory as the source.  It is up to the user to move it to or link to it from other locations.


USAGE
-----
Sorry, but there is no documentation yet.  I will try to do this soon or maybe not.  But, this is not geared towards users.  It is geared toward developers.  So, go to the ultimate source of documentation -- the code.


CONTACT
-------
For questions, comments, or bug reports contact Greg McGarragh at mcgarragh@atm.ox.ac.uk.

Bug reports are greatly appreciated especially if they are fixed!  If you would like to report a bug please include the inputs that produce the bug and expected outputs.
