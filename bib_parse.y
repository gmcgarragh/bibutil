/******************************************************************************%
**
**    Copyright (C) 1998-2017 Greg McGarragh <gregm@ssec.wisc.edu>
**
**    This source code is licensed under the GNU General Public License (GPL),
**    Version 3.  See the file COPYING for more details.
**
*******************************************************************************/

%{
#include <gutil.h>

#include "bib_db.h"
#include "bibutil.h"
/*
#define YYLEX_PARAM	locus, db
*/

int yylex(locus_type *locus, bib_db_type *db);
void yyerror(locus_type *locus, bib_db_type *db, char const *);

void add_abbrev(locus_type *, bib_db_type *, char *, char *);
void add_entry(locus_type *, bib_db_type *, int, char *, gdclist *);
%}


%lex-param	{locus_type *locus}
%lex-param	{bib_db_type *db}

%parse-param	{locus_type *locus}
%parse-param	{bib_db_type *db}


%union {
     char           *s_val;
     long           l_val;
     bib_field_type *field;
     gdclist        *list;
}


%token STRING

%token ARTICLE
%token BOOK
%token BOOKLET
%token CONFERENCE
%token INBOOK
%token INCOLLECTION
%token INPROCEEDINGS
%token MANUAL
%token MASTERSTHESIS
%token MISC
%token PHDTHESIS
%token PROCEEDINGS
%token TECHREPORT
%token UNPUBLISHED

%token ADDRESS
%token ANNOTE
%token AUTHOR
%token BOOKTITLE
%token CHAPTER
%token CROSSREF
%token DOI
%token EDITION
%token EDITOR
%token EID
%token HOWPUBLISHED
%token ISBN
%token ISBN_13
%token INSTITUTION
%token JOURNAL
%token KEY
%token KEYWORDS
%token LOCATION
%token MONTH
%token NOTE
%token NUMBER
%token NUMPAGES
%token ORGANIZATION
%token PAGES
%token PUBLISHER
%token SCHOOL
%token SERIES
%token TITLE
%token TYPE
%token VOLUME
%token YEAR

%token URL

%token <s_val>	GT_CONST_STRING
%token <l_val>	GT_CONST_INTEGER

%token <s_val>	GT_IDENTIFIER


%type <l_val>	entry_type

%type <list>	field_list
%type <field>	field
%type <l_val>	field_type

%%
/*------------------------------------------------------------------------------
 *
 *----------------------------------------------------------------------------*/
start			: entry
			| start entry
;

entry			: '@' STRING '{' GT_IDENTIFIER '=' GT_CONST_STRING '}'	{ add_abbrev(locus, db, $4, $6); }
			| '@' STRING '(' GT_IDENTIFIER '=' GT_CONST_STRING ')'	{ add_abbrev(locus, db, $4, $6); }
			| '@' entry_type '{' GT_IDENTIFIER ',' field_list '}'	{ add_entry(locus, db, $2, $4, $6); }
			| '@' entry_type '(' GT_IDENTIFIER ',' field_list ')'	{ add_entry(locus, db, $2, $4, $6); }
;

entry_type		: ARTICLE						{ $$ = ARTICLE; }
			| BOOK							{ $$ = BOOK; }
			| BOOKLET						{ $$ = BOOKLET; }
			| CONFERENCE						{ $$ = CONFERENCE; }
			| INBOOK						{ $$ = INBOOK; }
			| INCOLLECTION						{ $$ = INCOLLECTION; }
			| INPROCEEDINGS 					{ $$ = INPROCEEDINGS; }
			| MANUAL						{ $$ = MANUAL; }
			| MASTERSTHESIS 					{ $$ = MASTERSTHESIS; }
			| MISC							{ $$ = MISC; }
			| PHDTHESIS						{ $$ = PHDTHESIS; }
			| PROCEEDINGS						{ $$ = PROCEEDINGS; }
			| TECHREPORT						{ $$ = TECHREPORT; }
			| UNPUBLISHED						{ $$ = UNPUBLISHED; }
;

