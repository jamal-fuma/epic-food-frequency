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

/* comp.lang.c rune for maximum width of PRIu_n */
#define WIDTH(type) (((sizeof(type) * CHAR_BIT + 2) / 3 + 1 + 1))

static int
utility_strtod_raw(
        double *dst,
        const char *s
        );

/* Converts a substring to its  unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would underflow or would be > max_value
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
static inline int utility_sntoui_n(unsigned int *pdest,unsigned int max_val,const char *s,int len);
/* Converts a substring to its  unsigned long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would underflow or would be > max_value
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
static inline int utility_sntoul_n(uint64_t *pdest,uint64_t max_val,const char *s,int len);


/* Converts a string to its integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int
utility_stoi(int *pdest,const char *s)
{
	errno = EINVAL;
	return (!pdest || !s) ? -1 : utility_sntoi(pdest,s,strlen(s));
}


/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int
utility_stoui(unsigned int *pdest,const char *s)
{
	errno = EINVAL;
	return (!pdest || !s) ? -1 : utility_sntoui(pdest,s,strlen(s));
}
/* Converts a string to its long representation in decimal
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int
utility_stol(
        long *pdest,
        const char *s
        )
{
    errno = EINVAL;
    return (!pdest || !s) ? -1 : utility_sntol(pdest,s,strlen(s));
}
/* Converts a string to its unsigned long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int
utility_stoul(unsigned long *pdest,const char *s)
{
	errno = EINVAL;
	return (!pdest || !s) ? -1 : utility_sntoul(pdest,s,strlen(s));
}


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

/* Converts a substring to its unsigned long long integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reasons as
 * utility_strndup(), strtoull() */
int
utility_sntoull(
        uint64_t *pdest,
        const char *s,
        size_t len
        )
{
char buf[WIDTH(*pdest)+1] = {0};
char *pend;
unsigned long long val;
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
    val = strtoull(buf,&pend,radix);
    /* different from succesful conversion of zero, or no digits converted*/
    if( (errno && !val) || (pend == buf)  ||
            ((ERANGE == errno) && (ULLONG_MAX == val) ))
        return util_eError;

    /* store result and return number of unconverted chars remaining */
    *pdest = val;
    return (((intptr_t)(pend - buf)) > INT_MAX) ?  INT_MAX :
        ((int)(pend - buf)) ;
}

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
static inline int
utility_sntoui_n(unsigned int *pdest,unsigned int max_val,const char *s,int len)
{
int ret;
unsigned long val;
	errno = EINVAL;
	if(!pdest || !s || !len || (0>(ret =  utility_sntoul(&val,s,len))))
		return util_eError;

	/* don't truncate make caller be explicit about intentions */
	errno = ERANGE;
	if(val > max_val)
		return util_eError;

	*pdest = (unsigned int)val;
	return ret;
}
/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
static inline int
utility_sntoul_n(uint64_t *pdest,uint64_t max_val,const char *s,int len)
{
int ret;
uint64_t val;
	errno = EINVAL;
	if(!pdest || !s || !len || (0>(ret =  utility_sntoull(&val,s,len))))
		return util_eError;

	/* don't truncate make caller be explicit about intentions */
	errno = ERANGE;
	if(val > max_val)
		return util_eError;

	*pdest = (uint64_t)val;
	return ret;
}

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int
utility_sntoui(
        unsigned int *pdest,
        const char *s,
        size_t len)
{
	return utility_sntoui_n(pdest,UINT_MAX,s,len);
}

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int
utility_sntoui8(uint8_t *pdest,const char *s,int len)
{
	return utility_sntoui_n((unsigned int *)pdest,UINT8_MAX,s,len);
}

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int
utility_sntoui16(uint16_t *pdest,const char *s,int len)
{
	return utility_sntoui_n((unsigned int *)pdest,UINT16_MAX,s,len);
}

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int
utility_sntoui32(uint32_t *pdest,const char *s,int len)
{
	return utility_sntoui_n((unsigned int *)pdest,UINT32_MAX,s,len);
}

