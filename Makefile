#*******************************************************************************
#
# Copyright (C) 2014-2017 Greg McGarragh <gregm@atmos.colostate.edu>
#
# This source code is licensed under the GNU General Public License (GPL),
# Version 3.  See the file COPYING for more details.
#
#*******************************************************************************
.SUFFIXES: .c

OBJECTS = 

EXTRA_CLEANS = bindx_yylex_int.c bindx_yylex_int.h

include make.inc

all: bibutil

bibutil: bib_db.o bibutil.o bib_parse.yy.o bib_parse.tab.o gstruct.o gutil_error.o
	${CC} ${CCFLAGS} ${CCDEFINES} -o bibutil bib_db.o bibutil.o bib_parse.yy.o bib_parse.tab.o gstruct.o gutil_error.o \
        ${INCDIRS} ${LIBDIRS} ${LINKS}

bib_parse.yy.c: bib_parse.l bib_parse.tab.h
	flex -i -obib_parse.yy.c bib_parse.l

bib_parse.tab.h bib_parse.tab.c: bib_parse.y
	bison -d bib_parse.y

README: readme_source.txt
	fold --spaces --width=80 readme_source.txt > README
	sed -i 's/[ \t]*$$//' README

clean:
	rm -f *.o bibutil *.yy.c *.tab.c *.tab.h $(EXTRA_CLEANS)

.c.o:
	$(CC) $(CCFLAGS) $(INCDIRS) -I. -c -o $*.o $<

include dep.inc
