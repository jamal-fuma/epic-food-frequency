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
#ifndef LIB_UTILITY_PACK_H
#define LIB_UTILITY_PACK_H

#if defined( __cplusplus)
extern "C" {
#endif

#include "libUtility.h"


/* Packs a zero-terminated string into a byte buffer prefixed with 'len'
* Returns -1 on error setting errno
* Returns number of bytes converted (sizeof(len)+len) on success with value bytes stored 'off' bytes into 'ptr'
* EINVAL: invalid args passed
**/
int     utility_pack_c1S(void *ptr, size_t off, const char *s, uint8_t len);
int     utility_pack_c2S(void *ptr, size_t off, const char *s, uint16_t len);
int     utility_pack_c4S(void *ptr, size_t off, const char *s, uint32_t len);

/* Unpacks a length in intel order and a char buffer from byte buffer
* Returns NULL on error setting errno
* Returns on success,
*  pointer to string on length '*len',  the caller must free this memory
*      storing: length of string including terminating null into 'len'
*      returning allocated copy of 'len' bytes starting 'off' bytes into 'ptr'
* EINVAL: invalid args passed
* Function may fail and set errno for same reason as utility_strndup()
**/
char *  utility_unpack_c1S(uint8_t *len, void *ptr, size_t off);
char *  utility_unpack_c2S(uint16_t *len, void *ptr, size_t off);
char *  utility_unpack_c4S(uint32_t *len, void *ptr, size_t off);

/* Packs a zero-terminated string into a byte buffer prefixed with 'len'
* Returns -1 on error setting errno
* Returns number of bytes converted (sizeof(len)+len) on success with value bytes stored 'off' bytes into 'ptr'
* EINVAL: invalid args passed
**/
#define utility_pack_n1S utility_pack_c1S
int     utility_pack_n2S(void *ptr, size_t off, const char *s, uint16_t len);
int     utility_pack_n4S(void *ptr, size_t off, const char *s, uint32_t len);

/* Unpacks a length in network order and a char buffer from byte buffer
* Returns NULL on error setting errno
* Returns on success,
*  pointer to string on length '*len',  the caller must free this memory
*      storing: length of string including terminating null into 'len'
*      returning allocated copy of 'len' bytes starting 'off' bytes into 'ptr'
* EINVAL: invalid args passed
* Function may fail and set errno for same reason as utility_strndup()
**/
#define utility_unpack_n1S utility_unpack_c1S
char *  utility_unpack_n2S(uint16_t *len, void *ptr, size_t off);
char *  utility_unpack_n4S(uint32_t *len, void *ptr, size_t off);

/* pack intel order values */
/* Packs a value into a intel order byte buffer
* Returns -1 on error setting errno
* Returns number of bytes converted (sizeof(v)) on success with value bytes stored 'off' bytes into 'ptr'
* EINVAL: invalid args passed
**/
int     utility_pack_c2(void *ptr, size_t off, uint16_t v);
int     utility_pack_c4(void *ptr, size_t off, uint32_t v);
int     utility_pack_c8(void *ptr, size_t off, uint64_t v);

/* unpack intel order values */
int     utility_unpack_c4(uint32_t *dst,const void *ptr, size_t off);
int     utility_unpack_c2(uint16_t *dst,const void *ptr, size_t off);
int     utility_unpack_c8(uint64_t *dst,const void *ptr, size_t off);

/* pack network order values */
/* Packs a value into a intel order byte buffer
* Returns -1 on error setting errno
* Returns number of bytes converted (sizeof(v)) on success with value bytes stored 'off' bytes into 'ptr'
* EINVAL: invalid args passed
**/
int     utility_pack_n2(void *ptr, size_t off, uint16_t v);
int     utility_pack_n4(void *ptr, size_t off, uint32_t v);
int     utility_pack_n8(void *ptr, size_t off, uint64_t v);

/* Unpacks a value from network order byte buffer
* Returns -1 on error setting errno
* Returns  0 on success,
*      storing (sizeof(v)) bytes starting 'off' bytes into 'ptr'
*      into 'dst'
* EINVAL: invalid args passed
*/
int     utility_unpack_n2(uint16_t *dst,const void *ptr, size_t off);
int     utility_unpack_n4(uint32_t *dst,const void *ptr, size_t off);
int     utility_unpack_n8(uint64_t *dst,const void *ptr, size_t off);

/* Packs a zero-terminated string into a byte buffer
* Returns -1 on error setting errno
* Returns 0 on success with value bytes stored 'off' bytes into 'ptr'
* EINVAL: invalid args passed
**/
int     utility_pack_str(void *ptr, size_t off, const char *s,size_t len);

/* Simple string hash function
* based on a description of various hashing functions
* Returns -1 on error setting errno
* Returns 0 on success with 32 bit hash stored in 'dest'
* EINVAL: invalid args passed
**/
int utility_hash_string(uint32_t *dest, const char *s, int len);

/* Produces an argc/argv array from a buffer, buffer is converted inplace to have unix line-endings,
 * Returns NULL on error setting errno
 * Returns pointer to list of string pointers with number of entries stored in 'pargc'
 * EINVAL: invalid args passed
 * ENOMEM: allocating argv array failed, however buffer has been *modified*
 * Function may fail and set errno for same reasons as
 * utility_unixify()
 * utility_replace_ch()
 * strchr()
 * calloc()
 * */
char **
utility_unix_ln_split(
        size_t *pargc,
        char *s,
        size_t len
        );

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
#if defined( __cplusplus)
    }
#endif

#endif /* ndef LIB_UTILITY_PACK_H */
