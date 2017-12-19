/******************************************************************************%
**
**    Copyright (C) 1998-2017 Greg McGarragh <gregm@atmos.colostate.edu>
**
**    This source code is licensed under the GNU General Public License (GPL),
**    Version 3.  See the file COPYING for more details.
**
*******************************************************************************/

#include <gutil.h>
#include <gstruct.h>

#include "bib_db.h"
#include "bibutil.h"
#include "bib_parse.tab.h"


#ifdef NAME
#undef NAME
#endif
#define NAME    "bibutil"

#ifdef VERSION
#undef VERSION
#endif
#define VERSION "0.01"


#define MAX_FILES 128


typedef struct {
     int help;
     int expand_fields;
     int merge_files;
     int subset_entries;
     int version;
} Options;


void usage();
void version();


extern FILE *yyin;
extern FILE *yyout;


int yyparse(locus_type *, bib_db_type *);


int main(int argc, char *argv[]) {

     char line[LN];

     char *pos;

     char *url;

     char *entrylist;

     char *bibfiles[MAX_FILES];
     char *filelists[MAX_FILES];
     char *keywords[MAX_FILES];

     int i;
     int n;

     int count;

     int n_bibfiles;
     int n_filelists;

     int ret_val;

     FILE *fp;
/*
     bib_field_type field2;
*/
     bib_db_type *db;
     bib_field_type *field;
     bib_entry_type *entry;

     gdcelem *elem;

     locus_type locus;

     Options o;


     /*-------------------------------------------------------------------------
      *
      *-----------------------------------------------------------------------*/
     o.help           = 0;
     o.expand_fields  = 0;
     o.merge_files    = 0;
     o.subset_entries = 0;
     o.version        = 0;

     n_bibfiles  = 0;
     n_filelists = 0;

     n = 0;
     for (i = 1; i < argc; ++i) {
          if (argv[i][0] == '-') {
               if (strcmp(argv[i], "-bibfile") == 0)
                    bibfiles[n_bibfiles++] = argv[++i];
               else if (strcmp(argv[i], "-expand_fields") == 0)
                    o.expand_fields = 1;

               else if (strcmp(argv[i], "-help") == 0) {
                    usage();
                    exit(0);
               }
               else if (strcmp(argv[i], "-merge_files") == 0) {
                    o.merge_files = 1;
                    filelists[n_filelists] = argv[++i];
                    keywords[n_filelists] = argv[++i];
                    n_filelists++;
               }
               else if (strcmp(argv[i], "-subset_entries") == 0) {
                    o.subset_entries = 1;
                    entrylist = argv[++i];
               }
               else if (strcmp(argv[i], "-version") == 0) {
                    version();
                    exit(0);
               }

               else if (strcmp(argv[i], "-version") == 0) {
                    version();
                    exit(0);
               }
               else {
                    printf("Invalid option: %s, use -help for more information\n", argv[i]);
                    exit(1);
               }
          }
/*
          else {
               if (n == 0)
                    ;
               else {
                    printf("Too many arguments, use -help for more information\n");
                    exit(1);
               }

               ++n;
          }
*/
     }

     if (n < 0) {
          printf("Not enough arguments, use -help for more information\n");
          exit(1);
     }


     /*-------------------------------------------------------------------------
      *
      *-----------------------------------------------------------------------*/
     db = bib_db_create();


     /*-------------------------------------------------------------------------
      *
      *-----------------------------------------------------------------------*/
     if (n_bibfiles == 0) {
          locus.file = "stdin";
          locus.line = 1;

          ret_val = yyparse(&locus, db);
     }
     else {
          for (i = 0; i < n_bibfiles; ++i) {
               if ((yyin = fopen(bibfiles[i], "r")) == NULL) {
                    fprintf(stderr, "ERROR: Problem opening file for reading: %s\n", bibfiles[i]);
                    fprintf(stderr, "  ... %s\n", strerror(errno));
                    exit(1);
               }

               locus.file = bibfiles[i];
               locus.line = 1;

               ret_val = yyparse(&locus, db);

               fclose(yyin);

               if (ret_val)
                    break;
          }
     }


     /*-------------------------------------------------------------------------
      *
      *-----------------------------------------------------------------------*/
     if (o.merge_files) {
          for (i = 0; i < n_filelists; ++i) {
               if ((fp = fopen(filelists[i], "r")) == NULL) {
                    fprintf(stderr, "ERROR: Problem opening file for reading: %s\n", filelists[i]);
                    fprintf(stderr, "  ... %s\n", strerror(errno));
                    exit(1);
               }

               count = 0;
               while (fgets(line, LN, fp)) {
                    line[strlen(line) - 1] = '\0';

                    url = strdup(line);

                    *(pos = strrchr(line, '/')) = '\0';

                    *strchr(pos + 1, '.') = '\0';

                    entry = bib_entry_create(ARTICLE, strdup(pos + 1));

                    if ((elem = gdclist_find_comp(db->entry_list, entry))) {
/*
                         fprintf(stderr, "ERROR: new entry key already in use: %s\n", pos + 1);
*/
                         bib_entry_destroy(entry);

                         entry = (bib_entry_type *) gdcelem_value(elem);
/*
                         field2.type = KEYWORDS;
                         if ((elem = gdclist_find_comp(entry->field_list, &field2)) == NULL) {
                              fprintf(stderr, "ERROR: no keyword field for: %s\n", entry->key);
                              exit(1);
                         }

                         field = (bib_field_type *) gdcelem_value(elem);

                         fprintf(stderr, "%-40s%-40s<%s>\n", line, pos + 1, field->value);
*/
                    }
                    else {
                         if (gdclist_insert_sort_comp(db->entry_list, entry) == NULL) {
                              fprintf(stderr, "ERROR: Problem adding new bibtex entry: %s\n", pos + 1);
                              exit(1);
                         }

                         if (keywords[i][0] != '\0') {
                              field = bib_field_create(KEYWORDS, 0, strdup(keywords[i]));

                              if (gdclist_insert_end(entry->field_list, field) == NULL) {
                                   fprintf(stderr, "ERROR: Problem adding keywords field "
                                                   "to new bibtex entry: %s\n", pos + 1);
                                   exit(1);
                              }
                         }
                    }

                    field = bib_field_create(URL, 0, url);

                    if (gdclist_insert_end(entry->field_list, field) == NULL) {
                         fprintf(stderr, "ERROR: Problem adding keywords field "
                                         "to new bibtex entry: %s\n", pos + 1);
                         exit(1);
                    }

                    count++;
               }

               fclose(fp);

               fprintf(stderr, "%d new entries added from: %s\n", count, filelists[i]);
          }

          bib_db_print(stdout, db, o.expand_fields);
     }
     else if (o.subset_entries) {
          if ((fp = fopen(entrylist, "r")) == NULL) {
               fprintf(stderr, "ERROR: Problem opening file for reading: %s\n", filelists[i]);
               fprintf(stderr, "  ... %s\n", strerror(errno));
               exit(1);
          }

          while (fgets(line, LN, fp)) {
               line[strlen(line) - 1] = '\0';

               if (line[0] == '\0')
                    continue;

               entry = bib_entry_create(ARTICLE, line);

               if ((elem = gdclist_find_comp(db->entry_list, entry))) {
                    entry = (bib_entry_type *) gdcelem_value(elem);
                    bib_entry_print(stdout, entry, o.expand_fields ? db->abbrev_list : NULL);
                    printf("\n");
               }
          }

          fclose(fp);
     }
     else
          bib_db_print(stdout, db, o.expand_fields);


     /*-------------------------------------------------------------------------
      *
      *-----------------------------------------------------------------------*/
     bib_db_destroy(db);


     exit(0);
}



void usage() {

}



void version() {

     printf("%s version %s\n", NAME, VERSION);
}
