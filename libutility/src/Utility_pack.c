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

#if defined( __cplusplus)
extern "C" {
#endif

#include "libUtility.h"

#if !defined (VERBOSE_HASH_DEBUG)
    #undef  utility_trace
    #undef  utility_wrap_return_int
    #undef  utility_wrap_return_ptr
    #define utility_trace(a,b)
    #define utility_wrap_return_int(ret) return (ret)
    #define utility_wrap_return_ptr(ret) return (ret)
#endif


/* pack intel order lengths with strings */
/* Packs a zero-terminated string into a byte buffer prefixed with 'len'
* Returns -1 on error setting errno
* Returns number of bytes converted (1+len) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* ERANGE: conversion would underflow or would be > max_value
* Function may fail and set p2p_get_errno() for same reasons as
**/
int
utility_pack_c1S(void *ptr, size_t off, const char *s, uint8_t len)
{
    size_t sz = len;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !s || util_eError == utility_pack_str(ptr, (off + sizeof(len)), s,sz))
        return util_eError;

    ((uint8_t*)ptr)[off]  = len;
    return sizeof(len) + sz;
}

/* Packs a zero-terminated string into a byte buffer
* Returns -1 on error setting errno
* Returns number of bytes copied (len) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* ERANGE: conversion would underflow or would be > max_value
* Function may fail and set p2p_get_errno() for same reasons as
**/
int
utility_pack_c2S(void *ptr, size_t off, const char *s, uint16_t len)
{
    size_t sz = len;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !s ||
            util_eError == utility_pack_c2(ptr,off,len) ||
            util_eError == utility_pack_str(ptr, (off + sizeof(len)), s,sz))
        return util_eError;

    return sizeof(len) + sz;
}
/* Packs a zero-terminated string into a byte buffer prefixed with 'len' in intel order
* Returns -1 on error setting errno
* Returns number of bytes converted (4+len+1) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* Function may fail and set p2p_get_errno() for same reasons as
**/
int
utility_pack_c4S(void *ptr, size_t off, const char *s, uint32_t len)
{
    size_t sz = len;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !s ||
            util_eError == utility_pack_c4(ptr,off,len) ||
            util_eError == utility_pack_str(ptr, (off + sizeof(len)), s,sz))
        return util_eError;

    return sizeof(len) + sz;
}

/* Unpacks a length in intel order and a char buffer from byte buffer
* Returns NULL on error setting errno
* Returns on success,
*  pointer to string on length '*len',  the caller must free this memory
*      storing: length of string including terminating null into 'len'
*      returning allocated copy of 'len' bytes starting 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* Function may fail and set p2p_get_errno() for same reason as utility_strndup()
**/
char *
utility_unpack_c1S(uint8_t *len, void *ptr, size_t off)
{
    char  *val;
    uint8_t sz;
    p2p_set_errno(util_eInvalidArguments);

    sz = *( ((uint8_t*)ptr)+off);
    val = utility_strndup((((char*)ptr)+off+sizeof(sz)),sz);
    if(val)
        *len = sz;
    return val;
}

/* Unpacks a length in intel order and a char buffer from byte buffer
* Returns NULL on error setting errno
* Returns on success,
*  pointer to string on length '*len',  the caller must free this memory
*      storing: length of string including terminating null into 'len'
*      returning allocated copy of 'len' bytes starting 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* Function may fail and set p2p_get_errno() for same reason as utility_strndup()
**/
char *
utility_unpack_c2S(uint16_t *len, void *ptr, size_t off)
{
    char  *val;
    uint16_t sz;
    p2p_set_errno(util_eInvalidArguments);
    if(util_eError == utility_unpack_c2(&sz,ptr,off))
        return NULL;

    val = utility_strndup((((char*)ptr)+off+sizeof(sz)),sz);
    if(val)
        *len = sz;
    return val;
}

