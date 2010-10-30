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

enum { vector_eOwner=1 };

struct util_vector_tag
{
    /* basic idea is to dole out blocks from here and realloc when needed */
    uint32_t nblocks;       /* number of blocks currently in use, alloced and used*/
    uint32_t navailable;    /* number of blocks currently, alloced and unused */
    uintptr_t *blocks;          /* block table pointer */
    uint32_t flag;
};

/* Obtain footprint (number of bytes ocuppied by storage pointers) for a vector obtained from a previous call to;
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success modifiying dest_nentries  and dest_sz
 *
 * util_eInvalidArguments: invalid args passed
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_capacity()
 * */
static int
utility_vector_footprint(
        const util_vector_t * const vector,
        uint32_t *dest_nentries,
        size_t *dest_sz
        );

/* Determine if a vector is mutable or immutable
 *
 * util_eInvalidArguments: vector was NULL
 * EACCESS: attempted to modify an immutable vector
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_at()
 * utility_vector_readonly()
 * */
static int
utility_vector_readonly(
        const util_vector_t * const vector
        );

/* Increase footprint (number of bytes ocuppied by storage pointers)
 * for a vector obtained from a previous call to:
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success increasing capacity by the next polynomial prime larger then the requested increase
 *
 * util_eInvalidArguments: invalid args passed
 * ERANGE: increase overflows range of uint32_t
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_footprint()
 * utility_vector_polynomial_prime()
 * realloc()
 * calloc()
 * */
static int
utility_vector_increase(
        util_vector_t *vector,
        uint32_t increase
        );

/* Create mutable vector without allocating any storage for contents
 * Returns NULL on error setting errno
 * Returns pointer to vector
 * Function may fail and set p2p_get_errno() for same reasons as
 * malloc()
 * */
util_vector_t *
utility_vector_init(
        void
        )
{
    util_vector_t *vector = malloc(sizeof(*vector));
    if(!vector)
        return NULL;

    vector->blocks  = NULL;
    vector->nblocks = 0;
    vector->navailable  = 0;
    vector->flag = 0;
    return vector;
}

/* Create mutable vector, allocating storage for contents
 *
 * Returns NULL on error setting errno
 * Returns pointer to vector with suffient room to hold capacity elements
 * without additional reallocation of memory
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_init()
 * utility_vector_increase()
 * */
util_vector_t *
utility_vector_init_with_capacity(
        uint32_t reserve
        )
{
    util_vector_t *vector;
    vector = utility_vector_init();
    if(!vector)
        return NULL;

    if(utility_vector_increase(vector,reserve))
    {
        utility_vector_finit(vector);
        vector = NULL;
    }
    assert(vector->navailable >= reserve && "init_with_capacity failed");

    return vector;
}

/* Create mutable vector from splitting a delimited string,
 * The input string is tokenised and then each token duplicated using the user supplied allocator_func
 * the original input string is unmodifed
 *
 * Returns NULL on error setting errno
 * Returns pointer to mutable vector initialised from input string.
 *
 * util_eInvalidArguments: input string was NULL

 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_split()
 * utility_vector_init_with_capacity()
 * */
