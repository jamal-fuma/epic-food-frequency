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
 * Portions created by FumaSoftware Ltd are Copyright (C) 2009-2010 FumaSoftware
 * Ltd.
 *
 * Portions created by Jamal Natour are Copyright (C) 2009-2010
 * FumaSoftware Ltd and Jamal Natour.
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
 */

#if defined( __cplusplus)
extern "C" {
#endif

#include "libUtility.h"

/* Finds a name for a value given a key and parallel arrays of names and values
 * Return  NULL on error setting errno on error
 * Returns on success, pointer to element of 'names' at same offset as  first occurence of 'key' in 'values'
 * EINVAL: invalid args passed
 * ERANGE: max is too large*/
char *
utility_find_name_by_u8_str(const char *prefix,uint8_t key, const char **names, const uint8_t *values, size_t max)
{
    errno = EINVAL;
    if(names && values && prefix && max)
    {
        const char * symbol  = utility_find_name_by_u8(key,names,values,max);

        return (!symbol) ?
                utility_sprintf("%s='%-10c'\t",prefix,key)  :
                utility_sprintf("%s='%-10s'\t",prefix,symbol) ;
    }
    return NULL;
}

/* Finds a name for a value given a key and parallel arrays of names and values
 * Return  NULL on error setting errno on error
 * Returns on success, pointer to element of 'names' at same offset as  first occurence of 'key' in 'values'
 * EINVAL: invalid args passed
 * ERANGE: max is too large*/
char *
utility_find_name_by_u16_str(const char *prefix,uint16_t key, const char **names, const uint16_t *values, size_t max)
{
    errno = EINVAL;
    if(names && values && prefix && max)
    {
        const char * symbol  = utility_find_name_by_u16(key,names,values,max);

        return (!symbol) ?
                utility_sprintf("%s='%-10u'\t",prefix,key)  :
                utility_sprintf("%s='%-10s'\t",prefix,symbol) ;
    }
    return NULL;
}

/* Finds a name for a value given a key and parallel arrays of names and values
 * Return  NULL on error setting errno on error
 * Returns on success, pointer to element of 'names' at same offset as  first occurence of 'key' in 'values'
 * EINVAL: invalid args passed
 * ERANGE: max is too large*/
char *
utility_find_name_by_u32_str(const char *prefix,uint32_t key, const char **names, const uint32_t *values, size_t max)
{
    errno = EINVAL;
    if(names && values && prefix && max)
    {
        const char * symbol  = utility_find_name_by_u32(key,names,values,max);

        return (!symbol) ?
                utility_sprintf("%s='%-10u'\t",prefix,key)  :
                utility_sprintf("%s='%-10s'\t",prefix,symbol) ;
    }
    return NULL;
}


/* Finds a name for a value given a key and parallel arrays of names and values
 * Return  NULL on error setting errno on error
 * Returns on success, pointer to element of 'names' at same offset as  first occurence of 'key' in 'values'
 * EINVAL: invalid args passed
 * ERANGE: max is too large*/
const char *
utility_find_name_by_u8(uint8_t key, const char **names, const uint8_t *values, size_t max)
{
    ssize_t i;

    errno = EINVAL;
    if(!names || !values)
        return NULL;

    /* cant safely deal with that large an object with current impl
     * besides this is way too slow for anything but tiny lists */
    errno = ERANGE;
    if(max > SSIZE_MAX)
        return NULL;

    i = (ssize_t)max;
    while(--i >= 0)
        if(values[i] == key)
            return names[i];
    return NULL;
}

/* Finds a name for a value given a key and parallel arrays of names and values
 * Return  NULL on error setting errno on error
 * Returns on success, pointer to element of 'names' at same offset as  first occurence of 'key' in 'values'
 * EINVAL: invalid args passed
 * ERANGE: max is too large*/
const char *
utility_find_name_by_u16(uint16_t key, const char **names, const uint16_t *values, size_t max)
{
    ssize_t i;

    errno = EINVAL;
    if(!names || !values)
        return NULL;

    /* cant safely deal with that large an object with current impl
     * besides this is way too slow for anything but tiny lists */
    errno = ERANGE;
    if(max > SSIZE_MAX)
        return NULL;

    i = (ssize_t)max;
    while(--i >= 0)
        if(values[i] == key)
            return names[i];
    return NULL;
}

/* Finds a name for a value given a key and parallel arrays of names and values
 * Return  NULL on error setting errno on error
 * Returns on success, pointer to element of 'names' at same offset as  first occurence of 'key' in 'values'
 * EINVAL: invalid args passed
 * ERANGE: max is too large*/
const char *
utility_find_name_by_u32(uint32_t key, const char **names, const uint32_t *values, size_t max)
{
    ssize_t i;

    errno = EINVAL;
    if(!names || !values)
        return NULL;

    /* cant safely deal with that large an object with current impl
     * besides this is way too slow for anything but tiny lists */
    errno = ERANGE;
    if(max > SSIZE_MAX)
        return NULL;

    i = (ssize_t)max;
    while(--i >= 0)
        if(values[i] == key)
            return names[i];
    return NULL;
}

/* Finds a name for a value given a key and parallel arrays of names and values
 * Return  NULL on error setting errno on error
 * Returns on success, pointer to element of 'names' at same offset as  first occurence of 'key' in 'values'
 * EINVAL: invalid args passed
 * ERANGE: max is too large*/
const char *
utility_find_name_by_u64(uint64_t key, const char **names, const uint64_t *values, size_t max)
{
    ssize_t i;

    errno = EINVAL;
    if(!names || !values)
        return NULL;

    /* cant safely deal with that large an object with current impl
     * besides this is way too slow for anything but tiny lists */
    errno = ERANGE;
    if(max > SSIZE_MAX)
        return NULL;

    i = (ssize_t)max;
    while(--i >= 0)
        if(values[i] == key)
            return names[i];
    return NULL;
}

/* Finds a name for a value given a key and parallel arrays of names and values
 * Return  NULL on error setting errno on error
 * Returns on success, pointer to element of 'names' at same offset as  first occurence of 'key' in 'values'
 * EINVAL: invalid args passed
 * ERANGE: max is too large*/
const char *
utility_find_name_by_str(const char * key, const char **names, const char **values, size_t max)
{
    ssize_t i;

    errno = EINVAL;
    if(!names || !values)
        return NULL;

    /* cant safely deal with that large an object with current impl
     * besides this is way too slow for anything but tiny lists */
    errno = ERANGE;
    if(max > SSIZE_MAX)
        return NULL;

    i = (ssize_t)max;
    while(--i >= 0)
        if(!strcmp(values[i],key))
            return names[i];
    return NULL;
}



#if defined( __cplusplus)
    }
#endif