/* Unpacks a length in intel order and a char buffer from byte buffer
* Returns NULL on error setting errno
* Returns on success,
*  pointer to string on length '*len',  the caller must free this memory
*      storing: length of string including terminating null into 'len'
*      returning allocated copy of 'len' bytes starting 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* Function may fail and set p2p_get_errno() for same reason as utility_strndup()
**/
char *
utility_unpack_c4S(uint32_t *len, void *ptr, size_t off)
{
    char  *val;
    uint32_t sz;
    p2p_set_errno(util_eInvalidArguments);
    if(util_eError == utility_unpack_c4(&sz,ptr,off))
        return NULL;

    val = utility_strndup((((char*)ptr)+off+sizeof(sz)),sz);
    if(val)
        *len = sz;
    return val;
}
/* Packs a zero-terminated string into a byte buffer
* Returns -1 on error setting errno
* Returns number of bytes copied (len) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* ERANGE: conversion would underflow or would be > max_value
* Function may fail and set p2p_get_errno() for same reasons as
**/
int
utility_pack_n2S(void *ptr, size_t off, const char *s, uint16_t len)
{
    size_t sz = len;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !s ||
            util_eError == utility_pack_n2(ptr,off,len) ||
            util_eError == utility_pack_str(ptr, (off + sizeof(len)), s,sz))
        return util_eError;

    return sizeof(len) + sz;
}

/* Packs a zero-terminated string into a byte buffer prefixed with 'len' in network order
* Returns -1 on error setting errno
* Returns number of bytes converted (4+len+1) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* Function may fail and set p2p_get_errno() for same reasons as
**/
int
utility_pack_n4S(void *ptr, size_t off, const char *s, uint32_t len)
{
    size_t sz = len;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !s)
        return util_eError;

    if(util_eError == utility_pack_n4(ptr,off,len))
        return util_eError;

    if(util_eError == utility_pack_str(ptr, (off + sizeof(len)), s,sz))
        return util_eError;

    return sizeof(len) + len;
}

/* Unpacks a length in network order and a char buffer from byte buffer
* Returns NULL on error setting errno
* Returns on success,
*  pointer to string on length '*len',  the caller must free this memory
*      storing: length of string including terminating null into 'len'
*      returning allocated copy of 'len' bytes starting 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* Function may fail and set p2p_get_errno() for same reason as utility_strndup()
**/
char *
utility_unpack_n2S(uint16_t *len, void *ptr, size_t off)
{
    char  *val;
    uint16_t sz;
    p2p_set_errno(util_eInvalidArguments);
    if(util_eError == utility_unpack_n2(&sz,ptr,off))
        return NULL;

    val = utility_strndup((((char*)ptr)+off+sizeof(sz)),sz);
    if(val)
        *len = sz;
    return val;
}

/* Unpacks a length in network order and a char buffer from byte buffer
* Returns NULL on error setting errno
* Returns on success,
*  pointer to string on length '*len',  the caller must free this memory
*      storing: length of string including terminating null into 'len'
*      returning allocated copy of 'len' bytes starting 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
* Function may fail and set p2p_get_errno() for same reason as utility_strndup()
**/
char *
utility_unpack_n4S(uint32_t *len, void *ptr, size_t off)
{
    char  *val;
    uint32_t sz;
    p2p_set_errno(util_eInvalidArguments);
    if(util_eError == utility_unpack_n4(&sz,ptr,off))
        return NULL;

    val = utility_strndup((((char*)ptr)+off+sizeof(sz)),sz);
    if(val)
        *len = sz;
    return val;
}


/* Packs a uint16_t into a intel order byte buffer
* Returns -1 on error setting errno
* Returns number of bytes converted (2) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
**/
int
utility_pack_c2(void *ptr, size_t off, uint16_t v)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr)
        return util_eError;

    val = ((uint8_t*)ptr)+off;
    val[1]  = (UINT8_MAX & (v >> CHAR_BIT));
    val[0]  = (UINT8_MAX & v);
    return sizeof(v);
}

/* Packs a uint32 into a intel order byte buffer
* Returns -1 on error setting errno
* Returns number of bytes converted (4) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
**/
int
utility_pack_c4(void *ptr, size_t off, uint32_t v)
{
    p2p_set_errno(util_eInvalidArguments);
    if(ptr)
        return util_eError;

    uint8_t  *val = ((uint8_t*)ptr)+off;
    val[3]  = (UINT8_MAX & (v >> CHAR_BIT*3));
    val[2]  = (UINT8_MAX & (v >> CHAR_BIT*2));
    val[1]  = (UINT8_MAX & (v >> CHAR_BIT*1));
    val[0]  = (UINT8_MAX & v);
    return sizeof(v) ;
}