util_vector_t *
utility_vector_init_from_delimited_string_with_allocator(
        const char *s,
        int delim,
        char * (*allocator_func)(void * userdata, const char *),
        void *userdata
        )
{
    uint32_t nflds =0;
    size_t nelem =0;
    char **fldv;
    int errs;
    uintptr_t ptr;
    util_vector_t *vector;

    p2p_set_errno(util_eInvalidArguments);
    if(!s)
        return NULL;

    /* chop a string up into a container and pointers to string segments formed from delimited token */
    fldv = utility_split(&nelem,s,delim);
    if(!fldv)
        return NULL;

    if(nelem > UINT32_MAX)
    {
        free(fldv[0]);
        free(fldv);
        /* really this would overflow range of uint32_t */
        p2p_set_errno(util_eOpNotSupp);
        return NULL;
    }
    else
    {
        nflds = (uint32_t)nelem;
    }


    /* allocate sufficent room for all tokens */
    vector = utility_vector_init_with_capacity(nflds);
    if(!vector)
    {
        /* cleanup container and contents */
        errs = p2p_get_errno();
        free(fldv[0]);
        free(fldv);

        /* restore original p2p_get_errno() */
        p2p_set_errno(errs);
        fldv = NULL;
        vector = NULL;
        return NULL;
    }

    /* Create immutable vector from splitting a delimited string,
     * The input string is duplicated and a local copy is held by the vector with
     * the original input string unmodifed.*/
    if(!allocator_func)
    {
        /* make vector immutable since we now own the content */
        vector->flag |= vector_eOwner;
        for( vector->navailable -=nflds; nflds; ++vector->nblocks, --nflds)
        {
            /* store offset of each token */
            ptr = ((uintptr_t)fldv[vector->nblocks]);
            vector->blocks[vector->nblocks] = ptr;
        }
        /* cleanup container as we maintain copy of content */
        free(fldv);
        fldv=NULL;
        return vector;

    }

    /* Create mutable vector from splitting a delimited string,
     * The input string is tokenised and then each token duplicated using the user supplied allocator_func
     * the original input string is unmodifed */

    /* make vector mutable since user now owns the content */
    vector->flag &= ~vector_eOwner;
    for( vector->navailable -=nflds; nflds; ++vector->nblocks, --nflds)
    {
        /* make copies of each token */
        ptr = (uintptr_t) (*allocator_func)(userdata,fldv[vector->nblocks]);
        vector->blocks[vector->nblocks] = ptr;
    }
    /* cleanup container and contents as user own that memory */
    free(fldv[0]);
    free(fldv);
    fldv=NULL;
    return vector;
}

/* Create immutable vector from splitting a delimited string,
 * The input string is duplicated and a local copy is held by the vector with
 * the original input string unmodifed.
 *
 * Returns NULL on error setting errno
 * Returns pointer to immutable vector initialised from input string.
 *
 * util_eInvalidArguments: input string was NULL

 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_split()
 * utility_vector_init_with_capacity()
 * */
util_vector_t *
utility_vector_init_from_delimited_string(
        const char *s,
        int delim
        )
{
    return utility_vector_init_from_delimited_string_with_allocator(s,delim,NULL,NULL);
}



/* Free a vector obtained from a previous call to;
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * */
void
utility_vector_finit(
        util_vector_t *vector
        )
{
    if(vector)
    {
        assert(util_eOk == utility_vector_flush(vector));
        free(vector);
        vector = NULL;
    }
}

/* recycle vector contents obtained from a previous call to;
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 * leave storage for entries allocated
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * EEXIST: no entries in vector
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_at()
 * */
int
utility_vector_recycle(
        util_vector_t *vector
        )
{
    uintptr_t content_head;
    char *str;
    p2p_set_errno(util_eInvalidArguments);
    if(!vector)
        return util_eError;

    p2p_set_errno(EEXIST);
    if(!vector->nblocks)
        return util_eError;

    /* we own the content so must release it */
    if(vector_eOwner == (vector->flag & vector_eOwner))
    {
        if(utility_vector_at(vector,&content_head,0))
            return util_eError;

        /* strip owner bit as we no longer "own" content pointer */
        vector->flag &= (~vector_eOwner);
        str = (char *)content_head;
        free(str);
    }

    /* caller is supposed to own the "contained" storage while we own the "container" */
    vector->nblocks = 0;
    return util_eOk;
}

/* recycle vector contents obtained from a previous call to;
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 * reclaiming storage for entries allocated
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 *
 * EEXIST: no entries in vector
 * util_eInvalidArguments: invalid args passed
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_recycle()
 * */
int
utility_vector_flush(
        util_vector_t *vector
        )
{
    p2p_set_errno(util_eInvalidArguments);
    if((!vector) || ((vector->nblocks >0) && util_eError == utility_vector_recycle(vector)))
        return util_eError;

    free(vector->blocks);
    vector->blocks  = NULL;
    vector->navailable  = 0;
    return util_eOk;
}

