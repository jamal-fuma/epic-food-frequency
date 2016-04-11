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
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* helpers */
static inline int utility_pstk_full(PTR_STACK *ptr);
static inline int utility_pstk_empty(PTR_STACK *ptr);

/* stack of pointers */
struct utility_pstk
{
	void **stack;	/* pointer to malloced storage  */
	int16_t n;	/* current stack pointer 	*/
	uint16_t max;	/* maximum number of entries 	*/
};

/* Returns index of first occurence of ch in the string 's'
 * Returns -1 on error setting errno
 * Returns index of first occurence of ch on success or -2 if ch is not present
 * util_eInvalidArguments: invalid args passed*/
int
utility_index_of(int ch,const char *s)
{
char *pos;
	p2p_set_errno(util_eInvalidArguments);
	if(!s || ch<0)
		return util_eError;

        pos = strchr(s, ch);
	return (pos) ? (pos - s) : -2;
}

/* Returns index of first occurence of ch in the byte buffer 's'
 * Returns -1 on error setting errno
 * Returns index of first occurence of ch on success or -2 if ch is not present
 * util_eInvalidArguments: invalid args passed*/
int
utility_bindex_of(int ch,void *buf, size_t len)
{
char *pos;
	p2p_set_errno(util_eInvalidArguments);
	if(!buf || ch<0 || !len)
		return util_eError;

        pos = memchr(buf, ch,len);

	return (pos) ?(pos - (char *)buf) : -2;
}


/* Prints varadic arguments using an output_func and calls utility_gets to get the response
 * 'output_func' is called with a string containing the utility_sprintf'd arguments
 * 'output_func' is expected to return <0 on failure and >=0 on success
 * Returns <0 on error setting errno
 * Returns return value from 'output_func' on success,
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as utility_output() or utility_gets*/
int
utility_input(output_func_t output_func,char *dest, int len, const char *fmt,...)
{
va_list ap;

	if(!fmt || !dest || len<=0 || !output_func)
		return util_eError;

	va_start(ap,fmt);
	utility_output(output_func,fmt,ap);
	va_end(ap);

	return utility_gets(dest,len);
}

/* Printf that uses an output_func to do the write
 * 'output_func' is called with a string containing the utility_sprintf'd arguments
 * 'output_func' is expected to return <0 on failure and >=0 on success
 * Returns <0 on error setting errno
 * Returns return value from 'output_func' on success,
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as utility_output()*/
int
utility_printf(output_func_t output_func, const char *fmt,...)
{
int ret;
va_list ap;

	if(!fmt)
		return util_eOk;

	va_start(ap,fmt);
	ret = utility_output(output_func,fmt,ap);
	va_end(ap);

	return ret;
}

/* utility_vsprintf wrapper that allocates enough memory for argument list
 * Returns NULL on error setting errno
 * Returns pointer to allocated copy of 's'  on success, the caller must free this memory
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as utility_vsprintf()*/
char *
utility_sprintf(const char *fmt,...)
{
char *ret;
va_list ap;
	p2p_set_errno(util_eInvalidArguments);
	if(!fmt)
		return NULL;

	va_start(ap,fmt);
	ret = utility_vsprintf(fmt,ap);
	va_end(ap);

	return ret;

}

/* Sprintf that allocates enough memory for argument list ap
 * Relies on vsnprintf actually being posixly correct,
 * on (linux libc4) vsnprintf is defined as a sprintf wrapper which is broken
 * and makes this function unsafe, if you have that libc, complain to your vendor
 *
 * Returns NULL on error setting errno
 * Returns pointer to allocated copy of 's'  on success, the caller must free this memory
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as malloc()*/
char *
utility_vsprintf(const char *fmt, va_list ap)
{
size_t size = 128 ; /* this must be a power of two */
char *buf;
int n;

	/* loosely based on code taken from vprintf man page */
	p2p_set_errno(util_eInvalidArguments);
	if(!fmt || (!(buf = malloc(size))))
		return NULL;

	/* retry until success or we eat
	 * all available core */
	for(;;)
	{
		/* rely on oversized buffers to distinguish between complete and
		 * partial success, ignoring the case of a result that exactly fits the
		 * buffer
		 */
		if( 0>(n = vsnprintf(buf, size, fmt, ap)))
		{
			/* something went wrong */
			int err = errno;
			va_end(ap);
			free(buf);
			p2p_set_errno(err);
			return NULL;
		}
		/* result is larger then 'size' bytes */
		else if((size  - n) > size)
                {
                    /* this leaks with realloc but safe for utility_realloc */
                    if(!(buf = utility_realloc(buf,(size <<= 1))))
                        return NULL;
                }
		/* result fits in buffer of 'size' bytes */
                else
                    return buf;
	}
}

/* Strndup for systems that don't have it,
 * uses host implementation if natively supported
 * Returns NULL on error setting errno
 * Returns pointer to allocated copy of 's'  on success, the caller must free this memory
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as malloc()*/
char *
utility_strndup(
        const char *s,
        size_t len
        )
{
char *p;
	p2p_set_errno(util_eInvalidArguments);
	p = (!s || ((len+1) < len))? NULL : malloc( len+1 );

	if(p)
	{
		strncpy(p,s,len);
		p[len] = '\0';
	}

	return p;
}

/* Replace every occurence of 'old' with 'new' in the string 's'
 * Returns -1 on error setting errno
 * Returns  0 on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_replace_ch(
        char *haystack,
        size_t *dest_count,
        int needle,
        int ch
        )
{
size_t count=0;
char *search;

    p2p_set_errno(util_eInvalidArguments);
    if(!haystack || '\0' == needle || !dest_count)
	return util_eError;

    /* count occurences of char */
    for(search = haystack;  (search=strchr(search,needle)); ++count)
    {
        *search = ch;
        ++search;
    }
    *dest_count = count;
    return util_eOk;
}

/* Resize a chunk of memory obtained by a previous call to malloc()
 * The behaviour is different to stdlib realloc in that 'old' is always freed
 * Null pointers and zero sizes are not supported, use malloc/free directly if that behaviour is
 * ... desired.
 * This means that p = utility_realloc(p,size) is safe, while as we all know
 * 		   p = realloc(p,size)      is a leak waiting to happen
 * On success: Returns a pointer to size bytes of uninitialized memory freeing 'old'
 * On Failure: Returns NULL on failure modifiying errno AND freeing old.
 * util_eInvalidArguments: invalid args passed, 'old' is null or size is '0'
 * Function may fail and set errno for same reason as realloc()*/