/* Packs a uint64 into a intel order byte buffer
* Returns -1 on error setting errno
* Returns number of bytes converted (8) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
**/
int
utility_pack_c8(void *ptr, size_t off, uint64_t v)
{
    p2p_set_errno(util_eInvalidArguments);
    uint8_t  *val;
    if(!ptr)
        return util_eError;

    val = ((uint8_t*)ptr)+off;
    val[7]  = (UINT8_MAX & (v >> CHAR_BIT*7));
    val[6]  = (UINT8_MAX & (v >> CHAR_BIT*6));
    val[5]  = (UINT8_MAX & (v >> CHAR_BIT*5));
    val[4]  = (UINT8_MAX & (v >> CHAR_BIT*4));
    val[3]  = (UINT8_MAX & (v >> CHAR_BIT*3));
    val[2]  = (UINT8_MAX & (v >> CHAR_BIT*2));
    val[1]  = (UINT8_MAX & (v >> CHAR_BIT*1));
    val[0]  = (UINT8_MAX & v);
    return sizeof(v) ;
}

/* Unpacks a uint32_t from intel order byte buffer
* Returns -1 on error setting errno
* Returns  0 on success,
*      storing (4) bytes starting 'off' bytes into 'ptr' interpereted as uint32_t
*      into 'dst'
* util_eInvalidArguments: invalid args passed
**/
int
utility_unpack_c4(uint32_t *dst,const void *ptr, size_t off)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !dst)
        return util_eError;

    val = ((uint8_t*)ptr)+off;
    *dst |= ((UINT32_MAX & val[3]) << (CHAR_BIT * 3)) ;
    *dst |= ((UINT32_MAX & val[2]) << (CHAR_BIT * 2)) ;
    *dst |= ((UINT32_MAX & val[1]) << (CHAR_BIT * 1)) ;
    *dst |= (UINT32_MAX  & val[0]);
    return util_eOk;
}


/* Unpacks a uint16_t from intel order byte buffer
* Returns -1 on error setting errno
* Returns  0 on success,
*      storing (2) bytes starting 'off' bytes into 'ptr' interpereted as uint16_t
*      into 'dst'
* util_eInvalidArguments: invalid args passed
**/
int
utility_unpack_c2(uint16_t *dst,const void *ptr, size_t off)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !dst)
        return util_eError;

    val = ((uint8_t*)ptr)+off;

    *dst |= ((UINT16_MAX & val[1]) << (CHAR_BIT * 1)) ;
    *dst |= (UINT16_MAX & val[0]);
    return util_eOk;
}


/* Unpacks a uint64_t from intel order byte buffer
* Returns -1 on error setting errno
* Returns  0 on success,
*      storing (8) bytes starting 'off' bytes into 'ptr' interpereted as uint64_t
*      into 'dst'
* util_eInvalidArguments: invalid args passed
**/
int
utility_unpack_c8(uint64_t *dst,const void *ptr, size_t off)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !dst)
        return util_eError;

    val = ((uint8_t*)ptr)+off;

    *dst |= ((UINT64_MAX & val[7]) << (CHAR_BIT * 7)) ;
    *dst |= ((UINT64_MAX & val[6]) << (CHAR_BIT * 6)) ;
    *dst |= ((UINT64_MAX & val[5]) << (CHAR_BIT * 5)) ;
    *dst |= ((UINT64_MAX & val[4]) << (CHAR_BIT * 4)) ;
    *dst |= ((UINT64_MAX & val[3]) << (CHAR_BIT * 3)) ;
    *dst |= ((UINT64_MAX & val[2]) << (CHAR_BIT * 2)) ;
    *dst |= ((UINT64_MAX & val[1]) << (CHAR_BIT * 1)) ;
    *dst |= (UINT64_MAX & val[0]);
    return util_eOk;
}


/* Packs a uint16_t into a network order byte buffer
 * Returns -1 on error setting errno
 * Returns number of bytes converted (2) on success with value bytes stored 'off' bytes into 'ptr'
 * util_eInvalidArguments: invalid args passed
 * ERANGE: conversion would underflow or would be > max_value
 * Function may fail and set p2p_get_errno() for same reasons as
 **/
