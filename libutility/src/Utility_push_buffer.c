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


struct util_push_buf_tag
{
    FILE   *fp;			/* input stream */
    char   *storage;		/* underlying storage */
    const char *fname;          /* filename for fopen/fdreopen */
    size_t  len;		/* maximum number of elements in buffer */
    size_t  idx;		/* read/write head  */
};

/* Allocate a new push back buffer
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 *
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/
util_push_buf_t *
utility_push_buf_alloc(
        void
        )
{
util_push_buf_t *push_buf;

    push_buf = malloc(sizeof(*push_buf));
    if(!push_buf)
        return NULL;

    push_buf->fp = NULL;
    push_buf->storage = NULL;
    push_buf->fname = NULL;
    push_buf->idx = 0;
    push_buf->len = 0;
    return push_buf;
}

/* Release an existing push back buffer
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 *
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/

void
utility_push_buf_release(
        util_push_buf_t *push_buf
        )
{
    if(push_buf)
    {
        utility_push_buf_fclose(push_buf);
        free(push_buf);
        push_buf = NULL;
    }
}

/* Retrieve filename used to open underlying stream

 * Returns NULL on error setting errno
 * Returns valid pointer on success
 *
 * EINVAL: invalid args passed
 * function may fail and set errno for same reason
 * utility_file_exists()
 */
const char *
utility_push_buf_fname(
        util_push_buf_t *push_buf
        )
{
    errno = EINVAL;
    return ( !push_buf || (1 > utility_file_exists(push_buf->fname))) ? NULL : push_buf->fname;
}

/* Bind a file to the push back buffer

 * Returns NULL on error setting errno
 * Returns valid pointer on success
 *
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/
int
utility_push_buf_fopen(
        util_push_buf_t *push_buf,
        const char *fname
        )
{
    errno = EINVAL;
    if((1 > utility_file_exists(fname)) || !push_buf)
      return util_eError;

    push_buf->fp = utility_freopen(fname,"rb",push_buf->fp,1);
    if(!push_buf->fp)
        return util_eError;

    push_buf->fname = fname;
    return util_eOk ;
}

/* Unbind a file from the push back buffer
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 *
 * EINVAL: invalid args passed*/
int
utility_push_buf_fclose(
        util_push_buf_t *push_buf
        )
{
    errno = EINVAL;
    if(!push_buf)
        return util_eError;

    if(push_buf->fp)
    {
        fclose(push_buf->fp);
        push_buf->fp = NULL;
    }
    return util_eOk;
}

/* Flush push back buffer
 *
 * Returns -1 on error setting errno
 * Returns  0 on success
 *
 * EINVAL : push_buf is null
 * Function may fail and set errno for same reasons
 * */
int
utility_push_buf_flush(
        util_push_buf_t *push_buf
        )
{
    errno = EINVAL;
    if(!push_buf || !push_buf->storage)
        return util_eError;

    push_buf->idx           = 0;
    push_buf->storage[0]    ='\0';
    return util_eOk;
}


/* Obtain byte from stream unless a previous pushed byte exists
 *
 * Returns EOF on failure setting errno
 * Returns byte on success, EOF may be returned as a 'success' return when input stream is at end of file
 * EINVAL: invalid args passed*/
int
utility_push_buf_getc(
        util_push_buf_t *push_buf
        )
{
    size_t nelem = 1;
    errno = EINVAL;
    if(!push_buf || !push_buf->storage || !push_buf->fp)
        return EOF;

    if(push_buf->idx > 0)
        return push_buf->storage[--push_buf->idx];
    
    memset(push_buf->storage,'\0',push_buf->len);
    nelem = fread(push_buf->storage,sizeof(uint8_t),push_buf->len-1,push_buf->fp);
    /* short count is not always an error */
    if(nelem != push_buf->len)
    {
        if(ferror(push_buf->fp))
            return EOF;

        if(feof(push_buf->fp))
            return EOF;
    }

    push_buf->idx = nelem;
    utility_reverse(push_buf->storage,nelem);
    return push_buf->storage[--push_buf->idx];
}

/* Store a byte for later use
 *
 * Returns EOF on failure setting errno
 * Returns stored byte on success
 * EINVAL: invalid args passed*/
int
utility_push_buf_ungetc(
        util_push_buf_t *push_buf,
        int ch
        )
{
    errno = EINVAL;
    if(!push_buf ||  !push_buf->storage || !(push_buf->idx < push_buf->len))
        return EOF;

    if(EOF != ch)
        push_buf->storage[push_buf->idx++] = ch;

    return ch;
}

/* set or retrive user data,
 *  calling with null data is a retrieve
 *  calling with non null data is a write
 *
 *
 * EINVAL: parser ptr was null */
const char *
utility_push_buf_storage(
        util_push_buf_t *ptr,
        char *buf,
        size_t len
        )
{
    /* dodgy arguments so bail */
    errno = EINVAL;
    if(!ptr)
        return NULL;

    /* Read current value */
    if(!buf)
        return ptr->storage;

    /* Overwrite current value, returning updated value */
    ptr->storage = buf;
    ptr->len     = len;
    return buf;
}


#if defined( __cplusplus)
    }
#endif