void *
utility_realloc(void *old,size_t size)
{
void *p;
int err;

	p2p_set_errno(util_eInvalidArguments);
	if(!old || !size)
		return NULL;

	/* old is freed on success */
	if((p = realloc(old,size)))
		return p;

	/* old is not freed on stdlib realloc failure */
	err = errno;
	/* it is now */
	free(old);
	p2p_set_errno(err);
	return p;
}

/* Open a file with logging and filesystem checks on the filename
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * util_eInvalidArguments: invalid args passed
 * function may fail and set errno for same reasons as fopen()*/
FILE *
utility_fopen(
        const char *fname,
        const char *mode,
        int should_exist
)
{
    FILE *fp;


    if(!mode)
        utility_trace_return_ptr(NULL,"mode [%p] is incorrect",mode);

    if(!fname)
        utility_trace_return_ptr(NULL,"filename [%p] is incorrect",fname);

    if(should_exist && util_eError == utility_file_exists(fname))
        utility_trace_return_ptr(NULL,"file [%s] : not found", fname);

    /* 4) bail if file open fails */
    fp = fopen(fname,mode);
    if(!fp)
        utility_trace_return_ptr(NULL,"file [%s] : opening file failed", fname);

    return fp;
}

/* Reopen a file with logging and filesystem checks on the filename
 *
 * Returns NULL on error setting errno
 * Returns valid file pointer on success
 *
 * util_eInvalidArguments: invalid args passed
 * function may fail and set errno for same reasons as
 * freopen()
 * utility_fopen */
FILE *
utility_freopen(
        const char *fname,
        const char *mode,
        FILE       *old_fp,
        int should_exist
        )
{
    FILE *fp=NULL;
    p2p_set_errno(util_eInvalidArguments);
    if(!fname)
        utility_trace_return_ptr(NULL,
                "Error : %s","filename is incorrect");

    if(old_fp)
        fclose(old_fp);

    if(!(fp = utility_fopen(fname,mode,should_exist)))
        utility_trace_return_ptr(NULL,
                "Error : filename [%s] refused to open",fname);

    return fp;
}


/* Printf a log file in "a+" mode writes 'buf' to fp and closes
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * function may fail and set errno for same reasons as utility_tmp_name()/ or fopen()/unlink()
*/
int
utility_fname_printf(
        const char *logfile,
        const char *fmt,
        ...
)
{
    va_list ap;
    p2p_set_errno(util_eInvalidArguments);
    if(!fmt || !logfile)
        return util_eError;

    va_start(ap,fmt);
    FILE *fp;
    fp = fopen(logfile,"a+");
    if(fp)
    {
        int ret = vfprintf(fp,fmt, ap);
        utility_fflush(fp);
        fclose(fp);
        fp = NULL;
        va_end(ap);
        return ret;
    }
    return util_eError;
}

int
utility_fname_write(
        const char *logfile,
        const char *mode,
        void *buf,
        size_t len
)
{
    FILE *fp;
    int ret;
    p2p_set_errno(util_eInvalidArguments);
    if(!logfile || !mode)
        return util_eError;

    fp = fopen(logfile,mode);
    if(!fp)
        return util_eError;

    ret = (int)fwrite(buf,len,1,fp);
    utility_fflush(fp);
    fclose(fp);
    fp = NULL;
    return (ret == 1)? util_eOk : util_eError;
}




/* Allow handling of variadic arguments without cluttering code
 * 'func' is called with a string containing the utility_sprintf'd arguments
 * 'func' is expected to return <0 on failure and >=0 on success
 * Returns <0 on error setting errno
 * Returns return value from 'func' on success,
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as utility_sprintf()*/
int
utility_output(output_func_t func,const char *fmt,va_list ap)
{
char *p;
int ret;
	p2p_set_errno(util_eInvalidArguments);
	if(!fmt || !func)
		return util_eError;

	/* makes sure string is big enough for all args */
	if(!(p = utility_vsprintf(fmt,ap)))
		return util_eError;

	ret = (*func)(p);

	/* plug the leak */
	free(p);
	p = NULL;

	return ret;
}

/* Obtains upto len-1 bytes from stdin and nul terminate the result
 * Returns <0 on error setting errno
 * Returns number of bytes read on success,
 * A return value of 0 means EOF was encountered
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as fgets()*/
int
utility_gets(
        char *dest,
        size_t len
        )
{
    return utility_fgets(dest,len,stdin);
}

/* Obtains upto len-1 bytes from fp and nul terminate the result
 * Returns <0 on error setting errno
 * Returns number of bytes read on success,
 * A return value of 0 means EOF was encountered
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as fgets()*/
int
utility_fgets(
        char *dest,
        size_t len,
        FILE *fp
        )
{
char *p = NULL ;
	p2p_set_errno(util_eInvalidArguments);
	if(!dest || !len || !fp)
		return util_eError;

	/* obtain newline terminated input lines */
	dest[0] = '\0';

	/* destinguish errors and eof */
	if(!fgets(dest,len-1,fp))
		return util_eError;

	/* replace newline with nul or nul terminate in the case of truncated input */
	*((!(p = strchr(dest,'\n')))? dest+len : p)  = '\0';

	return strlen(dest);
}

/* Call a func for each element in the list
 * returns index of entry that caused func to return non zero
 * returns -1 if the whole list was traversed without func returning non-zero
 * returns -2 setting errno on error
 * util_eInvalidArguments: invalid arguments */
int
utility_for_each_in_list( void *plist,size_t entry_size, size_t nentries,
			  int (*fptr)(void *parg))
{
void *p;
void *pend;
void *pfound = NULL;
int n=0;

	p2p_set_errno(util_eInvalidArguments);
	if(!plist | !fptr || !entry_size || !nentries)
		return -2;

	/* need to do the maths ourselves as voids provide no hint as to stride width */
	pend = (uint8_t *)plist+(nentries * entry_size);

	/* call a func for each element in the list or until the func returns 1 */
	for(p = plist; !n &&  p < pend; p = (((uint8_t *)p) + entry_size) )
		if( (n = (*fptr)(p)) )
			pfound = p;

	/* return index causing func to return 1 or -1 if the whole list was traversed */
	return (n) ? (int)(((uint8_t *)pfound - (uint8_t *)plist) / entry_size)  : -1 ;
}