/* Obtain capacity (number of possible entries) of a vector obtained from a previous call to;
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 *
 * util_eInvalidArguments: invalid args passed
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * */
int
utility_vector_capacity(
        const util_vector_t * const vector,
        uint32_t *dest
        )
{
    const int invalid_args = (!vector || !dest);
    p2p_set_errno(util_eInvalidArguments);
    if(!invalid_args)
        *dest = (vector->nblocks + vector->navailable);
    return 0 - invalid_args;
}

/* Obtain size (number of actual entries) of a vector obtained from a previous call to;
 *  utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set p2p_get_errno() for same reasons as
 * */
int
utility_vector_size(
        const util_vector_t * const vector,
        uint32_t *dest
        )
{
    const int invalid_args = (!vector || !dest);
    p2p_set_errno(util_eInvalidArguments);
    if(!invalid_args)
        *dest = vector->nblocks;
    return 0 - invalid_args;
}

/* Obtain footprint (number of bytes ocuppied by storage pointers) for a vector obtained from a previous call to;
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success modifiying dest_nentries  and dest_sz
 *
 * util_eInvalidArguments: invalid args passed
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_capacity()
 * */
static int
utility_vector_footprint(
        const util_vector_t *vector,
        uint32_t *dest_nentries,
        size_t *dest_sz
        )
{
    p2p_set_errno(util_eInvalidArguments);
    if(utility_vector_capacity(vector,dest_nentries) || !dest_sz)
        return util_eError;

    *dest_sz =  (*dest_nentries  * sizeof(uintptr_t));
    return util_eOk;
}

/* Increase footprint (number of bytes ocuppied by storage pointers)
 * for a vector obtained from a previous call to:
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success increasing capacity by the next polynomial prime larger then the requested increase
 *
 * util_eInvalidArguments: invalid args passed
 * ERANGE: increase overflows range of uint32_t
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_footprint()
 * utility_vector_polynomial_prime()
 * realloc()
 * calloc()
 * */
static int
utility_vector_increase(
        util_vector_t *vector,
        uint32_t  increase
        )
{
    uint32_t  entries;
    size_t sz;
    void *ptr;

    if(utility_vector_footprint(vector,&entries,&sz))
        return util_eError;

    /* this list might already be crammed to the roof, causing counter to wrap */
    p2p_set_errno(ERANGE);
    increase = utility_vector_polynomial_prime(entries+increase);
    if( increase <= entries)
        return util_eError;

    /* account for the existing entries */
    increase = (increase - entries);

    /* Allocate room for new pointers */
    sz += (sizeof(uintptr_t) * increase);
    ptr = (entries) ? realloc(vector->blocks,sz) : calloc(1,sz);
    if(!ptr)
        return util_eError;

    /* update the storage and free counts */
    vector->blocks = ptr;
    vector->navailable += increase;
    return util_eOk;
}

/* Store an entry at end of a mutable vector obtained from a previous call to:
 * utility_vector_init() or utility_vector_init_with_capacity() increasing capacity if needed
 *
 * Returns UINT32_MAX on error setting errno
 * Returns index of stored data on success
 *
 * util_eInvalidArguments: invalid args passed
 * EACCESS: attempted to modify an immutable vector
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_increase()
 * */
uint32_t
utility_vector_put_back(
        util_vector_t *vector,
        const uintptr_t data
        )
{
    if(util_eOk != utility_vector_readonly(vector))
    {
        p2p_set_errno((vector)? EACCES : util_eInvalidArguments);
        return UINT32_MAX;
    }

    if(!vector->navailable && utility_vector_increase(vector,1))
        return UINT32_MAX;

    --vector->navailable;
    vector->blocks[vector->nblocks] = data;
    return vector->nblocks++;
}

/* Store an entry immediately preceding the specified index into a mutable vector obtained from a previous call to:
 * utility_vector_init() or utility_vector_init_with_capacity() increasing capacity if needed
 *
 * Returns UINT32_MAX on error setting errno
 * Returns index of stored data on success
 *
 * util_eInvalidArguments: invalid args passed
 * EACCESS: attempted to modify an immutable vector
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_capacity()
 * utility_vector_increase()
 * */
