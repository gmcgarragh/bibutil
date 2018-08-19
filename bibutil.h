/*******************************************************************************
**
**    Copyright (C) 1998-2018 Greg McGarragh <greg.mcgarragh@colostate.edu>
**
**    This source code is licensed under the GNU General Public License (GPL),
**    Version 3.  See the file COPYING for more details.
**
*******************************************************************************/

#ifndef BIBUTIL_H
#define BIBUTIL_H

#include <gutil.h>
#include <gstruct.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct locus_type_ {
     char *file;
     int  line;
} locus_type;


#ifdef __cplusplus
}
#endif

#endif /* BIBUTIL_H */