/* Call a func for each element in the list, passing the 'state' args to 'fptr'
 * returns index of entry that caused func to return non zero
 * returns -1 if the whole list was traversed without func returning non-zero
 * returns -2 setting errno on error
 * util_eInvalidArguments: invalid arguments */
int
utility_for_each_in_list_with_state( void *plist,size_t entry_size, size_t nentries,void *state,
			  int (*fptr)(void *parg,void *pstate))
{
void *p;
void *pend;
void *pfound = NULL;
int n=0;

	p2p_set_errno(util_eInvalidArguments);
	if(!plist | !fptr || !entry_size || !nentries || !state)
		return -2;

	/* need to do the maths ourselves as voids provide no hint as to stride width */
	pend = (uint8_t *)plist+(nentries * entry_size);

	/* call a func for each element in the list or until the func returns 1 */
	for(p = plist; !n &&  p < pend; p = (((uint8_t *)p) + entry_size) )
		if( (n = (*fptr)(p,state)) )
			pfound = p;

	/* return index causing func to return 1 or -1 if the whole list was traversed */
	return (n) ? (int)(((uint8_t *)pfound - (uint8_t *)plist) / entry_size)  : -1 ;
}

/* Call a func for each element in the list, passing the current index to 'fptr'
 * returns index of entry that caused func to return non zero
 * returns -1 if the whole list was traversed without func returning non-zero
 * returns -2 setting errno on error
 * util_eInvalidArguments: invalid arguments */
int
utility_for_each_in_list_with_index( void *plist,size_t entry_size, size_t nentries,
			  int (*fptr)(void *parg,size_t index))
{
void *p;
void *pend;
void *pfound = NULL;
int n=0;
size_t idx = 0;
	p2p_set_errno(util_eInvalidArguments);
	if(!plist | !fptr || !entry_size || !nentries)
		return -2;

	/* need to do the maths ourselves as voids provide no hint as to stride width */
	pend = (uint8_t *)plist+(nentries * entry_size);

	/* call a func for each element in the list or until the func returns 1 */
	for(p = plist; !n &&  p < pend; p = (((uint8_t *)p) + entry_size) )
		if( (n = (*fptr)(p,idx++)) )
			pfound = p;

	/* return index causing func to return 1 or -1 if the whole list was traversed */
	return (n) ? (int)(((uint8_t *)pfound - (uint8_t *)plist) / entry_size)  : -1 ;
}

/* Call a func for each element in the list, passing the current index and 'state' to 'fptr'
 * returns index of entry that caused func to return non zero
 * returns -1 if the whole list was traversed without func returning non-zero
 * returns -2 setting errno on error
 * util_eInvalidArguments: invalid arguments */
int
utility_for_each_in_list_with_index_and_state( void *plist,size_t entry_size, size_t nentries,void *state,
			  int (*fptr)(void *parg,size_t index,void *pstate))
{
void *p;
void *pend;
void *pfound = NULL;
size_t idx = 0;
int n=0;

	p2p_set_errno(util_eInvalidArguments);
	if(!plist | !fptr || !entry_size || !nentries || !state)
		return -2;

	/* need to do the maths ourselves as voids provide no hint as to stride width */
	pend = (uint8_t *)plist+(nentries * entry_size);

	/* call a func for each element in the list or until the func returns 1 */
	for(p = plist; !n &&  p < pend; p = (((uint8_t *)p) + entry_size) )
		if( (n = (*fptr)(p,idx++,state)) )
			pfound = p;

	/* return index causing func to return 1 or -1 if the whole list was traversed */
	return (n) ? (int)(((uint8_t *)pfound - (uint8_t *)plist) / entry_size)  : -1 ;
}

/* Downcase a string in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * util_eInvalidArguments: invalid args passed*/
char *
utility_downcase(char *s)
{
char *p;
	p2p_set_errno(util_eInvalidArguments);
	if(!s)
		return NULL;

	for(p =s; (*p); p++)
		*p = tolower(*p);

	return s;
}

/* Upcase a string in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * util_eInvalidArguments: invalid args passed*/
char *
utility_upcase(char *s)
{
char *p;
	p2p_set_errno(util_eInvalidArguments);
	if(!s)
		return NULL;

	for(p=s; (*p); p++)
		*p = toupper(*p);

	return s;
}

/* Reverse a string in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * util_eInvalidArguments: invalid args passed*/
char *
utility_reverse(char *s,size_t len)
{
char *p,*q, *src;
	p2p_set_errno(util_eInvalidArguments);
	if(!s)
		return NULL;

        /* no-op */
        if(!len)
            return s;

        for(src = p = s, q = src + len - 1; (q > p); ++p, --q)
        {
            *q ^= *p;
            *p ^= *q;
            *q ^= *p;
        }

	return src;
}

/* Reverse a byte string in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * util_eInvalidArguments: invalid args passed*/
uint8_t *
utility_reverse_ui8(uint8_t *s,int len)
{
    /* don't truncate make caller be explicit about intentions */
    return (uint8_t*)utility_reverse((char *)s,len);
}

/* Reverse a uint16_t array in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * util_eInvalidArguments: invalid args passed*/
uint16_t *
utility_reverse_ui16(uint16_t *s,int len)
{
    uint16_t *p,*q, *src;
	p2p_set_errno(util_eInvalidArguments);
	if(!s)
		return NULL;

        p2p_set_errno(ERANGE);
        if(!len || len < 0)
            return NULL;

        for(src = p = s, q = src + len - 1; (q > p); ++p, --q)
        {
            *q ^= *p;
            *p ^= *q;
            *q ^= *p;
        }

	return src;
}

/* Reverse a uint32_t array in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * util_eInvalidArguments: invalid args passed*/
uint32_t *
utility_reverse_ui32(uint32_t *s,int len)
{
    uint32_t *p,*q, *src;
	p2p_set_errno(util_eInvalidArguments);
	if(!s)
		return NULL;

        p2p_set_errno(ERANGE);
        if(!len || len < 0)
            return NULL;

        for(src = p = s, q = src + len - 1; (q > p); ++p, --q)
        {
            *q ^= *p;
            *p ^= *q;
            *q ^= *p;
        }

	return src;
}

