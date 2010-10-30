/* Version: MPL 1.1/GPL 2.0
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See
 * the License for the specific language governing rights and
 * limitations under the License.
 *
 * The Original Code is libUtility
 *
 * The Initial Developers of the Original Code are FumaSoftware Ltd, and Jamal Natour.
 * Portions created before 1-Jan-2007 00:00:00 GMT by Jamal Natour, are Copyright
 * (C) 2004-2007 Jamal Natour
 *
 * Portions created by Fumasoftware Ltd are Copyright (C) 2009-2010 FumaSoftware
 * Ltd.
 *
 * Portions created by Jamal Natour are Copyright (C) 2009-2010
 * Fumasoftware Ltd and Jamal Natour.
 *
 * All Rights Reserved.
 *
 * Contributor(s): ______________________________________.
 *
 * Alternatively, the contents of this file may be used under the terms
 * of the GNU General Public License Version 2 or later (the "GPL"), in
 * which case the provisions of the GPL are applicable instead of those
 * above. If you wish to allow use of your version of this file only
 * under the terms of the GPL, and not to allow others to use your
 * version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the
 * notice and other provisions required by the GPL. If you do not
 * delete the provisions above, a recipient may use your version of
 * this file under the terms of any one of the MPL or the GPL.
 *
 */

#ifndef UTILITY_STRING_H
#define UTILITY_STRING_H

#if defined( __cplusplus)
extern "C" {
#endif

#include "libUtility.h"

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


/* Downcase a string in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * EINVAL: invalid args passed*/
char *utility_downcase(char *s);

/* Upcase a string in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * EINVAL: invalid args passed*/
char *utility_upcase(char *s);

/* Reverse a string in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * EINVAL: invalid args passed*/
char *utility_reverse(char *s,size_t len);

/* Duplicates a string reversing byte order
 * Returns NULL on error setting errno
 * Returns pointer to allocated reversed copy of 's' on success,
 * the caller must free this memory
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), utility_reverse) */
char *utility_strndup_reverse(const char *s, int len);

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


/* Duplicates a string converting to lowercase
 * Returns NULL on error setting errno
 * Returns pointer to allocated downcased copy of 's' on success,
 * the caller must free this memory
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), utility_downcase() */
char * utility_strndup_downcase(const char *s, int len);

/* Duplicates a string converting to uppercase
 * Returns NULL on error setting errno
 * Returns pointer to allocated uppercased copy of 's' on success,
 * the caller must free this memory
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), utility_upcase() */
char * utility_strndup_upcase(const char *s, int len);

/* Converts a substring to its integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int utility_sntoi(int *pdest,const char *s,int len);

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

/* Converts a substring to its long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), strtol() */
int utility_sntol(
        long *pdest,
        const char *s,
        size_t len
        );

/* Converts a string to its unsigned long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int utility_stoul(
        unsigned long *pdest,
        const char *s
        );

/* Converts a string to its long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int utility_stol(long *pdest,const char *s);

/* Converts a substring to its unsigned long long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * strtoull() */
int utility_sntoull(
        uint64_t *pdest,
        const char *s,
        size_t len
        );

/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int utility_stoui(unsigned int *pdest,const char *s);

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int utility_sntoui8(uint8_t *pdest,const char *s,int len);

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int utility_sntoui16(uint16_t *pdest,const char *s,int len);

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int utility_sntoui32(uint32_t *pdest,const char *s,int len);

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int utility_sntoui64(uint64_t *pdest,const char *s,int len);

/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoui8() */
int utility_stoui8(uint8_t *pdest,const char *s);

/* Converts a string to its unsigned integer representation;
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoui16() */
int utility_stoui16(uint16_t *pdest,const char *s) ;

/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoui32() */
int utility_stoui32(uint32_t *pdest,const char *s) ;

/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoui64() */
int utility_stoui64(uint64_t *pdest,const char *s);

/* Converts a string to its integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int utility_stoi(int *pdest,const char *s);

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

/* these should really be in Utility_pack
 *
 *----------------------------------------------*/

/* Converts a substring to its integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * util_eInvalidArguments: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int
utility_sntoi(
        int *pdest,
        const char *s,
        int len);

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int utility_sntoui(
        unsigned int *pdest,
        const char *s,
        size_t len
        );


/* Converts a string to its unsigned long long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int
utility_stoull(
        unsigned long long *pdest,
        const char *s
        );


#if defined( __cplusplus)
    }
#endif

/*
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#endif /* ndef UTILITY_STRING_H */

