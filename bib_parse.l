/******************************************************************************%
**
**    Copyright (C) 1998-2017 Greg McGarragh <gregm@atmos.colostate.edu>
**
**    This source code is licensed under the GNU General Public License (GPL),
**    Version 3.  See the file COPYING for more details.
**
*******************************************************************************/

%{
#define BISON_INCLUDE

#include <gutil.h>

#include "bib_db.h"
#include "bibutil.h"
#include "bib_parse.tab.h"

#define YY_DECL int yylex(locus_type *locus, bib_db_type *db)

void yyerror(locus_type *, bib_db_type *, char const *);
%}


%array
     /* flex only */
%option stack


whitespace [ \t]


%%
{whitespace}+				;

%.*					;

string					{ return STRING; }

article					{ return ARTICLE; }
book					{ return BOOK; }
booklet					{ return BOOKLET; }
conference				{ return CONFERENCE; }
inbook					{ return INBOOK; }
incollection				{ return INCOLLECTION; }
inproceedings				{ return INPROCEEDINGS; }
manual					{ return MANUAL; }
mastersthesis				{ return MASTERSTHESIS; }
misc					{ return MISC; }
phdthesis				{ return PHDTHESIS; }
proceedings				{ return PROCEEDINGS; }
techreport				{ return TECHREPORT; }
unpublished				{ return UNPUBLISHED; }

address					{ return ADDRESS; }
annote					{ return ANNOTE; }
author					{ return AUTHOR; }
booktitle				{ return BOOKTITLE; }
chapter					{ return CHAPTER; }
crossref				{ return CROSSREF; }
doi					{ return DOI; }
edition					{ return EDITION; }
editor					{ return EDITOR; }
eid					{ return EID; }
howpublished				{ return HOWPUBLISHED; }
institution				{ return INSTITUTION; }
isbn					{ return ISBN; }
isbn-13					{ return ISBN_13; }
journal					{ return JOURNAL; }
key					{ return KEY; }
keywords				{ return KEYWORDS; }
location				{ return LOCATION; }
month					{ return MONTH; }
note					{ return NOTE; }
number					{ return NUMBER; }
numpages				{ return NUMPAGES; }
organization				{ return ORGANIZATION; }
pages					{ return PAGES; }
publisher				{ return PUBLISHER; }
school					{ return SCHOOL; }
series					{ return SERIES; }
title					{ return TITLE; }
type					{ return TYPE; }
volume					{ return VOLUME; }
year					{ return YEAR; }

url					{ return URL; }

\"([^\"\n]|(\\\"))*\"			{
     yytext[yyleng - 1] = '\0';
     yylval.s_val = strdup(yytext + 1);
     return GT_CONST_STRING;
}

[A-Za-z_][A-za-z0-9_\-:]*		{
     yylval.s_val = strdup(yytext);
     return GT_IDENTIFIER;
}

\n					{ locus->line++; }

.					{ return yytext[0]; }

%%



void yyerror(locus_type *locus, bib_db_type *db, char const *s) {

     printf("%s, line %d: %s at \'%s\'\n", locus->file, locus->line, s, yytext);

     exit(1);
}



int yywrap() {

     return 1;
}