/* Reverse a uint64_t array in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * util_eInvalidArguments: invalid args passed*/
uint64_t *
utility_reverse_ui64(uint64_t *s,int len)
{
    uint64_t *p,*q, *src;
	p2p_set_errno(util_eInvalidArguments);
	if(!s)
		return NULL;

        p2p_set_errno(ERANGE);
        if(!len || len < 0)
            return NULL;

        for(src = p = s, q = src + len - 1; (q > p); ++p, --q)
        {
            *q ^= *p;
            *p ^= *q;
            *q ^= *p;
        }

	return src;
}


/* Duplicates a string converting to uppercase
 * Returns NULL on error setting errno
 * Returns pointer to allocated uppercased copy of 's' on success,
 * the caller must free this memory
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), utility_upcase() */
char *
utility_strndup_upcase(const char *s, int len)
{
char *p;

	/* duplicate, letting strndup do the validation */
	p = utility_strndup(s,len);
	 if(!p)
		return NULL;

	 /* uppercase the allocated string */
	 if(!utility_upcase(p))
	 {
		/* clean up on error */
		int err = errno;
		free(p);
		p2p_set_errno(err);
		return NULL;
	 }

	 /* one uppercase copy */
	 return p;
}

/* Duplicates a string converting to lowercase
 * Returns NULL on error setting errno
 * Returns pointer to allocated downcased copy of 's' on success,
 * the caller must free this memory
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), utility_downcase() */
char *
utility_strndup_downcase(const char *s, int len)
{
char *p;

	/* duplicate, letting strndup do the validation */
	p = utility_strndup(s,len);
	 if(!p)
		return NULL;

	 /* uppercase the allocated string */
	 if(!utility_downcase(p))
	 {
		/* clean up on error */
		int err = errno;
		free(p);
		p2p_set_errno(err);
		return NULL;
	 }

	 /* one lowercase copy */
	 return p;
}

/* Duplicates a string reversing byte order
 * Returns NULL on error setting errno
 * Returns pointer to allocated reversed copy of 's' on success,
 * the caller must free this memory
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), utility_reverse) */
char *
utility_strndup_reverse(const char *s, int len)
{
char *p;

	/* duplicate, letting strndup do the validation */
	p = utility_strndup(s,len);
	 if(!p)
		return NULL;

	 /* uppercase the allocated string */
	 if(!utility_reverse(p,len))
	 {
		/* clean up on error */
		int err = errno;
		free(p);
		p2p_set_errno(err);
		return NULL;
	 }

	 /* one lowercase copy */
	 return p;
}

/* Allocate a new stack of pointers of size 'entries'
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * util_eInvalidArguments: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/
PTR_STACK *
utility_pstk_alloc(const int16_t entries)
{
PTR_STACK *ptr;

	/* alloc container */
	p2p_set_errno(util_eInvalidArguments);
	if(!entries || (!(ptr = malloc(sizeof(PTR_STACK)))) )
		return NULL;

	/* alloc contents */
	if(!(ptr->stack = malloc( (1+entries) * sizeof(void *) )))
	{
		int err = errno;
		free(ptr);
		p2p_set_errno(err);
		return NULL;
	}

	/* initalise contents */
	for( ptr->max = ptr->n = 0; ptr->max <=entries; ptr->max++)
		ptr->stack[ptr->max] = NULL;

	return ptr;
}

/* Free a stack of pointers obtained from a previous call to utility_pstk_alloc()
 * if 'free_func' is not NULL, then 'free_func' will be called with each entry *once*
 * It is the callers responsibility to make sure this behaviour doesn't result in double free
 * i.e. if this can happen, then pass NULL as the 'free_func' argument after
 * calling utility_pstk_pop() in a loop
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_pstk_free(
        PTR_STACK *ptr,
        void (*free_func)(void *)
        )
{
    if(utility_pstk_flush(ptr,free_func))
        return util_eError;



    free(ptr->stack);
    free(ptr);

    return util_eOk;
}

/* Flush a stack of pointers obtained from a previous call to utility_pstk_alloc()
 * if 'free_func' is not NULL, then 'free_func' will be called with each entry *once*
 * It is the callers responsibility to make sure this behaviour doesn't result in double free
 * i.e. if this can happen, then pass NULL as the 'free_func' argument after
 * calling utility_pstk_pop() in a loop
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_pstk_flush(
        PTR_STACK *ptr,
        void (*free_func)(void *)
        )
{
    p2p_set_errno(util_eInvalidArguments);
    if(!ptr)
        return util_eError;

    /* allow caller to handle any clean up needed */
    if(free_func)
    {
        while(--ptr->n >= 0)
            (*free_func)(ptr->stack[ptr->n]) ;
    }
    ptr->n = 0;
    return util_eOk;
}


/* Store a pointer
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * ENOMEM: not enough room to store 'pval'
 * util_eInvalidArguments: invalid args passed*/
int
utility_pstk_push(PTR_STACK *ptr, const void *pval)
{
	/* crap args */
	p2p_set_errno(util_eInvalidArguments);
	if(!ptr)
		return util_eError;

	/* prevent stack overflow */
	if( utility_pstk_full(ptr))
		return util_eError;

	/* store, returning new depth for statistics gathering */
	ptr->stack[ptr->n++] = (void *)pval;
	return ptr->n;
}

/* Destructively retrieve the last value pushed
 *
 * Returns -1 on error setting errno
 * Returns 0 on success storing in '*pval'
 * util_eNoBufs: stack is empty
 * util_eInvalidArguments: invalid args passed*/
int
utility_pstk_pop(PTR_STACK *ptr, void **pval)
{
	if(0>utility_pstk_peek(ptr,pval))
		return util_eError;

	ptr->stack[--ptr->n] = NULL;
	return ptr->n;
}

/* Non destructively retrieve the last value pushed
 *
 * Returns -1 on error setting errno
 * Returns 0 on success storing in '*pval'
 * util_eNoBufs: stack is empty
 * util_eInvalidArguments: invalid args passed*/
int
utility_pstk_peek(PTR_STACK *ptr, void **pval)
{
	/* crap args */
	p2p_set_errno(util_eInvalidArguments);
	if(!ptr || !pval || utility_pstk_empty(ptr))
		return util_eError;

	/* store val without removing from stack*/
	*pval = ptr->stack[ptr->n-1];
	return ptr->n;
}