uint32_t
utility_vector_insert_before(
        util_vector_t *vector,
        const uintptr_t data,
        const uint32_t idx
        )
{
uint32_t entries;
int available;

    if(util_eOk != utility_vector_readonly(vector))
    {
        p2p_set_errno((vector)? EACCES : util_eInvalidArguments);
        return UINT32_MAX;
    }

    if(utility_vector_capacity(vector,&entries))
        return UINT32_MAX;

    available = ((idx < entries) && vector->navailable);
    if(!available && utility_vector_increase(vector,idx))
        return UINT32_MAX;

    --vector->navailable;
    /* shuffle everything down one place */
    memmove(vector->blocks+idx+1u,vector->blocks+idx,vector->nblocks-idx);
    vector->blocks[idx] = data;
    return vector->nblocks++;
}

/* Retrive entry at specified index into a vector obtained from a previous call to:
 *  utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success modifiying dest to hold content
 *
 * util_eInvalidArguments: dest or vector is NULL
 * EXIST : index out of range
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * */
int
utility_vector_at(
        const util_vector_t *vector,
        uintptr_t *dest,
        const uint32_t idx
        )
{
    p2p_set_errno(util_eInvalidArguments);
    if(!dest || !vector)
        return util_eError;

    p2p_set_errno(EEXIST);
    if( !(vector->blocks) ||  (idx >= vector->nblocks))
        return util_eError;

    *dest = vector->blocks[idx];
    return util_eOk;
}

/* Exchange entry at specified index into a mutable vector obtained from a previous call to:
 * utility_vector_init() or utility_vector_init_with_capacity()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success modifiying dest to hold content
 *
 * util_eInvalidArguments: dest or vector is NULL
 * EXIST : index out of range
 * EACCESS: attempted to modify an immutable vector
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * */
int
utility_vector_exchange(
        const util_vector_t *vector,
        uintptr_t *dest,
        const uintptr_t data,
        const uint32_t idx
        )
{
    p2p_set_errno(util_eInvalidArguments);
    if(!dest || !vector)
        return util_eError;

    if(util_eOk != utility_vector_readonly(vector))
        return util_eError;

    p2p_set_errno(EEXIST);
    if(!(idx < vector->nblocks && vector->blocks))
        return util_eError;

    *dest = vector->blocks[idx];
    vector->blocks[idx] = data;
    return util_eOk;
}

/* collect all the entries of a mutable vector obtained from a previous call to:
 * utility_vector_init() or utility_vector_init_with_capacity() into an array of uintptr_t
 *
 * Returns -1 on error setting errno
 * Returns 0 on success modifiying dest to hold content
 *
 * util_eInvalidArguments: dest or vector is NULL
 * EXIST : index out of range
 * EACCESS: attempted to modify an immutable vector
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_readonly()
 * */
int
utility_vector_contents(
        const util_vector_t *vector,
        uintptr_t **dest_list,
        uint32_t *dest_sz
        )
{
    int valid_args = (dest_list && vector && dest_sz);

    if(util_eOk != utility_vector_readonly(vector))
        return util_eError;


    if(valid_args)
    {
        *dest_sz   = vector->nblocks;
        *dest_list = vector->blocks;
    }
    return 0 - !valid_args ;
}

/* Determine if a vector is mutable or immutable
 *
 * util_eInvalidArguments: vector was NULL
 * EACCESS: attempted to modify an immutable vector
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_at()
 * */
static int
utility_vector_readonly(
        const util_vector_t * const vector
    )
{
    if(!vector)
    {
        p2p_set_errno(util_eInvalidArguments);
        return util_eError ;
    }

    /* treat content we own as immutable to simplfy memory management */
    if(vector_eOwner == (vector->flag & vector_eOwner))
    {
        p2p_set_errno(EACCES);
        return 1;
    }

    /* vector is valid and mutable */
    return 0;
}

/* Destructively retrieve last entry from a mutable vector obtained from a previous call to:
 * utility_vector_init() or utility_vector_init_with_capacity() increasing capacity if needed
 *
 * Returns UINT32_MAX on error setting errno
 * Returns index of stored data on success
 *
 * util_eInvalidArguments: invalid args passed
 * EACCESS: attempted to modify an immutable vector
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_readonly()
 * */