int
utility_pack_n2(void *ptr, size_t off, uint16_t v)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr)
        return util_eError;

    val = ((uint8_t*)ptr)+off;

    val[0]  = (UINT8_MAX & (v >> CHAR_BIT));
    val[1]  = (UINT8_MAX & v);
    return sizeof(v);
}


/* Packs a uint32 into a network order byte buffer
* Returns -1 on error setting errno
* Returns number of bytes converted (4) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
**/
int
utility_pack_n4(void *ptr, size_t off, uint32_t v)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr)
        return util_eError;

    val = ((uint8_t*)ptr)+off;

     val[0]  = (UINT8_MAX & (v >> CHAR_BIT*3));
    val[1]  = (UINT8_MAX & (v >> CHAR_BIT*2));
    val[2]  = (UINT8_MAX & (v >> CHAR_BIT*1));
    val[3]  = (UINT8_MAX & v);
    return sizeof(v) ;
}


/* Packs a uint64 into a network order byte buffer
* Returns -1 on error setting errno
* Returns number of bytes converted (8) on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
**/
int
utility_pack_n8(void *ptr, size_t off, uint64_t v)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr)
        return util_eError;

    val = ((uint8_t*)ptr)+off;

    val[0]  = (UINT8_MAX & (v >> CHAR_BIT*7));
    val[1]  = (UINT8_MAX & (v >> CHAR_BIT*6));
    val[2]  = (UINT8_MAX & (v >> CHAR_BIT*5));
    val[3]  = (UINT8_MAX & (v >> CHAR_BIT*4));
    val[4]  = (UINT8_MAX & (v >> CHAR_BIT*3));
    val[5]  = (UINT8_MAX & (v >> CHAR_BIT*2));
    val[6]  = (UINT8_MAX & (v >> CHAR_BIT*1));
    val[7]  = (UINT8_MAX & v);
    return sizeof(v) ;
}



/* Unpacks a uint16_t from network order byte buffer
* Returns -1 on error setting errno
* Returns  0 on success,
*      storing (2) bytes starting 'off' bytes into 'ptr' interpereted as uint16_t
*      into 'dst'
* util_eInvalidArguments: invalid args passed
**/
int
utility_unpack_n2(uint16_t *dst,const void *ptr, size_t off)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !dst)
        return util_eError;

    val = ((uint8_t*)ptr)+off;
    *dst |= ((UINT16_MAX & val[0]) << (CHAR_BIT * 1)) ;
    *dst |= (UINT16_MAX & val[1]);
    return util_eOk;
}



/* Unpacks a uint32_t from network order byte buffer
* Returns -1 on error setting errno
* Returns  0 on success,
*      storing (4) bytes starting 'off' bytes into 'ptr' interpereted as uint32_t
*      into 'dst'
* util_eInvalidArguments: invalid args passed
**/
int
utility_unpack_n4(uint32_t *dst,const void *ptr, size_t off)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !dst)
        return util_eError;

    val = ((uint8_t*)ptr)+off;
    *dst |= ((UINT32_MAX & val[0]) << (CHAR_BIT * 3)) ;
    *dst |= ((UINT32_MAX & val[1]) << (CHAR_BIT * 2)) ;
    *dst |= ((UINT32_MAX & val[2]) << (CHAR_BIT * 1)) ;
    *dst |= (UINT32_MAX & val[3]);
    return util_eOk;
}

/* Unpacks a uint64_t from network order byte buffer
* Returns -1 on error setting errno
* Returns  0 on success,
*      storing (8) bytes starting 'off' bytes into 'ptr' interpereted as uint64_t
*      into 'dst'
* util_eInvalidArguments: invalid args passed
**/
int
utility_unpack_n8(uint64_t *dst,const void *ptr, size_t off)
{
    uint8_t  *val;
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !dst)
        return util_eError;

    val = ((uint8_t*)ptr)+off;
    *dst |= ((UINT64_MAX & val[0]) << (CHAR_BIT * 7)) ;
    *dst |= ((UINT64_MAX & val[1]) << (CHAR_BIT * 6)) ;
    *dst |= ((UINT64_MAX & val[2]) << (CHAR_BIT * 5)) ;
    *dst |= ((UINT64_MAX & val[3]) << (CHAR_BIT * 4)) ;
    *dst |= ((UINT64_MAX & val[4]) << (CHAR_BIT * 3)) ;
    *dst |= ((UINT64_MAX & val[5]) << (CHAR_BIT * 2)) ;
    *dst |= ((UINT64_MAX & val[6]) << (CHAR_BIT * 1)) ;
    *dst |= (UINT64_MAX & val[7]);
    return util_eOk;
}


