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
        utility_s_vector_put_back(
                util_vector_t *vector,
                const char *sdata
                );

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
        utility_s_vector_insert_before(
                util_vector_t *vector,
                const char *sdata,
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
        utility_s_vector_at(
                const util_vector_t *vector,
                char **dest,
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
        utility_s_vector_exchange(
                const util_vector_t *vector,
                char  **dest,
                const char *sdata,
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
     * utility_vector_readonly()
     * */
    int
        utility_s_vector_contents(
                const util_vector_t *vector,
                char ***dest_list,
                uint32_t *dest_sz
                );

    /* visit all the entries of a vector obtained from a previous call to:
     * utility_vector_init() or utility_vector_init_with_capacity()
     *
     * returns index of entry that caused func to return non zero in retval
     * returns -1 if the whole list was traversed without func returning non-zero
     * returns -2 setting errno on error
     * util_eInvalidArguments: dest or vector is NULL
     * EXIST : index out of range
     * EACCESS: attempted to modify an immutable vector
     *
     * Function may fail and set p2p_get_errno() for same reasons as
     * utility_vector_readonly()
     * utility_for_each_in_list_with_index_and_state()
     * */
    int
        utility_s_vector_visit(
                const util_vector_t *vector,
                int *retval,
		int (*vistor)(void * /* parg */, size_t /* index */,void * /* pstate */),
                void *vistor_arg
                );

    /* Destructively retrieve last entry from a mutable vector obtained from a previous call to:
     * utility_vector_init() or utility_vector_init_with_capacity() increasing capacity if needed
     *
     * util_eInvalidArguments: invalid args passed
     * EACCESS: attempted to modify an immutable vector
     *
     * Function may fail and set p2p_get_errno() for same reasons as
     * utility_vector_readonly()
     * */

    int
        utility_s_vector_delete_back(
                util_vector_t *vector,
                char **dest
                );

    /* Non Destructively retrieve last entry from a mutable vector obtained from a previous call to:
     * utility_vector_init() or utility_vector_init_with_capacity() increasing capacity if needed
     *
     * util_eInvalidArguments: invalid args passed
     * EACCESS: attempted to modify an immutable vector
     *
     * Function may fail and set p2p_get_errno() for same reasons as
     * utility_vector_is_readonly()
     * */

    int
        utility_s_vector_at_tail(
                util_vector_t *vector,
                char **dest
                );

#if defined( __cplusplus)
}
#endif
