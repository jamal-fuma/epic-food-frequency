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
#ifndef UTILITY_TOKEN_H
#define UTILITY_TOKEN_H

#if defined( __cplusplus)
extern "C" {
#endif

typedef struct util_token_tag util_token_t;

 /* Create token using buffer as storage for contents
 *
 * Returns NULL on failure setting errno
 * Returns token on success,
 *
 * EINVAL: null buffer passed
 *
 * Function may fail and set errno for same reasons as
 * malloc()
 * */
util_token_t *
utility_token_alloc(
        uint32_t len
        );


 /* Release token obtained from a previous call to utility_token_alloc
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success,
 *
 * EINVAL: null token passed
 *
 * Function may fail and set errno for same reasons as
 * malloc()
 * */
int
utility_token_free(
        util_token_t *ptoken
        );

/* Destructively obtain buffer contents converting to double
 *
 * Returns -1 on failure setting errno
 * Returns number of unconverted trailing char on success,
 *
 * EINVAL: invalid args passed*/
int
utility_token_double(
        util_token_t *ptoken,
        double *dest
        );

/* Destructively obtain buffer contents converting to long
 *
 * Returns -1 on failure setting errno
 * Returns number of unconverted trailing char on success,
 *
 * EINVAL: invalid args passed*/
int
utility_token_long(
        util_token_t *ptoken,
        long *dest
        );


int utility_token_set_type(util_token_t *ptoken,int type);
int utility_token_get_type(util_token_t *ptoken,int *ptype);
int utility_token_is_empty(util_token_t *ptoken);

int utility_token_append(util_token_t *ptoken,int ch);

/* Append a string to the token
 * Returns -1 on error setting errno
 * Returns  0 on success
 * EINVAL: invalid args passed
 * ENOBUFS: write would cause overflow */
int
utility_token_append_str(
        util_token_t *ptoken,
        const char *str
);

/* Destructively obtain last appended byte
 *
 * Returns EOF on failure setting errno
 * Returns byte on success,
 *
 * EOF may be returned as a 'success' return when underlying buffer is empty errno is not modified in this case.
 * the two cases, (valid call of empty buffer and invalid call) can be distinguished by setting errno to zero prior to call and test after return
 *
 * EINVAL: invalid args passed*/
int
utility_token_getc(
        util_token_t *ptoken
        );

char * utility_token_flush(util_token_t *ptoken);

int utility_token_clear(util_token_t *ptoken);

#if defined( __cplusplus)
    }
#endif

#endif /* ndef UTILITY_TOKEN_H */
