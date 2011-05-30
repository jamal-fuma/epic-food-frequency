/* This file is part of the libEpic library */

/*
libEpic is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libEpic is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef EPIC_UTIL_H
#define EPIC_UTIL_H

#include <errno.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

enum { util_eError = -1, util_eOk = 0};


#if defined( __cplusplus)
extern "C" {
#endif


/* Strndup for systems that don't have it,
 * uses host implementation if natively supported
 * otherwise uses PGC's lean mean implementation
 * Returns NULL on error setting errno
 * Returns pointer to allocated copy of 's'  on success, the caller must free this memory
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reason as malloc()*/
char *utility_strndup(
        const char *s,
        size_t len);


/* Convert buffer inplace to have unix line-endings, i.e. CR+LF or CR becomes LF, while LF is unchanged
 * Returns NULL on error setting errno
 * Returns s on success with number of linefeeds seen, stored in dest
 * EINVAL: invalid args passed
 * */
char *
utility_unixify(
        size_t *dest,
        char *s,
        size_t len
        );
/* Replace every occurence of 'old' with 'new' in the string 's'
 *
 * Returns -1 on error setting errno
 * Returns  0 on success
 * EINVAL: invalid args passed*/
int
utility_replace_ch(
        char *haystack,
        size_t *dest_count,
        int needle,
        int ch
        );
/* Converts a substring to its unsigned long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), strtoul() */
int utility_sntoul(
        unsigned long *pdest,
        const char *s,
        size_t len
        );

/* Converts a substring to its unsigned long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * strtoul() */
int
utility_sntol(
        long *pdest,
        const char *s,
        size_t len
        );

/* Count occurences of character 'ch' in string 's'
 * Returns -1 on error setting errno
 * Returns  count in 'dest'
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * */
int
utility_count_ch(
        size_t *dest,
        const char *s,
        int ch);

/* Produces an argc/argv array from a string, original is not touched
 * Returns NULL on error setting errno
 * Returns pointer to list of string pointers with number of entries stored in 'pargc'
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_count_ch()
 * utility_replace_ch()
 * utility_strndup()
 * strchr()
 * calloc()
 * */
char **
utility_split(
        size_t *pargc,
        const char *s,
        int ch
        );

int
utility_same_double(
        const double a,
        const double b
        );
int
utility_diff_within_delta(
        const double diff,
        const double delta
        );


#if defined( __cplusplus)
    }
#endif


#endif  /* #ifndef EPIC_UTIL_H */