/* helpers */
static inline int
utility_pstk_full(PTR_STACK *ptr)
{
	assert(ptr != NULL);
	p2p_set_errno(ENOMEM);
	return !(ptr->max > ptr->n+1 );
}

static inline int
utility_pstk_empty(PTR_STACK *ptr)
{
	assert(ptr != NULL);
	p2p_set_errno(util_eNoBufs);
	return !( ptr->n );
}
/* Obtain os stat struct
 * may give inaccurate results in the case of NFS files
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/

int
utility_stat(
        struct stat *dst_st,
        const char *fname
        )
{
int err;
    p2p_set_errno(util_eInvalidArguments);
    if(!fname || !dst_st)
        return util_eError;
again:
    if(!stat(fname,dst_st))
        return util_eOk;

    err = p2p_get_errno();
    if(util_eInterupted == err || util_eWouldBlock == err)
        goto again;

    return util_eError;
}

/* Obtain the status of file on disk as reported by fstat
 * may give inaccurate results in the case of NFS files
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_fstat(
        struct stat *dst_st,
        int fd
        )
{
int err;
    p2p_set_errno(util_eInvalidArguments);
    if(!dst_st)
        return util_eError;
again:
    if(!fstat(fd,dst_st))
        return util_eOk;

    err = p2p_get_errno();
    if(util_eInterupted == err)
        goto again;

    return util_eError;
}

/* Flush stdio buffer
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_fflush(
        FILE *fp
        )
{
int err;
    p2p_set_errno(util_eInvalidArguments);
    if(!fp)
        return util_eError;
again:
    if(0 == fflush(fp))
        return util_eOk;

    err = p2p_get_errno();
    if(util_eInterupted == err)
        goto again;

    return util_eError;
}

/* Fileno
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_fileno(
        int *dst_fd,
        FILE *fp
        )
{
int err;
    p2p_set_errno(util_eInvalidArguments);
    if(!fp || !dst_fd)
        return util_eError;

again:
    if((err = fileno(fp)) >= 0)
    {
        *dst_fd = err;
        return util_eOk;
    }

    err = p2p_get_errno();
    if(util_eInterupted == err)
        goto again;

    return util_eError;
}

/* Obtain the status of file on disk as reported by fstat
 * may give inaccurate results in the case of NFS files
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_fp_stat(
        struct stat *dst_st,
        FILE *fp
        )
{
int fd;
    p2p_set_errno(util_eInvalidArguments);
    if(!dst_st || !fp)
        return util_eError;

    if(utility_fileno(&fd,fp))
        return util_eError;

    return utility_fstat(dst_st,fd);
}

/* Obtain the size of a file on disk as reported by stat
 * may give inaccurate results in the case of NFS files
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_filesize(
        size_t *dest_sz,
        const char *fname
        )
{
struct stat st;
    p2p_set_errno(util_eInvalidArguments);
    if(!dest_sz || utility_stat(&st,fname))
        return util_eError;

    *dest_sz = (size_t)st.st_size;
    return util_eOk;
}

/* Obtain the size of a file on disk as reported by stat
 * may give inaccurate results in the case of NFS files
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_fp_filesize(
        size_t *dest_sz,
        FILE *fp
        )
{
struct stat st;
    p2p_set_errno(util_eInvalidArguments);
    if(!dest_sz || utility_fp_stat(&st,fp))
        return util_eError;

    *dest_sz = (size_t)st.st_size;
    return util_eOk;
}

/* Determine if fname resolves to a regular file (i.e implements  [ -f fname ]
 *
 * Returns -1 on failure setting errno
 * Returns  0 on successfully discovering fname is not a file (i.e. doesn't exist or is a directory)
 * Returns  1 on successfully discovering fname is a file (i.e. exists and is a regular file)
 * util_eInvalidArguments: invalid args passed*/
int
utility_file_exists(
        const char *fname
        )
{
    struct stat st;
    p2p_set_errno(util_eInvalidArguments);
    if(!fname)
        return util_eError;


    if(utility_stat(&st,fname))
        return util_eError;

    return (S_ISREG(st.st_mode));
}

/* Determine if dname resolves to a directory (i.e implements  [ -d dname ]
 *
 * Returns -1 on failure setting errno
 * Returns  0 on successfully discovering dname is not a directory (i.e. doesn't exist or a file/fifo/link etc)
 * Returns  1 on successfully discovering dname is a directory (i.e. exists and is a directory)
 * util_eInvalidArguments: invalid args passed*/
int
utility_dir_exists(
        const char *fname
        )
{
    struct stat st;
    p2p_set_errno(util_eInvalidArguments);
    if(!fname)
        return util_eError;


    if(utility_stat(&st,fname))
        return util_eError;

    return (S_ISDIR(st.st_mode));
}

/* Determine last modified time of path
 *
 * Returns -1 on failure setting errno
 * Returns  0 on successfully discovering dname is not a directory (i.e. doesn't exist or a file/fifo/link etc)
 * Returns  1 on successfully discovering dname is a directory (i.e. exists and is a directory)
 * util_eInvalidArguments: invalid args passed
 * function may fail and set errno for same reasons as
 * utility_stat */
int
utility_last_modified_at(
        const char *fname,
        time_t     *dest
        )
{
    struct stat st;
    p2p_set_errno(util_eInvalidArguments);
    if(!fname || !dest)
        return util_eError;

    if(utility_stat(&st,fname))
        return util_eError;

    *dest = st.st_mtime;
    return util_eOk;
}


/* Allocate an array of bytes copied from 'fname'
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced copy of file on success
 * util_eInvalidArguments: invalid args passed
 * ENOENT: empty file
 * function may fail and set errno for same reasons as
 * utility_fslurp() */
void *
utility_slurp(
        const char *fname
        )
{
size_t sz;
    return utility_slurp_with_sz(&sz,fname);
}

/* Allocate an array of bytes copied from 'fname'
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced copy of file on success
 * util_eInvalidArguments: invalid args passed
 * ENOENT: empty file
 * function may fail and set errno for same reasons as
 * utility_fslurp() */
void *
utility_slurp_with_sz(
        size_t *dst_sz,
        const char *fname
        )
{
FILE *fp;
void *p;
int err;
    p2p_set_errno(util_eInvalidArguments);
    if(!dst_sz || !fname)
        return NULL;

    fp = fopen(fname,"rb");
    if(!fp)
        return NULL;

    /* newline terminated file of whitespace delimited command line arguments */
    p = utility_fslurp_with_sz(fp,dst_sz);
    err = errno;
    fclose(fp);
    fp = NULL;

    p2p_set_errno(err);
    return p;
}

