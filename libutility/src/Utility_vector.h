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

#ifndef UTILITY_VECTOR_H
#define UTILITY_VECTOR_H

#if defined( __cplusplus)
extern "C" {
#endif

#include "libUtility.h"

typedef struct util_vector_tag util_vector_t;

/* Create mutable vector without allocating any storage for contents
 *
 * Returns NULL on error setting errno
 * Returns pointer to mutable vector on success
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * malloc()
 * */
util_vector_t *
utility_vector_init(
        void
        );

/* Create mutable vector, reserving storage for contents
 *
 * Returns NULL on error setting errno
 * Returns pointer to mutable vector with suffient room to hold capacity elements
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_init()
 * utility_vector_increase()
 * */
util_vector_t *
utility_vector_init_with_capacity(
        uint32_t reserve
        );

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
        );

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
        );

/* Free a vector obtained from a previous call to;
 *  utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * */
void
utility_vector_finit(
        util_vector_t *vector
        );

/* recycle vector contents obtained from a previous call to;
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * The effect of recyling an immutable vector result in conversion of a immutable vector to a mutable vector
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 *
 * EEXIST: no entries in vector
 * util_eInvalidArguments: invalid args passed
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_at()
 * */
int
utility_vector_recycle(
        util_vector_t *vector
        );

/* recycle vector contents obtained from a previous call to;
 * utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 * reclaiming storage for entries allocated
 *
 * The effect of flushing an immutable vector results in conversion of a immutable vector to a mutable vector
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
        );

/* Obtain capacity (number of possible entries) of a vector obtained from a previous call to
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
        );

/* Obtain size (number of actual entries) of a vector obtained from a previous call to;
 *  utility_vector_init() or its's variants utility_vector_init_with_capacity() and utility_vector_init_from_delimited_string()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 *
 * util_eInvalidArguments: invalid args passed
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * */
int
utility_vector_size(
        const util_vector_t * const vector,
        uint32_t *dest
        );

/* Store an entry at end of a mutable vector obtained from a previous call to:
 * utility_vector_init() or utility_vector_init_with_capacity() increasing capacity if needed
 *
 * Returns UINT32_MAX on error setting errno
 * Returns index of stored data on success
 * util_eInvalidArguments: invalid args passed
 * EACCESS: attempted to modify an immutable vector
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_increase()
 * */
uint32_t
utility_vector_put_back(
        util_vector_t *vector,
        const uintptr_t data
        );

/* Store an entry immediately preceding the specified index into a mutable vector obtained from a previous call to:
 * utility_vector_init() or utility_vector_init_with_capacity() increasing capacity if needed
 *
 * Returns UINT32_MAX on error setting errno
 * Returns index of stored data on success
 * util_eInvalidArguments: invalid args passed
 * EACCESS: attempted to modify an immutable vector
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_capacity()
 * utility_vector_increase()
 * */
uint32_t
utility_vector_insert_before(
        util_vector_t *vector,
        const uintptr_t data,
        const uint32_t idx
        );

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
        );

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
        );


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
 * utility_vector_is_readonly()
 * */
int
utility_vector_contents(
        const util_vector_t *vector,
        uintptr_t **dest_list,
        uint32_t *dest_sz
        );


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
 * utility_vector_is_readonly()
 * */

int
utility_vector_delete_back(
        util_vector_t *vector,
        uintptr_t *dest
        );


/* Determine if vector contains any entries
 *
 * Returns -1 on error setting p2p_get_errno() and leaving dest unmodifed
 * Returns  0 on success modifiying dest
 */
int
utility_vector_is_empty(
        const util_vector_t * const vector,
        int *dest
        );

/* Determine if a vector is mutable or immutable
 *
 * util_eInvalidArguments: vector was NULL
 * EACCESS: attempted to modify an immutable vector
 *
 * Function may fail and set p2p_get_errno() for same reasons as
 * utility_vector_at()
 * */
int
    utility_vector_is_readonly(
            const util_vector_t * const vector,
            int *dest
            );


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
        );

/* Returns a 32 bit polynomial prime larger then i or zero on error with errno=ERANGE
 */
uint32_t
utility_vector_polynomial_prime(
    uint32_t i
    );

#if defined( __cplusplus)
    }
#endif


#endif /* ndef UTILITY_VECTOR_H */