field_list		:
			| field							{ $$ = gdclist_create();
                                                                                  gdclist_set_comp($$, (int (*)(const void *, const void *)) bib_field_comp);
                                                                                  gdclist_set_free($$, (void (*)(void *)) bib_field_destroy);
/*
										  if (gdclist_insert_end($$, $1) == NULL)
										       yyerror(locus, db, "error adding bibtex field");
*/
										  if (gdclist_insert_sort_comp($$, $1) == NULL)
										       yyerror(locus, db, "error adding bibtex field");
										}
			| field_list ',' field					{
/*
										  if (gdclist_insert_end($$, $3) == NULL)
										       yyerror(locus, db, "error adding bibtex field");
*/
										  if (gdclist_insert_sort_comp($$, $3) == NULL)
										       yyerror(locus, db, "error adding bibtex field");
										}
;

field			: field_type '=' GT_CONST_STRING			{ $$ = bib_field_create($1, 0, $3); }
			| field_type '=' GT_IDENTIFIER				{ $$ = bib_field_create($1, 1, $3); }
;

field_type		: ADDRESS						{ $$ = ADDRESS; }
			| ANNOTE						{ $$ = ANNOTE; }
			| AUTHOR						{ $$ = AUTHOR; }
			| BOOKTITLE						{ $$ = BOOKTITLE; }
			| CHAPTER						{ $$ = CHAPTER; }
			| CROSSREF						{ $$ = CROSSREF; }
			| DOI							{ $$ = DOI; }
			| EDITION						{ $$ = EDITION; }
			| EDITOR						{ $$ = EDITOR; }
			| EID							{ $$ = EID; }
			| HOWPUBLISHED  					{ $$ = HOWPUBLISHED; }
			| ISBN		   					{ $$ = ISBN; }
			| ISBN_13		   				{ $$ = ISBN_13; }
			| INSTITUTION   					{ $$ = INSTITUTION; }
			| JOURNAL						{ $$ = JOURNAL; }
			| KEY							{ $$ = KEY; }
			| KEYWORDS						{ $$ = KEYWORDS; }
			| LOCATION 						{ $$ = LOCATION; }
			| MONTH 						{ $$ = MONTH; }
			| NOTE							{ $$ = NOTE; }
			| NUMBER						{ $$ = NUMBER; }
			| NUMPAGES						{ $$ = NUMPAGES; }
			| ORGANIZATION						{ $$ = ORGANIZATION; }
			| PAGES 						{ $$ = PAGES; }
			| PUBLISHER						{ $$ = PUBLISHER; }
			| SCHOOL						{ $$ = SCHOOL; }
			| SERIES						{ $$ = SERIES; }
			| TITLE 						{ $$ = TITLE; }
			| TYPE							{ $$ = TYPE; }
			| VOLUME						{ $$ = VOLUME; }
			| YEAR							{ $$ = YEAR; }
			| URL							{ $$ = URL; }
;

%%



void add_abbrev(locus_type *locus, bib_db_type *db, char *name, char *value) {

     bib_abbrev_type *abbrev;

     abbrev = bib_abbrev_create(name, value);

     if (gdclist_insert_sort_comp(db->abbrev_list, abbrev) == NULL)
          yyerror(locus, db, "error adding bibtex abbrev");
}



void add_entry(locus_type *locus, bib_db_type *db, int type, char *key, gdclist *list) {

     bib_entry_type *entry;
/*
     entry = bib_entry_create(type, key);

     entry->field_list = list;
*/
     entry = malloc(sizeof(bib_entry_type));

     bib_entry_init(entry);

     entry->type = type;
     entry->key  = key;

     entry->field_list = list;

     if (gdclist_find_comp(db->entry_list, entry))
          yyerror(locus, db, "duplicate bibtex entry key");

     if (gdclist_insert_end(db->entry_list, entry) == NULL)
          yyerror(locus, db, "error adding bibtex entry" );
/*
     if (gdclist_insert_sort_comp(db->entry_list, entry) == NULL)
          yyerror(locus, db, "error adding bibtex entry" );
*/
}
