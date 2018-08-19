/*******************************************************************************
**
**    Copyright (C) 1998-2018 Greg McGarragh <greg.mcgarragh@colostate.edu>
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


/*******************************************************************************
 *
 ******************************************************************************/
void bib_abbrev_init(bib_abbrev_type *bib_abbrev) {

     bib_abbrev->name  = NULL;
     bib_abbrev->value = NULL;
}



int bib_abbrev_alloc(bib_abbrev_type *bib_abbrev) {

     return 0;
}



bib_abbrev_type *bib_abbrev_create(char *name, char *value) {

     bib_abbrev_type *bib_abbrev;

     bib_abbrev = malloc(sizeof(bib_abbrev_type));

     bib_abbrev_init(bib_abbrev);
     bib_abbrev_alloc(bib_abbrev);

     bib_abbrev->name  = name;
     bib_abbrev->value = value;

     return bib_abbrev;
}



void bib_abbrev_free(bib_abbrev_type *bib_abbrev) {

     free(bib_abbrev->name);
     free(bib_abbrev->value);
}



void bib_abbrev_destroy(bib_abbrev_type *bib_abbrev) {

     bib_abbrev_free(bib_abbrev);

     free(bib_abbrev);
}



int bib_abbrev_comp(const void *v1, const void *v2) {

     return strcmp(((bib_abbrev_type *) v1)->name, ((bib_abbrev_type *) v2)->name);
}



int bib_abbrev_print(FILE *fp, bib_abbrev_type *bib_abbrev) {

     int n = 0;

     n += fprintf(fp, "@string{%s = \"%s\"}\n",
                  bib_abbrev->name, bib_abbrev->value);

     return n;
}



/*******************************************************************************
 *
 ******************************************************************************/
static const char *field_names[] = {
     "address",
     "annote",
     "author",
     "booktitle",
     "chapter",
     "crossref",
     "doi",
     "edition",
     "editor",
     "eid",
     "howpublished",
     "isbn",
     "isbn-13",
     "institution",
     "journal",
     "key",
     "keywords",
     "location",
     "month",
     "note",
     "number",
     "numpages",
     "organization",
     "pages",
     "publisher",
     "school",
     "series",
     "title",
     "type",
     "volume",
     "year",
     "url"
};


void bib_field_init(bib_field_type *bib_field) {

     bib_field->type = -1;
     bib_field->flag =  0;

     bib_field->value = NULL;
}



int bib_field_alloc(bib_field_type *bib_field) {

     return 0;
}



bib_field_type *bib_field_create(int type, int flag, char *value) {

     bib_field_type *bib_field;

     bib_field = malloc(sizeof(bib_field_type));

     bib_field_init(bib_field);
     bib_field_alloc(bib_field);

     bib_field->type  = type;
     bib_field->flag  = flag;
     bib_field->value = value;

     return bib_field;
}



void bib_field_free(bib_field_type *bib_field) {

     free(bib_field->value);
}



void bib_field_destroy(bib_field_type *bib_field) {

     bib_field_free(bib_field);

     free(bib_field);
}



int bib_field_comp(const void *v1, const void *v2) {

     return strcmp(field_names[((bib_field_type *) v1)->type - ADDRESS],
                   field_names[((bib_field_type *) v2)->type - ADDRESS]);
}



int bib_field_print(FILE *fp, bib_field_type *bib_field, gdclist *abbrev_list, int flag) {

     int n = 0;

     gdcelem *elem;

     bib_abbrev_type abbrev;

      if (! bib_field->flag)
          n += fprintf(fp, "\t%-12s = {%s}",
                       field_names[bib_field->type - ADDRESS], bib_field->value);
     else {
          if (! abbrev_list)
               n += fprintf(fp, "\t%-12s = %s",
                            field_names[bib_field->type - ADDRESS], bib_field->value);
          else {
               abbrev.name = bib_field->value;

               if (! (elem = gdclist_find_comp(abbrev_list, &abbrev))) {
                    fprintf(stderr, "ERROR: abbreviation not found: %s\n", bib_field->value);
                    exit(1);
               }

               n += fprintf(fp, "\t%-12s = {%s}",
                            field_names[bib_field->type - ADDRESS], ((bib_abbrev_type *) gdcelem_value(elem))->value);
          }
     }

     if (! flag)
          fprintf(fp, ",\n");
     else
          fprintf(fp, "\n");

     return n;
}



/*******************************************************************************
 *
 ******************************************************************************/