/* Converts a substring to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoul() */
int
utility_sntoui64(uint64_t *pdest,const char *s,int len)
{
	return utility_sntoul_n(pdest,UINT64_MAX,s,len);
}
/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoui8() */
int
utility_stoui8(uint8_t *pdest,const char *s)
{
	errno = EINVAL;
	return (!s) ? -1 : utility_sntoui8(pdest,s,strlen(s));
}

/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoui16() */
int
utility_stoui16(uint16_t *pdest,const char *s)
{
	errno = EINVAL;
	return (!s) ? -1 : utility_sntoui16(pdest,s,strlen(s));
}

/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoui32() */
int
utility_stoui32(uint32_t *pdest,const char *s)
{
	errno = EINVAL;
	return (!s) ? -1 : utility_sntoui32(pdest,s,strlen(s));
}

/* Converts a string to its unsigned integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntoui64() */
int
utility_stoui64(uint64_t *pdest,const char *s)
{
	errno = EINVAL;
	return (!s) ? -1 : utility_sntoui64(pdest,s,strlen(s));
}

/* Converts a substring to its integer representation
 * Returns -1 on error setting errno
 * Returns number of unconverted chars on success with numeric value stored in pdest
 * EINVAL: invalid args passed
 * ERANGE: conversion would over/underflow
 * Function may fail and set errno for same reasons as
 * utility_sntol() */
int
utility_sntoi(int *pdest,const char *s,int len)
{
int ret;
long val;

	errno = EINVAL;
	if(!pdest || !s || !len || (0>(ret =  utility_sntol(&val,s,len))))
		return util_eError;

	/* don't truncate make caller be explicit about intentions */
	if(val < INT_MIN || val > INT_MAX)
	{
		errno = ERANGE;
		return util_eError;
	}

	*pdest = (int)val;
	return ret;
}


int
utility_strtod(
        double *dst,
        const char *s
        )
{
    static const double epsilion = 0.0001F; /* tolerence in conversion error */
    static const double pad      = 0.5F;    /* ammount to pad up/down rounded values */
    static const int scale       = 100;     /* scale between fractional and integer portions */

    long    long_value;
    double  double_value,
            dbl;

    /* get rough value */
    errno = EINVAL;
    if(utility_strtod_raw(&dbl,s))
            return -1;

    /* extract integer and fractional portions */
    errno = ERANGE;
    dbl *= scale;
    if(utility_round(&double_value,&long_value,dbl,pad))
        return -1;

    /* ensure error is within tolerence */
    if(!( ( utility_diff_within_delta( utility_fabs(double_value - dbl), epsilion)) && ( utility_diff_within_delta( utility_fabs( (((double)long_value)-dbl) ), epsilion)) ))
        return -1;

    /* this is a valid double */
    *dst = double_value;
    return 0;
}

static int
utility_strtod_raw(
        double *dst,
        const char *s
        )
{
    static const double delta    = 0.0005F; /* tolerence in equality tests */
    errno = EINVAL;
    if(!s || !dst)
        return -1;

    /* check for decimal places */
    errno = EDOM;
    if(!strchr(s,'.'))
        return -1;

    /* atof is a bit crusty with indicating error returns but it's widely available */
    *dst = atof(s);
    return (0 - (utility_approximately_zero(*dst,delta)));
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
utility_approximately_zero(
        const double v,
        const double delta
        )
{
    return utility_diff_within_delta((v-0.0F),delta);
}

double
utility_fabs(
        const double v
        )
{
    return (v>0.0)? v : -v;
}

int
utility_round(
        double *dst_double,
        long  *dst_long,
        double dbl,
        const double pad
        )
{
    if(!dst_long || !dst_double)
        return -1;

    /* round up to nearest integer */
    dbl = ( ( dbl < 0.0) ?  (dbl-pad) : (dbl+pad) );

    /* truncate to integer representation of value
     * returning both long and double representations */
    *dst_long   = (long)dbl;
    dbl         = (double)(*dst_long);
    *dst_long   = (long)dbl;
    *dst_double = (double)(*dst_long);
    return 0;
}



#if defined( __cplusplus)
    }
#endif


