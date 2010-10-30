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
#include "libUtility.h"

#if defined( __cplusplus)
extern "C" {
#endif

#if !(defined HAVE_STDIO_H)
    #error "stdio.h missing" __FILE__ __LINE__ __DATE__
#endif /* !HAVE_STDIO_H */

#if !(defined HAVE_ERRNO_H)
#error "errno.h missing" __FILE__ __LINE__ __DATE__
#endif /* !HAVE_ERRNO_H */

void utility_token_trace(util_token_t *ptr,const char *msg);
#define utility_token_trace(token,msg) \
    do { \
        int old_p2p_set_errno(errno); \
        utility_fname_printf("Trace.log", ( \
            "%s:%04d " \
             msg    \
             " [%p]:\n" \
            "token->octets    [%s]\n" \
            "token->noctets    [%d]\n" \
            "token->navailable    [%d]\n" \
            "token->type   [%d]\n\n" \
             ), \
            __func__,__LINE__, \
            (token),   \
            (token)->octets, \
            (token)->noctets, \
            (token)->navailable, \
            (token)->type \
        ); \
        p2p_set_errno(old_errno); \
    }while(0)

#if !defined (VERBOSE_TOKEN_DEBUG)
    #undef  utility_token_trace
    #undef  utility_trace
    #undef  utility_wrap_return_int
    #undef  utility_wrap_return_ptr
    #define utility_trace(a,b)
    #define utility_token_trace(a,b)
    #define utility_wrap_return_int(ret) return (ret)
    #define utility_wrap_return_ptr(ret) return (ret)
#endif

/* Determine if a write of '1' byte would overflow token buffer
 * Returns -1 on error setting errno
 * Returns  0 if a write of '1' byte wouldn't overflow.
 * Returns  1  setting errno to util_eNoBufs,
 *             if a write of '1' byte would overflow.
 * util_eInvalidArguments: invalid args passed*/
static inline int
would_overflow_ch(
        util_token_t *token
);

/* Determine if a write of 'len' bytes would overflow token buffer
 * Returns -1 on error setting errno
 * Returns  0 if a write of 'len' bytes wouldn't overflow.
 * Returns  1  setting errno to util_eNoBufs,
 *             if a write of 'len' bytes would overflow.
 * util_eInvalidArguments: invalid args passed*/
static inline int
would_overflow_str(
        util_token_t *token,
        ssize_t len
);

struct util_token_tag
{
    /* basic idea is to dole out octets from here and realloc when needed */
    uint32_t noctets;       /* number of octets currently  alloced and used*/
    uint32_t navailable;    /* number of octets currently, alloced and unused */
    uint8_t *octets;        /* octet table pointer */
    uint32_t flag;

    /* old code uses these */
    char *buf;
    int  len;
    int  max;
    int  type;
};

util_token_t *
utility_token_alloc(
        uint32_t len
        )
{
util_token_t *token;

    p2p_set_errno(util_eInvalidArguments);
    if(!len || !(token = malloc(sizeof(util_token_t))))
        return NULL;

    token->navailable = len;
    token->noctets = 0;
    token->octets = malloc(len+1);
    if(!token->octets)
    {
        free(token);
        token = NULL;
        return NULL;
    }

    return token;
}

int
utility_token_free(
        util_token_t *token
        )
{
    p2p_set_errno(util_eInvalidArguments);
    if(!token)
        return util_eError;

    utility_token_trace(token,"Freeing token");

    free(token->octets);
    free(token);
    return util_eOk;
}



int
utility_token_set_type(util_token_t *token,int type)
{
	p2p_set_errno(util_eInvalidArguments);
	if(!token)
		return util_eError;

	token->type = type;
	return util_eOk;
}

int
utility_token_get_type(util_token_t *token,int *ptype)
{
	p2p_set_errno(util_eInvalidArguments);
	if(!token || !ptype)
		return util_eError;

	*ptype = token->type;
	return util_eOk;
}

int
utility_token_is_empty(util_token_t *token)
{
	p2p_set_errno(util_eInvalidArguments);
	if(!token)
		return util_eError;

	return (0 == token->noctets);
}


int
utility_token_append(util_token_t *token,int ch)
{
    p2p_set_errno(util_eInvalidArguments);
    if(!token)
        return util_eError;

    if(would_overflow_ch(token))
        return util_eError;

    token->octets[token->noctets++] = ch;
    return util_eOk;
}

/* Destructively obtain last appended byte
 *
 * Returns EOF on failure setting errno
 * Returns byte on success,
 *
 * EOF may be returned as a 'success' return when underlying buffer is empty errno is not modified in this case.
 * the two cases, (valid call of empty buffer and invalid call) can be distinguished by setting errno to zero prior to call and test after return
 *
 * util_eInvalidArguments: invalid args passed*/
int
utility_token_getc(
        util_token_t *token
        )
{
    if(token && (token->noctets))
       return token->octets[--token->noctets] ;

    if(!token)
        p2p_set_errno(util_eInvalidArguments);
    return EOF;
}

/* Append a string to the token
 * Returns -1 on error setting errno
 * Returns  0 on success
 * util_eInvalidArguments: invalid args passed
 * util_eNoBufs: write would cause overflow */
int
utility_token_append_str(
        util_token_t *token,
        const char *str
)
{
size_t len;
        p2p_set_errno(util_eInvalidArguments);
	if(!token || !str)
		return util_eError;

	if(would_overflow_str(token, (len=strlen(str))))
		return util_eError;

        memcpy( (token->octets + token->noctets), str, len);
	token->noctets += len;
	return util_eOk;
}

char *
utility_token_flush(
        util_token_t *token
        )
{
    p2p_set_errno(util_eInvalidArguments);
    if(!token)
        return NULL;

    token->octets[token->noctets]='\0';
    token->noctets = 0;
    return (char *)token->octets;
}

/* Destructively obtain buffer contents converting to double
 *
 * Returns -1 on failure setting errno
 * Returns number of unconverted trailing char on success,
 *
 * util_eInvalidArguments: invalid args passed*/
int
utility_token_double(
        util_token_t *token,
        double *dest
        )
{
char *p;
    p = utility_token_flush(token);
    return (!p || !dest) ? util_eError : utility_strtod(dest,p);
}

/* Destructively obtain buffer contents converting to long
 *
 * Returns -1 on failure setting errno
 * Returns number of unconverted trailing char on success,
 *
 * util_eInvalidArguments: invalid args passed*/
int
utility_token_long(
        util_token_t *token,
        long *dest
        )
{
char *p;
uint32_t len;
    if(!token )
        return util_eError;

    len = token->noctets;
    p = utility_token_flush(token);
    return (!dest || !p) ? util_eError : utility_sntol(dest,p,len);
}

int
utility_token_clear(
        util_token_t *token
        )
{
    p2p_set_errno(util_eInvalidArguments);

    if(!token )
        return util_eError;

    token->octets[0] = '\0';
    token->noctets    = 0;
    return util_eOk;
}

/* Determine if a write of '1' byte would overflow token buffer
 * Returns -1 on error setting errno
 * Returns  0 if a write of '1' byte wouldn't overflow.
 * Returns  1  setting errno to util_eNoBufs,
 *             if a write of '1' byte would overflow.
 * util_eInvalidArguments: invalid args passed*/
static inline int
would_overflow_ch(
        util_token_t *token
)
{
    return would_overflow_str(token,1);
}

/* Determine if a write of 'len' bytes would overflow token buffer
 * Returns -1 on error setting errno
 * Returns  0 if a write of 'len' bytes wouldn't overflow.
 * Returns  1  setting errno to util_eNoBufs,
 *             if a write of 'len' bytes would overflow.
 * util_eInvalidArguments: invalid args passed*/
static inline int
would_overflow_str(
        util_token_t *token,
        ssize_t len
)
{
    size_t avail = 0;

    p2p_set_errno(util_eInvalidArguments);
    if(!token)
        return util_eError;
    if(token->navailable < token->noctets+(len))
    {
        void * buf;
        avail = utility_vector_polynomial_prime(token->noctets+(len));
        buf = realloc(token->octets,avail);
        if(!buf)
        {
            p2p_set_errno(util_eNoBufs);
            return 1;
        }
        token->octets = buf;
        token->navailable += avail-token->navailable;

    }
    return 0;
}

#if defined( __cplusplus)
    }
#endif