static const char *entry_names[] = {
     "article",
     "book",
     "booklet",
     "conference",
     "inbook",
     "incollection",
     "inproceedings",
     "manual",
     "mastersthesis",
     "misc",
     "phdthesis",
     "proceedings",
     "techreport",
     "unpublished"
};


void bib_entry_init(bib_entry_type *bib_entry) {

     bib_entry->type = -1;

     bib_entry->field_list = NULL;
}



int bib_entry_alloc(bib_entry_type *bib_entry) {

     if ((bib_entry->field_list = gdclist_create()) == NULL)
          return -1;
     gdclist_set_comp(bib_entry->field_list, (int (*)(const void *, const void *)) bib_field_comp);
     gdclist_set_free(bib_entry->field_list, (void (*)(void *)) bib_field_destroy);

     return 0;
}



bib_entry_type *bib_entry_create(int type, char *key) {

     bib_entry_type *bib_entry;

     bib_entry = malloc(sizeof(bib_entry_type));

     bib_entry_init(bib_entry);
     bib_entry_alloc(bib_entry);

     bib_entry->type = type;
     bib_entry->key  = key;

     return bib_entry;
}



void bib_entry_free(bib_entry_type *bib_entry) {

     free(bib_entry->key);

     gdclist_free(bib_entry->field_list);
}



void bib_entry_destroy(bib_entry_type *bib_entry) {

     bib_entry_free(bib_entry);

     free(bib_entry);
}



int bib_entry_comp(const void *v1, const void *v2) {

     return strcmp(((bib_entry_type *) v1)->key, ((bib_entry_type *) v2)->key);
}



int bib_entry_print(FILE *fp, bib_entry_type *bib_entry, gdclist *abbrev_list) {

     int n = 0;

     int flag = 0;

     gdcelem *elem;
     gdcelem *first;

     n += fprintf(fp, "@%s{%s,\n",
                  entry_names[bib_entry->type - ARTICLE], bib_entry->key);

     if ((first = gdclist_first(bib_entry->field_list))) {
          elem = first;
          do {
               if (elem->next == first)
                    flag = 1;
               n += bib_field_print(fp, gdcelem_value(elem), abbrev_list, flag);
          } while ((elem = gdcelem_advance(elem)) != first);
     }

     n += fprintf(fp, "}\n");

     return n;
}



/*******************************************************************************
 *
 ******************************************************************************/
void bib_db_init(bib_db_type *bib_db) {

     bib_db->abbrev_list = NULL;
     bib_db->entry_list  = NULL;
}



int bib_db_alloc(bib_db_type *bib_db) {

     if ((bib_db->abbrev_list = gdclist_create()) == NULL)
          return -1;
     gdclist_set_comp(bib_db->abbrev_list, (int (*)(const void *, const void *)) bib_abbrev_comp);
     gdclist_set_free(bib_db->abbrev_list, (void (*)(void *)) bib_abbrev_destroy);

     if ((bib_db->entry_list  = gdclist_create()) == NULL)
          return -1;
     gdclist_set_comp(bib_db->entry_list, (int (*)(const void *, const void *)) bib_entry_comp);
     gdclist_set_free(bib_db->entry_list, (void (*)(void *)) bib_entry_destroy);

     return 0;
}



bib_db_type *bib_db_create() {

     bib_db_type *bib_db;

     bib_db = malloc(sizeof(bib_db_type));

     bib_db_init(bib_db);
     bib_db_alloc(bib_db);

     return bib_db;
}



void bib_db_free(bib_db_type *bib_db) {

     gdclist_free(bib_db->abbrev_list);
     gdclist_free(bib_db->entry_list);
}



void bib_db_destroy(bib_db_type *bib_db) {

     bib_db_free(bib_db);

     free(bib_db);
}



int bib_db_print(FILE *fp, bib_db_type *bib_db, int flag) {

     int n = 0;

     gdcelem *elem;
     gdcelem *first;

     gdclist *abbrev_list = NULL;

     if (! flag) {
          if ((first = gdclist_first(bib_db->abbrev_list))) {
               elem = first;
               do {
                    n += bib_abbrev_print(fp, gdcelem_value(elem));
               } while ((elem = gdcelem_advance(elem)) != first);

               n += fprintf(fp, "\n");
          }
     }
     else
          abbrev_list = bib_db->abbrev_list;

     if ((first = gdclist_first(bib_db->entry_list))) {
          elem = first;
          do {
               n += bib_entry_print(fp, gdcelem_value(elem), abbrev_list);
               n += fprintf(fp, "\n");
          } while ((elem = gdcelem_advance(elem)) != first);
     }

     return n;
}