int
utility_vector_delete_back(
        util_vector_t *vector,
        uintptr_t *dest
        )
{
    int ret;
    p2p_set_errno(util_eInvalidArguments);
    if(!vector || !vector->nblocks || utility_vector_readonly(vector))
        return util_eError;

    if(!(ret = utility_vector_at(vector,dest,vector->nblocks-1)))
    {
        vector->blocks[--vector->nblocks] = (uintptr_t)NULL;
        ++vector->navailable;
    }
    return ret;
}

/* Determine if vector contains any entries
 *
 * Returns -1 on error setting p2p_get_errno() and leaving dest unmodifed
 * Returns  0 on success modifiying dest
 */
int
utility_vector_is_empty(
        const util_vector_t * const vector,
        int *dest
        )
{
     p2p_set_errno(util_eInvalidArguments);
    if(!vector || !dest)
        return util_eError;

    *dest = (0 == vector->nblocks);
    return util_eOk;
}

/* Determine if vector is mutable
 *
 * Returns -1 on error setting p2p_get_errno() and leaving dest unmodifed
 * Returns  0 on success modifiying dest
 */
int
utility_vector_is_readonly(
        const util_vector_t * const vector,
        int *dest
        )
{
int ret;
    p2p_set_errno(util_eInvalidArguments);
    if(!vector || !dest)
        return util_eError;

    ret = utility_vector_readonly(vector);
    if(util_eError == ret)
        return util_eError;

    *dest = ret;
    return util_eOk;
}


/* Non Destructively retrieve last entry from a vector obtained from a previous call to:
 * utility_vector_init() or utility_vector_init_with_capacity()
 *
 * util_eInvalidArguments: invalid args passed
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_is_readonly()
 * */

int
utility_vector_at_tail(
        util_vector_t *vector,
        size_t *dest
        )
{
int empty=1;
    p2p_set_errno(util_eInvalidArguments);
    if(!vector || !dest)
        return util_eError;

    if(utility_vector_is_empty(vector,&empty))
        return util_eError;

    p2p_set_errno(ENOENT);
    if(empty)
        return util_eError;

    return utility_vector_at(vector,dest,vector->nblocks-1);
}



/* Table of prime numbers 2^n+au, 2<=n<=30.  */
/* lookup table constants taken from public domain st_hash.c */
static const uint32_t g_polynomial_primes[30] = {
    8U+3u,		16U+3u,		        32U+5u,		    64U+3u,
    128U+3u,		256U+27u,		512U+9u,	    1024U+9u,
    2048U+5u,		4096U+3u,		8192U+27u,	    16384U+43u,
    32768U+3u,		65536U+45u,		131072U+29u,	    262144U+3u,

    524288U+21u,	1048576U+7u,		2097152U+17u,	    4194304U+15u,
    8388608U+9u,	16777216U+43u,		33554432U+35u,	    67108864U+15u,
    134217728U+29u,	268435456U+3u,		536870912U+11u,	    1073741824U+85u,
    0u,                  0
};

static const int g_debruijn32[32] = {
     0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
       8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
};

#define DEBRUIJN_MASK  0x07C4ACDDUL
#define DEBRUIJN_SHIFT 27u
/* Returns a 32 bit polynomial prime larger then i or zero on error with errno=ERANGE
 */
uint32_t
utility_vector_polynomial_prime(
    uint32_t i
    )
{
uint32_t v = i;
    /* map values 0..31 onto range 0..29 folding [0 .. 2] into the same value*/
    p2p_set_errno(ERANGE);

    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    /* set i to index of msb of size using debruijn lookup */
    v = (uint32_t) g_debruijn32[((v * DEBRUIJN_MASK) >> DEBRUIJN_SHIFT)];

    if(v<=2)
        return g_polynomial_primes[0];

    /* finally select the immediately larger prime from the list */
    return g_polynomial_primes[v-2];
}

#if defined( __cplusplus)
    }
#endif