/* Packs a zero-terminated string into a byte buffer
* Returns -1 on error setting errno
* Returns 0 on success with value bytes stored 'off' bytes into 'ptr'
* util_eInvalidArguments: invalid args passed
**/
int
utility_pack_str(void *ptr, size_t off, const char *s,size_t len)
{
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr || !s)
        return util_eError;

    memcpy(((char*)ptr)+off,s,len);
    return util_eOk;
}


/* Simple string hash function
* based on a description of various hashing functions
* Returns -1 on error setting errno
* Returns 0 on success with 32 bit hash stored in 'dest'
* util_eInvalidArguments: invalid args passed
**/
int
utility_hash_string(uint32_t *dest, const char *s, int len)
{
    uint64_t n=129 + len;
    if(!s || !dest)
        utility_trace_return_int(util_eError,"Utility: %s ","hashing aborted as arguments are invalid");

    /* based on a description of various hashing functions
    */
    if(len >= 3)
    {
        n *=  ~((n *  s[0] ^ 337)<<14) + (s[2] ^ 57531)+len  ;
    }
    if(len >= 2)
    {
        n *=  s[1] ^ 535137 ;
        n ^= (n *  s[0] ^ 337)<<14 ;
    }
    if(len >= 1)
    {
        n ^= (n *  s[0] ^ 337)<<14 ;
    }

    for(; --len >=0; )
        n ^= (s[len] * (0x735357 ^ (s[len] * 0x5533 + (n<<2))));

    *dest = ((0x137517  * ((n & UINT32_MAX) * ((n >> 9) & UINT32_MAX)) & UINT32_MAX) ^ (*s + len));
    utility_trace("Utility: hashing  produced [%u]",*dest);
    utility_trace_return_int(util_eOk,"Utility: hashing  [%s] completed",s);
}

/* Produces an argc/argv array from a string, original is not touched
 * Returns NULL on error setting errno
 * Returns pointer to list of string pointers with number of entries stored in 'pargc'
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set p2p_get_errno() for same reasons as
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
        )
{
    char *tmp,*search,*found;
    char **argv;
    size_t count=0;
    p2p_set_errno(util_eInvalidArguments);

    /* count occurences of char and  test for overflow as need to increment counter to accomadate sentinal  */
    if( (!pargc || !s) || (util_eError == utility_count_ch(&count,s,ch)) || (count+1 < count))
           return NULL;

    /* sentinal is not present, and string is empty */
    if(!count && !strcmp("",s))
           return NULL;

    /* allocate array of string pointers plus sentinal */
    argv = calloc(count+2,sizeof(char *));
    if(!argv)
       return NULL;

    tmp = utility_strndup(s,strlen(s)+1);
    if(!tmp)
    {
        free(argv);
        p2p_set_errno(ENOMEM);
        return NULL;
    }

    count=0;
    /*restart the search one byte in from last entry */
    for(search = tmp; ((found = strchr(search,ch))); ++count)
    {
        argv[count] = search;
        search = found +1;
    }

    argv[count] = search;

    /* terminate the array with a sentinal */
    argv[ ++count ] = NULL;
    *pargc = count;

    /* zero-terminate all the sections */
    count = 0;
    utility_replace_ch(tmp,&count,ch,'\0');
    return argv;
}

/* Produces an array of values s  argc/argv array from a string, original is not touched
 * Returns NULL on error setting errno
 * Returns pointer to list of values from tring pointers with number of entries stored in 'pargc'
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set p2p_get_errno() for same reasons as
 * strchr()
 * calloc()
 * */