/* Allocate an array of bytes copied from 'fp'
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced copy of file on success
 * util_eInvalidArguments: invalid args passed
 * ENOENT: empty file
 * function may fail and set errno for same reasons as
 * malloc()
 * ftell/fseek/utility_fp_filesize/fread*/
void *
utility_fslurp(FILE *fp)
{
size_t sz;
        return utility_fslurp_with_sz(fp,&sz);
}

/* Allocate an array of bytes copied from 'fp'
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced copy of file on success
 * util_eInvalidArguments: invalid args passed
 * ENOENT: empty file
 * function may fail and set errno for same reasons as
 * malloc()
 * ftell/fseek/utility_fp_filesize/fread*/
void *
utility_fslurp_with_sz(
        FILE *fp,
        size_t *psz
        )
{
    size_t sz;
    void *p;
    int err ;
    p2p_set_errno(util_eInvalidArguments);
    if(utility_fp_filesize(&sz,fp) || !sz)
        return NULL;

    p2p_set_errno(ENOENT);
    if( !(p = malloc(sz+1)))
        return NULL;

    if( 1 != fread(p,sz,1,fp))
    {
        err = errno;
        free(p);
        p2p_set_errno(err);
        return NULL;
    }

    /* success */
    *psz = sz;
    *(((char *)p)+sz) = '\0';
    return p;
}

/* Allocate an unique temp name using prefix, pid/ppid and system clock for entropy
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced tmp_name on success
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_tmp_name_prefixed(const char *prefix)
{
/* use timer to generate a unique file name */
time_t ticks = 0;
    p2p_set_errno(util_eInvalidArguments);
    if(prefix)
       return utility_sprintf("%s_%07ld%05d",prefix,time(&ticks),getpid());
    return NULL;
}
/* Allocate an unique logfile name using prefix, pid/ppid and system clock for entropy
 * caller must free memory
 * returns null on failure setting errno
 * returns pointer to malloced tmp_name on success
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_logfile_name(
        const char *prefix
        )
{
char *p;
/* use timer to generate a unique file name */
time_t ticks = 0;
size_t cnt=0;

    p2p_set_errno(util_eInvalidArguments);
    if(prefix)
    {
        p = utility_sprintf("%s_%05d_%09u.log",prefix,time(&ticks),getpid());
        if(!utility_replace_ch(p,&cnt,DETECT_SLASH,'_') && cnt)
            return p;
    }
    return NULL;
}


/* Allocate an unique temp name using utility_tmp_name_prefixed("tmp-file-name")
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced tmp_name on success
 * function may fail and set errno for same reasons as utility_tmp_name_prefixed()*/
char *
utility_tmp_name(void)
{
    return utility_tmp_name_prefixed("tmp-file-name");
}

/* Open a file in "w+" mode using the name from utility_tmp_name(name is freed)
 * Returns NULL on failure setting errno
 * Returns fp opened in "w+" on success
 * function may fail and set errno for same reasons as utility_tmp_name()/ or fopen()/unlink()*/
FILE *
utility_tmp_file(void)
{
FILE *fp;
int err;
char *p;
	p2p_set_errno(util_eInvalidArguments);

	if(!(p = utility_tmp_name()))
		return NULL;

	fp = fopen(p,"w+");

	err = errno;
	if(unlink(p))
	{
		err = errno;
		if(fp)
			fclose(fp);
		fp = NULL;
	}
	free(p);
	p2p_set_errno(err);
	return fp;
}

gid_t
get_directory_group(void)
{
    return (gid_t)-1;
}

mode_t
get_directory_permissions(
        void
        )
{
    /* windows is not unix it is vms */
#if !defined(_WIN32)
    /* mode 755 */
    return (mode_t)(
             S_IRWXU |              /* owner rd-wr-x */
             S_IRGRP | S_IXGRP |    /* group rd-x    */
             S_IROTH | S_IXOTH );   /* other rd-x    */
#endif
    return 0;
}

#if(0)
char *
utility_dir_name(
        const char *path
        )
{
#if defined(_WIN32)
    char path_buffer[_MAX_PATH] = {0};
    char drive[_MAX_DRIVE] = {0};
    char dir[_MAX_DIR] = {0};
    char fname[_MAX_FNAME] = {0};
    char ext[_MAX_EXT] = {0};

    p2p_set_errno(util_eInvalidArguments);
    if(!path)
        return NULL;

    /* windows still has buffer overflow designed in */
    if(util_eOk ==
            _splitpath_s(
                path,
                drive,  UTILITY_NENTRIES(drive),
                dir,    UTILITY_NENTRIES(dir),
                fname,  UTILITY_NENTRIES(fname),
                ext,    UTILITY_NENTRIES(ext)
                ) )
    {
        return utility_strndup(drive,strlen(drive));
    }
    return NULL;
#endif

#if !defined(_WIN32) && defined HAVE_BASENAME
    char *dir=NULL;
    char *old_path = NULL;

    p2p_set_errno(util_eInvalidArguments);
    if(!path )
        return NULL;

    old_path = utility_strndup(path,strlen(path));
    if(!old_path)
        return NULL;

    /* stash a copy in case this version overwrites */
    path = dirname(old_path);
    if(path)
    {
        printf("%s %s\n", "dirname returned :", path);
        dir = utility_sprintf("%s/",path);
        if(dir)
            printf("%s %s\n", "sprintf returned :",dir);
        else
            printf("%s %p\n", "sprintf returned :",dir);
    }
    free(old_path);
    old_path = NULL;
    return dir;
#elif !defined(_WIN32) && !defined HAVE_BASENAME
    #error "basename is not implemented for this platform"
#endif
}

#endif

#if(0)
/* resolve 'base' to a directory and appends 'filename'
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_filename(
        const char *base,
        const char *filename
        )
{
char *dir= NULL;

    p2p_set_errno(util_eInvalidArguments);
    if(!base || !filename)
        return NULL;

    /* safe with utility_realloc() but leaks with stdlib realloc() */
    dir = utility_realloc(
            utility_dir_name(base),
            /* allow for very strange implementations of dirname */
            (strlen(base) + strlen(filename) + 1 + 1 + 1 +1)
    );

    if(!dir)
        return NULL;

    return strcat(dir,filename);
}

