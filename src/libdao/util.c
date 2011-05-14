#if defined( __cplusplus)
    extern "C" {
#endif

#include "util.h"
#include <ctype.h>
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
                errno = EINVAL;
                //p2p_set_errno(util_eInvalidArguments);
                p = (!s || ((len+1) < len))? NULL : (char *)malloc( len+1 );

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

                errno = EINVAL;
                //p2p_set_errno(util_eInvalidArguments);
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
                errno = EINVAL;
                //p2p_set_errno(util_eInvalidArguments);
                if(!s || !dest)
                    return util_eError;

                /* count occurences of char */
                for(search = (char *)s;  (search=strchr(search,ch)); ++count)
                    ++search;

                *dest = count;
                return util_eOk;
            }

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
                errno = EINVAL;
                //p2p_set_errno(util_eInvalidArguments);

                /* count occurences of char and  test for overflow as need to increment counter to accomadate sentinal  */
                if( (!pargc || !s) || (util_eError == utility_count_ch(&count,s,ch)) || (count+1 < count))
                    return NULL;

                /* sentinal is not present, and string is empty */
                if(!count && !strcmp("",s))
                    return NULL;

                /* allocate array of string pointers plus sentinal */
                argv = (char **)calloc(count+2,sizeof(char *));
                if(!argv)
                    return NULL;

                tmp = utility_strndup(s,strlen(s)+1);
                if(!tmp)
                {
                    free(argv);
                    errno = ENOMEM;
                    //p2p_set_errno(ENOMEM);
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

        /* comp.lang.c rune for maximum width of PRIu_n */
#define WIDTH(type) (((sizeof(type) * CHAR_BIT + 2) / 3 + 1 + 1))


        /* Converts a substring to its unsigned long integer representation
         * Returns -1 on error setting errno
         * Returns number of unconverted chars on success with numeric value stored in pdest
         * EINVAL: invalid args passed
         * Function may fail and set errno for same reasons as
         * strtol() */
        int
            utility_sntoul(
                    unsigned long *pdest,
                    const char *s,
                    size_t len
                    )
            {
                char buf[WIDTH(*pdest)+1] = {0};
                char *pend;
                unsigned long val;
                int neg=0;
                int zero=0;
                const int radix = 10;
                errno = EINVAL;
                if(!s  || !len || !pdest || len >= WIDTH(*pdest))
                    return util_eError;

                /* skip leading space */
                for(pend = (char*)s; (len && isspace(*pend)); ++pend)
                    --len;

                /* leading positive sign is meaningless but valid */
                errno = ERANGE;
                if(!len)
                    return util_eError;

                /* skip leading zeros and sign, we rely on side effects nasty */
                if( ('+' == *pend) || (neg=('-' == *pend)) || (zero=('0' == *pend)))
                    for( ++pend, --len; (len && '0' == *pend); --len)
                        ++pend;

                /* only zeros found so we discard any sign  */
                if(!len && !zero)
                    return util_eError;

                else if(!len)
                {
                    *pdest = 0;
                    return util_eOk;
                }

                memcpy(buf,pend,len);
                buf[len] = '\0';

                /* need this to tell the difference
                 * between error and conversion of zero, which we handle above */
                errno = 0;
                val = strtoul(buf, &pend,radix);

                /* different from succesful conversion of zero, or no digits converted*/
                if( (errno && !val) || (pend == buf)  ||
                        ((ERANGE == errno) && (ULONG_MAX == val) ))
                    return util_eError;

                /* store result and return number of unconverted chars remaining */
                *pdest = val;
                return (((intptr_t)(pend - buf)) > INT_MAX) ?  INT_MAX :
                    ((int)(pend - buf)) ;
            }

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
        )
{
char buf[WIDTH(*pdest)+1] = {0};
char *pend;
long val;
int neg=0;
int zero=0;
const int radix = 10;
    errno = EINVAL;
    if(!s  || !len || !pdest || len >= WIDTH(*pdest))
        return util_eError;

    /* skip leading space */
    for(pend = (char*)s; (len && isspace(*pend)); ++pend)
        --len;

    /* leading positive sign is meaningless but valid */
    errno = ERANGE;
    if(!len)
        return util_eError;

    /* skip leading zeros and sign, we rely on side effects nasty */
    if( ('+' == *pend) || (neg=('-' == *pend)) || (zero=('0' == *pend)))
        for( ++pend, --len; (len && '0' == *pend); --len)
            ++pend;

    /* only zeros found so we discard any sign  */
    if(!len)
        return (zero) ? (*pdest = 0) : util_eError;

    if(neg)
        buf[0] = '-';

    memcpy((buf+neg),pend,len);
    (buf+neg)[len] = '\0';

    /* need this to tell the difference
     * between error and conversion of zero, which we handle above */
    errno = 0;
    val = strtol(buf, &pend,radix);

    /* different from succesful conversion of zero, or no digits converted*/
    if( (errno && !val) || (pend == buf)  || ((ERANGE == errno) &&
            /*overflow/underflow of value */
             (LONG_MAX == val || LONG_MIN == val)))
        return util_eError;

    /* store result and return number of unconverted chars remaining */
    *pdest = val;
    return (((intptr_t)(pend - buf)) > INT_MAX) ?  INT_MAX :
        ((int)(pend - buf)) ;
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

                errno = EINVAL;
                //p2p_set_errno(util_eInvalidArguments);
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
int
utility_diff_within_delta(
        const double diff,
        const double delta
        )
{
    return ( ((-delta) < diff) && (diff < delta) );
}

int
utility_same_double(
        const double a,
        const double b
        )
{
    return utility_diff_within_delta( (a-b), 0.00005);
}



#if defined( __cplusplus)
    }
#endif