size_t  *
utility_series(
        size_t min,
        size_t count
        )
{
    size_t *values;
    size_t n=0;

    p2p_set_errno(util_eInvalidArguments);
    if( !count)
       return NULL;

    values = malloc((count * sizeof(size_t)));
    if(!values)
       return NULL;

    for(n=0; (n < count); ++n)
        values[n] = (n + min);

    return values;
}

/* Count occurences of character 'ch' in string 's'
 * Returns -1 on error setting errno
 * Returns  count in 'dest'
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set p2p_get_errno() for same reasons as
 * */
int
utility_count_ch(
        size_t *dest,
        const char *s,
        int ch)
{
    size_t count=0;
    char *search;
    p2p_set_errno(util_eInvalidArguments);
    if(!s || !dest)
        return util_eError;

    /* count occurences of char */
    for(search = (char *)s;  (search=strchr(search,ch)); ++count)
        ++search;

    *dest = count;
    return util_eOk;
}

/* Convert buffer inplace to have unix line-endings, when interpreted as 8-bit ascii
 * CR+LF or CR becomes LF
 * LF is unchanged
 * TAB becomes SPACE
 * Returns NULL on error setting errno
 * Returns s on success with number of linefeeds seen, stored in dest
 * util_eInvalidArguments: invalid args passed
 * */
char *
utility_unixify(
        size_t *dest,
        char *s,
        size_t len
        )
{
    size_t cr=0,lf=0,ws=0,spc=0,i;
    char *p;

    p2p_set_errno(util_eInvalidArguments);
    if( !s || !len || !dest)
           return NULL;

    for(i=0;  i<len && *(p=s+i); ++i)
        switch(*p)
        {
            /* squeeze tabs and spaces into single space */
            case 0x09:  *p = 0x20; /*fall through */
            case 0x20 : for(spc=0;  ((*(p+1+spc)) && isspace((*(p+1+spc)))); ++spc)
                            /*(*(p+1+spc)) = '\0'; */
                            ;
                        if(spc)
                            memmove(p+1,  p+1+(spc),len-(i+spc));
                        *p = 0x20;
                        ++ws;
                        i += spc;
                        len -= spc;
                        break;

            /* turn CR  or CR+LF into LF  stripping lines consisting of blanks */
            case 0x0D : ++cr;
                        for(spc=0;  ((*(p+1+spc)) && isspace((*(p+1+spc)))); ++spc)
                            /*(*(p+1+spc)) = '\0'; */
                            ;

                        if(spc)
                            memmove(p+1,  p+1+(spc),len-(i+spc));
                        *p = 0x0A;
                        ++ws;
                        i += spc;
                        len -= spc;
                        /* fall through */

            /* squeeze lines that are empty or wholely consist of control characters  */
            case 0x0A : ++lf;
                        for(spc=0;  ((*(p+1+spc)) && isspace((*(p+1+spc)))); ++spc)
                            /*(*(p+1+spc)) = '\0'; */
                            ;
                        if(spc)
                            memmove(p+1,  p+1+(spc),len-(i+spc));
                        *p = 0x0A;
                        ++ws;
                        i += spc;
                        len -= spc;
            default   : break;
        }
    *dest = lf;
    return s;
}

/* Produces an argc/argv array from a buffer, buffer is converted inplace to have unix line-endings,
 * Returns NULL on error setting errno
 * Returns pointer to list of string pointers with number of entries stored in 'pargc'
 * util_eInvalidArguments: invalid args passed
 * ENOMEM: allocating argv array failed, however buffer has been *modified*
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_unixify()
 * utility_count_ch()
 * utility_replace_ch()
 * strchr()
 * calloc()
 * */
char **
utility_unix_ln_split(
       size_t *pargc,
        char *s,
        size_t len
        )
{
    size_t nlines,count=0;
    char *found,*search;
    char **argv;

    p2p_set_errno(util_eInvalidArguments);
    if( (!pargc || !s || !len) ||  (!utility_unixify(&nlines,s,len)) )
        return NULL;

    /* allocate array of string pointers plus sentinal */
    argv = calloc(nlines+1,sizeof(char *));
    if(!argv)
       return NULL;

    /*restart the search one byte in from last entry */
    argv[count] = s;
    for(search = s+1; ((found = strchr(search,0x0A)));)
    {
        search = argv[++count] = found+1;
        *found = '\0'; 
    }
    /* terminate the array with a sentinal */
    argv[ count ] = NULL;
    *pargc = nlines;
    count = 0;

    /* zero-terminate all the sections */
    utility_replace_ch(s,&count,0x0A,'\0');

    return argv;
}