#endif
int
utility_chown(
        const char * dir,
        uid_t   uid,
        gid_t gid
        )
{

/* this is complicated on windows, see http://wwwthep.physik.uni-mainz.de/~frink/nt.html */
#if defined(_WIN32)
    p2p_set_errno(util_eOpNotSupp);
    return util_eError;
#endif

#if !defined(_WIN32)
    int err=0;

    p2p_set_errno(util_eInvalidArguments);
    if(!dir)
        return util_eError;

again: /* restart interupted syscalls */
    if(!chown(dir,uid,gid))
        return util_eOk;

    err = p2p_get_errno();
    if(util_eInterupted == err)
        goto again;

    return util_eError;
#endif

}

int
utility_chmod(
        const char * dir,
        mode_t  mode
        )
{
int err=0;
#if !defined(_WIN32)
again: /* restart interupted syscalls */
    if(!chmod(dir,mode))
        return util_eOk;

    err = p2p_get_errno();
    if(util_eInterupted == err)
        goto again;

    return util_eError;
#endif

#if defined(_WIN32)
mode_t flag;
again: /* restart interupted syscalls */
    flag = GetFileAttributes(dir);
    if(INVALID_FILE_ATTRIBUTES == flag)
        return util_eError;

    if(SetFileAttributes(dir,(flag |= mode)))
        return util_eOk;

    err = GetLastError();
    p2p_set_errno(err);
    if(util_eInterupted == err)
        goto again;

    return util_eError;
#endif /* defined(_WIN32)  */
}


int
utility_mkdir(
        const char * dir,
        mode_t mode
        )
{
    int err=0;

    p2p_set_errno(ENOENT);
    if(1 == utility_dir_exists(dir))
        return util_eError;

    if(0 == mode)
        mode = get_directory_permissions();

again: /* restart interupted syscalls */
#if !defined(_WIN32)
    if(1 == utility_dir_exists(dir))
        return util_eOk;

    if(!mkdir(dir,mode))
        return util_eOk;

    err = p2p_get_errno();
    if(util_eInterupted == err)
        goto again;

    return util_eError;
#endif

#if defined(_WIN32)
    if(CreateDirectory(dir,NULL))
        return utility_chmod(dir,mode);

    err = GetLastError();
    p2p_set_errno(err);
    if(util_eInterupted == err)
        goto again;

    return util_eError;
#endif

}

int
utility_rmdir(
        const char * dir
        )
{
    int err=0;

    /* tried to remove a non existent directory */
    p2p_set_errno(ENOENT);
    if(0 == utility_dir_exists(dir))
        return util_eError;

again: /* restart interupted syscalls */
#if !defined(_WIN32)
    if(0 == utility_dir_exists(dir))
        return util_eOk;

    if(!rmdir(dir))
        return util_eOk;

    err = p2p_get_errno();
    if(util_eInterupted == err)
        goto again;

    return util_eError;
#endif

#if defined(_WIN32)
    if(0 == utility_dir_exists(dir))
        return util_eOk;

    if(RemoveDirectory(dir))
        return util_eOk;

    err = GetLastError();
    p2p_set_errno(err);
    if(util_eInterupted == err)
        goto again;

    return util_eError;
#endif
}

int
utility_mkdir_with_perms(
        const char * dir,
        mode_t mode,
        uid_t  uid,
        gid_t  gid
        )
{
int err;
    if(utility_mkdir(dir,mode))
        return util_eError;

    /* check owner and group are correct */
    if( (utility_chown(dir,uid,gid) && (util_eOpNotSupp != (err = p2p_get_errno()))))
    {
        utility_rmdir(dir);
        return util_eError;
    }

    return util_eOk;
}

/* opens 'fname' in 'mode' and mmap into current address space
 * Caller must call mmunmap with the same 'size' argument as used in this call
 * Returns NULL on failure setting errno
 * Returns pointer to mmap'd file on success
 * util_eInvalidArguments: invalid args passed*/
void *
utility_map_file(
        const char *name,  /* file to map (file is created if it doen't exist*/
        int mode,           /* one of O_RDONLY, O_WRONLY or O_RDWR */
        size_t size         /* size of the mapping */
)
{
#if ! defined (_WIN32)
void *addr;
int fd;
int flags = (O_EXCL |O_TRUNC| O_CREAT|mode);
#endif

#if defined (_WIN32)
    int err;
    DWORD sz = (DWORD)size; /* can only map 2^31 on windows */
    DWORD hdr_sz;
    HANDLE fd;
    void *user,*addr;
    uint32_t flags = (PAGE_READWRITE | SEC_COMMIT | SEC_NOCACHE | (unsigned int )mode);
#endif

    p2p_set_errno(util_eInvalidArguments);
    if(!name || !size)
        return NULL;

#if defined (_WIN32)
    hdr_sz =  ALIGN_8_SZ(sizeof(hdr_sz));
    hdr_sz += ALIGN_8_SZ(sizeof(uintptr_t));
    hdr_sz += ALIGN_8_SZ(sizeof(size));
    hdr_sz += ALIGN_8_SZ(sizeof(sz));
    hdr_sz =  ALIGN_8_SZ(hdr_sz);

    size = (ALIGN_8_SZ(size+hdr_sz));
    sz = ((DWORD)size);

    addr = user = NULL;
    fd = CreateFileMappingA(INVALID_HANDLE_VALUE, NULL, flags, 0,(DWORD)sz, name);
    if(fd != NULL)
    {
        /* share access so ipc works */
        user = addr = MapViewOfFile(fd,FILE_MAP_ALL_ACCESS, 0,0,(DWORD)sz);
        if(addr)
        {
            /* write header size */
            *(DWORD *)user = hdr_sz;
            user = ALIGN_8_ADDR((((char *)user) + ALIGN_8_SZ(sizeof(hdr_sz))));
            sz -= ALIGN_8_SZ(sizeof(size));

             /* write decriptor so we don't have to carry it about */
            *(uintptr_t *)user = (uintptr_t)fd;
            user = ALIGN_8_ADDR((((char *)user) + ALIGN_8_SZ(sizeof(uintptr_t))));
            sz -= ALIGN_8_SZ(sizeof(uintptr_t));

            /* write mapping size */
            *(size_t *)user = size;
            user = ALIGN_8_ADDR((((char *)user) + ALIGN_8_SZ(sizeof(size))));
            sz -= ALIGN_8_SZ(sizeof(size));

            /* write remaining size */
            *(size_t *)user = sz;
            user = ALIGN_8_ADDR((((char *)user) + ALIGN_8_SZ(sizeof(sz))));
            sz -= ALIGN_8_SZ(sizeof(sz));

            memset(user,0,sz);
            return user;
        }
        err = GetLastError();

        CloseHandle(fd);
        fd = INVALID_HANDLE_VALUE;
        p2p_set_errno(err);
        return NULL;
    }
    err = GetLastError();
    p2p_set_errno(err);
    return NULL;

#elif ! defined (_WIN32)
again:
    /* try to open with excluive rights so we know if it's safe to resize */
    fd = open(name, flags,0600);
    if(-1 == fd && (EEXIST == errno && (O_TRUNC == (O_TRUNC & flags))) )
    {
        /* well the file exists already so we can't do that*/
        /* we'll open the file in shared fashion*/
        flags &= ~(O_EXCL |O_TRUNC| O_CREAT);
        goto again;
    }

    if(-1 == fd)
        /* strange, even dropping to shared access causes failure, oh well can't win them all */
        return NULL;

    /* try to resize data file  */
    if(O_TRUNC == (O_TRUNC & flags) && (-1 == ftruncate(fd, size)))
    {
        /* we can only remove if we created otherwise data will be lost */
        unlink(name);
        close(fd);
        return NULL;
    }

    /* map into this process */
    addr = mmap(NULL, size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);
    close(fd);

    /* we can only remove if we created otherwise data will be lost */
    if((addr == MAP_FAILED) && (O_TRUNC == (O_TRUNC & flags)))
        unlink(name);

    return (addr == MAP_FAILED) ? NULL : addr;
#endif /* !defined (_WIN32) */
}


