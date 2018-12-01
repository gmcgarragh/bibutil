#*******************************************************************************
#
# Copyright (C) 2014-2018 Greg McGarragh <greg.mcgarragh@colostate.edu>
#
# This source code is licensed under the GNU General Public License (GPL),
# Version 3.  See the file COPYING for more details.
#
#*******************************************************************************
.SUFFIXES: .c

include make.inc

INCDIRS += -I.

OBJECTS = bib_db.o \
          bibutil.o \
          bib_parse.yy.o \
          bib_parse.tab.o \
          gstruct.o

EXTRA_CLEANS = bib_parse.tab.c bib_parse.tab.h bib_parse.yy.c

all: bibutil

bibutil: bib_db.o bibutil.o bib_parse.yy.o bib_parse.tab.o gstruct.o
	$(CC) $(CCFLAGS) -o bibutil $(OBJECTS) \
        $(INCDIRS) $(LIBDIRS) $(LINKS)

bib_parse.yy.c: bib_parse.l bib_parse.tab.h
	flex -i -obib_parse.yy.c bib_parse.l

bib_parse.tab.h bib_parse.tab.c: bib_parse.y
	bison -d bib_parse.y

README: readme_source.txt
	fold --spaces --width=80 readme_source.txt > README
	sed -i 's/[ \t]*$$//' README

clean:
	rm -f *.o bibutil $(EXTRA_CLEANS)

.c.o:
	$(CC) $(CCFLAGS) $(INCDIRS) -c -o $*.o $<

# Requires gcc
depend:
	@files=`find . -maxdepth 1 -name "*.c" | sort`; \
        if test $${#files} != 0; then \
             if (eval gcc -v 1> /dev/null 2>&1); then \
                  echo gcc -MM -w $$files "> depend.inc"; \
                  gcc -MM -w $$files > dep.inc; \
             else \
                  echo makedepend -f- -Y -- -- $$files "> depend.inc"; \
                  makedepend -f- -Y -- -- $$files > dep.inc; \
             fi \
        else \
             echo -n '' > dep.inc; \
        fi

include depend.inc