/* Call a func for each element in the list of lines returned for slurping file and
 * treating result as a newline terminated array of char *, i.e. argv style
 * returns -1 setting p2p_get_errno() on error
 * returns -2 prologue canceled file operation
 * returns -3 visitor canceled file operation
 * util_eInvalidArguments: invalid arguments */
int
utility_fname_visit(
        size_t *dst_nlines,
        const char *filename,
        void *state,
        int (*prologue)(const char *filename,size_t nlines,void *state),
        int (*visitor)(void *parg,size_t index, void *state)
        )
{
    char *buf,
         **argv;
    size_t argc,
           len;
    int ret;

    p2p_set_errno(util_eInvalidArguments);
    if(!(dst_nlines && filename && prologue && visitor))
        return util_eError;

    /* newline terminated file of whitespace delimited tokens */
    buf = utility_slurp_with_sz(&len,filename);
    if(!buf)
        return util_eError;

    /* convert buffer in place to array of lines */
    argv = utility_unix_ln_split(&argc,buf,len);
    if(!argv)
    {
        free(buf);
        buf = NULL;
        return util_eError;
    }

    /* if prolouge didn't cancel on us,
     * walk the list, passing each element in turn to visitor */
    ret = (*prologue)(filename,argc,state);
    if(!ret && (*dst_nlines = argc))

    for(len = 0; len < argc; ++len)
        (*visitor)(argv[len],len,state);

    /* clean up list and wrappers */
    free(argv);
    argv = NULL;

    free(buf);
    buf = NULL;

    return ret;
}

uint8_t
reverse_8(uint8_t arg)
{
int s = ((sizeof(arg) * CHAR_BIT)-1);/* number of bits to process*/
uint8_t  ret=0;                      /* temp for the reversed bit pattern */
	/* build the reversed pattern by grabbing bits from the bottom of arg
           .. and shifting into correct position in ret */
	while(s--)
	{
		ret = ((ret | (arg & 1)) << 1)  ;
		arg >>= 1;
        }
	/* finally set the LSB on ret to obtain a reversed byte */
	return (ret | arg) ;
}

uint16_t
reverse_16(uint16_t arg)
{
	/* 0 reversed is still zero and all bits set is still all bits set */
	if(UINT16_MAX == arg || !arg)
		return arg;


	/* reverse_8 returns an 8bit value so the reversed low byte of arg
     ... must be cast to a 16bit value or bits are lost by shifting */
	return reverse_8(arg >> (sizeof(uint8_t) * CHAR_BIT)) |
	((uint16_t)reverse_8(arg & UINT8_MAX)) << (sizeof(uint8_t) * CHAR_BIT);
}

uint32_t
reverse_32(uint32_t arg)
{
	/* 0 reversed is still zero and all bits set is still all bits set */
	if(UINT32_MAX == arg || !arg)
		return arg;

	/* reverse_16 returns an 16bit value so the reversed low halfword of arg
	  ... must be cast to a 32bit value or bits are lost by shifting */
	return reverse_16(arg >> (sizeof(uint16_t) * CHAR_BIT)) |
	( (uint32_t)reverse_16(arg & UINT16_MAX) ) << (sizeof(uint16_t) * CHAR_BIT);
}

uint64_t
reverse_64(uint64_t arg)
{
	/* 0 reversed is still zero and all bits set is still all bits set */
	if(UINT64_MAX == arg || !arg)
		return arg;

	/* reverse_32 returns an 32bit value so the reversed low halfword of arg
	  ... must be cast to a 64bit value or bits are lost by shifting */
	return reverse_32(arg >> (sizeof(uint32_t) * CHAR_BIT)) |
	( (uint64_t)reverse_32(arg & UINT32_MAX) ) << (sizeof(uint32_t) * CHAR_BIT);
}



#if defined( __cplusplus)
}
#endif

