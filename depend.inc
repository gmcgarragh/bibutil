bib_db.o: bib_db.c bib_db.h bibutil.h bib_parse.tab.h
bib_parse.tab.o: bib_parse.tab.c bib_db.h bibutil.h
bib_parse.yy.o: bib_parse.yy.c bib_db.h bibutil.h bib_parse.tab.h
bibutil.o: bibutil.c bib_db.h bibutil.h bib_parse.tab.h
gstruct.o: gstruct.c gutil.h gstruct.h gstruct_dclist.h