/* Determine if fp refers to a terminal
 *
 * Returns -1 on failure setting errno
 * Returns  0 on succcess modifiying dest to non-zero if isatty
 *
 * util_eInvalidArguments : context data is null
 * function may fail and set errno for same reasons as
 * fileno
 * istty()*/
int
utility_isatty(
        FILE *fp,
        int *dest
        )
{
    int fd;
    p2p_set_errno(util_eInvalidArguments);
    if(!fp || !dest)
        return util_eError;

    if(utility_fileno(&fd,fp))
        return util_eError;

    *dest = (1 ==  isatty(fd));
    return util_eOk;
}

/* join directory and 'filename' with platform suitable delimiter
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_dir_filename(
        const char *dirname,
        const char *filename
        )
{
#if defined(_WIN32)
        static const int delimiter = '\\';
#else
        static const int delimiter = '/';
#endif

    p2p_set_errno(util_eInvalidArguments);
    if(filename && dirname)
        return utility_sprintf("%s%c%s",dirname, delimiter, filename);

    return NULL;
}
#if(0)
/* resolve user home directory and appends 'filename'
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_home_filename(
        const char *filename
        )
{
util_uid_t uid;
    p2p_set_errno(util_eInvalidArguments);
    if(!filename)
        return NULL;

    return utility_get_uid_and_home_dir(&uid,filename);
}
#endif

#if(0)
/* resolve user home directory and appends 'filename'
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_home_dirname(
        const char *dirname
        )
{
util_uid_t uid;
    p2p_set_errno(util_eInvalidArguments);
    if(!dirname)
        return NULL;

    return utility_get_uid_and_home_dir(&uid,dirname);
}
#endif

#if(0)
/* resolve user home directory and appends 'filename'
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_home_dir_filename(
        const char *dirname,
        const char *filename
        )
{
util_uid_t uid = (util_uid_t)0;
char *home = NULL;
char *path = NULL;

    p2p_set_errno(util_eInvalidArguments);
    if(!filename || !dirname)
        return NULL;

    home = utility_get_uid_and_home_dir(&uid,dirname);
    if(!home)
        return NULL;

    path = utility_mk_dir_filename(home,filename);
    free(home);
    home = NULL;
    return path;
}
#endif

#if(0)
/* resolve user home directory and user id (uid)
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_get_uid_and_home_dir(
        util_uid_t *dest_uid,
        const char *suffix
        )
{
#if (!defined(_WIN32) && (defined(HAVE_GETPWUID) && defined(HAVE_GETUID)))
    struct passwd *user = NULL;
#endif /* (!defined(_WIN32) && (defined(HAVE_GETPWUID) && defined(HAVE_GETUID))) */

    char *home = NULL;
    size_t len = 0;
    util_uid_t uid = (util_uid_t)0;
    p2p_set_errno(util_eInvalidArguments);
    if(!dest_uid || !suffix)
        return NULL;

    len = strlen(suffix);

    /* grab id and lookup user in /etc/passwd */
#if (!defined(_WIN32) && (defined(HAVE_GETPWUID) && defined(HAVE_GETUID)))
    /* lookup ~/user  */
    if(NULL == (user = getpwuid((uid = getuid()))))
        return NULL;

    home = (len > 0) ? utility_mk_dir_filename(user->pw_dir,suffix) : utility_sprintf("%s%s",user->pw_dir,suffix);

#endif /* (!defined(_WIN32) && (defined(HAVE_GETPWUID) && defined(HAVE_GETUID))) */

#if defined(_WIN32)
    home = getenv("USERPROFILE"); /* XP variant */

    home = (!home) ?   /* 98  ? */
        getenv("HOME") : home;

    /* Windows Me maybe ? */
    if(!home && (home = utility_mk_dir_filename(getenv("HOMEDRIVE"),getenv("HOMEPATH"))))
    {
        if(len > 0)
        {
            home = utility_realloc(home,len+1+1+1+1+1);
            if(!home)
                return home;

            strcat(home,"\\");
            strcat(home,suffix);
        }
    }
    else if(!home)
    {
        /* ancient windows,  fallback windows idea lifted from sqlite */
        home = utility_mk_dir_filename("C:",suffix)
    }
#endif /* defined WIN32 */

    if(!home)
        return NULL;

    *dest_uid = uid;
    return home;
}
#endif /* 0*/

#if defined( __cplusplus)
}
#endif
