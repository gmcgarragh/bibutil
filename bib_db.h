/*******************************************************************************
**
**    Copyright (C) 1998-2018 Greg McGarragh <greg.mcgarragh@colostate.edu>
**
**    This source code is licensed under the GNU General Public License (GPL),
**    Version 3.  See the file COPYING for more details.
**
*******************************************************************************/

#ifndef BIB_DB_H
#define BIB_DB_H

#include <gutil.h>
#include <gstruct.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct bib_abbrev_type_ {
     char *name;
     char *value;
} bib_abbrev_type;


typedef struct bib_field_type_ {
     int type;
     int flag;
     char *value;
} bib_field_type;


typedef struct bib_entry_type_ {
     int type;
     char *key;
     gdclist *field_list;
} bib_entry_type;


typedef struct bib_db_type_ {
     gdclist *abbrev_list;
     gdclist *entry_list;
} bib_db_type;


void bib_abbrev_init(bib_abbrev_type *);
int bib_abbrev_alloc(bib_abbrev_type *);
bib_abbrev_type *bib_abbrev_create(char *, char *);
void bib_abbrev_free(bib_abbrev_type *);
void bib_abbrev_destroy(bib_abbrev_type *);
int bib_abbrev_comp(const void *, const void *);
int bib_abbrev_print(FILE *, bib_abbrev_type *);


void bib_field_init(bib_field_type *);
int bib_field_alloc(bib_field_type *);
bib_field_type *bib_field_create(int, int, char *);
void bib_field_free(bib_field_type *);
void bib_field_destroy(bib_field_type *);
int bib_field_comp(const void *, const void *);
int bib_field_print(FILE *, bib_field_type *, gdclist *abbrev_list, int);


void bib_entry_init(bib_entry_type *);
int bib_entry_alloc(bib_entry_type *);
bib_entry_type *bib_entry_create(int, char *);
void bib_entry_free(bib_entry_type *);
void bib_entry_destroy(bib_entry_type *);
int bib_entry_comp(const void *, const void *);
int bib_entry_print(FILE *, bib_entry_type *, gdclist *abbrev_list);


void bib_db_init(bib_db_type *);
int bib_db_alloc(bib_db_type *);
bib_db_type *bib_db_create();
void bib_db_free(bib_db_type *);
void bib_db_destroy(bib_db_type *);
int bib_db_print(FILE *, bib_db_type *, int flag);


#ifdef __cplusplus
}
#endif

#endif /* BIB_DB_H */
