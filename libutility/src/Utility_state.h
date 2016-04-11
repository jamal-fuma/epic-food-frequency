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
#ifndef UTILITY_STATE_H
#define UTILITY_STATE_H

#if defined( __cplusplus)
extern "C" {
#endif

#include "libUtility.h"

/* A Table of state rules */
typedef struct util_state_table_tag     util_state_table_t;

/* A State transition rule */
typedef struct util_state_rule_tag      util_state_rule_t;

/* A match forcing a transition to another rule */
typedef struct util_state_match_tag     util_state_match_t;

typedef                     uint32_t    util_state_number_t;
typedef                     uint64_t    util_state_counter_t;
typedef                     uint16_t    util_state_action_t;

#define MAX_STATE_LEN 63

    /* token patterns */
#define  DETECT_NEWLINE 0x0A            /* \n   */
#define  DETECT_CARRIAGE_RETURN 0x0D    /* \r   */
#define  DETECT_COMMA   ','             /*  ,   */
#define  DETECT_DQUOTE  0x22            /*  "   */
#define  DETECT_SQUOTE  0x27            /*  '   */
#define  DETECT_STAR    0x2A            /*  *   */
#define  DETECT_ANY     1	        /* .	   */
#define  DETECT_DIGIT   2	        /* isdigit */
#define  DETECT_ALPHA   3    	        /* isalpha */
#define  DETECT_ALNUM   4    	        /* isalnum */
#define  DETECT_XDIGIT  5	        /* isxdigit*/
#define  DETECT_WSPACE  6	        /* isspace */
#define  DETECT_BLANK   7	        /* isblank */

#define  DETECT_SLASH   0x2f  /*  /   */
#define  DETECT_BSLASH  0x5C  /*  \   */

/* actions peformed on state transitions */
#define  TRIGGER_ESCAPE  	1<<0
#define  TRIGGER_DELETE  	1<<1
#define  TRIGGER_PRINT   	1<<2
#define  TRIGGER_HOLD    	1<<3
#define  TRIGGER_FLUSH   	1<<4
#define  TRIGGER_ERROR   	1<<5
#define  TRIGGER_UNGET  	1<<6
#define  TRIGGER_SPACE	 	1<<7
#define  TRIGGER_FLUSH_N 	1<<8
#define  TRIGGER_EXCHANGE 	1<<9
#define  TRIGGER_PUSH_HOLD	1<<10
#define  TRIGGER_PUSH_BACK	1<<11
#define  TRIGGER_POP_HOLD	1<<12
#define  TRIGGER_FLUSH_HOLD	1<<13
#define  TRIGGER_FLUSH_SPLIT	1<<14
#define  TRIGGER_NEWLINE DETECT_NEWLINE
#define  TRIGGER_SLASH   DETECT_SLASH
#define  TRIGGER_UNKNOWN	0x1FFF

/* Allocate a new state transition rule
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/
util_state_rule_t *
utility_state_rule_alloc(
	util_state_number_t state_num,	    /* the state that marks the start of this rule */
	uint8_t byte,			    /* the trigger for selecting this rule */
	util_state_action_t action,	    /* the action taken when transitioning out of this rule*/
	util_state_number_t next_state);    /* the state this rules transitions into */

/* Allocate a list of size 'entries' of  new state transition rule
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/
util_state_rule_t *
utility_state_rule_alloc_list(
        util_state_counter_t entries);


/* Free state transition rules obtained from a previous call to utility_state_rule_alloc() or utility_state_rule_alloc_list()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_rule_free(
	util_state_rule_t *pstate);


/* Write a human readable description of a state transition rule to stderr
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_rule_dump_err(
	const util_state_rule_t *pstate);


/* Write a human readable description of a state transition rule to logfile
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_rule_dump(
        const char *logfile,
        const char *mode,
        const util_state_rule_t *pstate);


/* Write a human readable description of a state transition rule to logfile
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_rule_fdump(
	FILE *fp,
	 const util_state_rule_t *pstate);
/* Write a human readable description of a state transition rule to malloc'd buffer
 *
 * Returns NULL on failure setting errno
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_state_rule_str(
        const util_state_rule_t *pstate,
        uint8_t ch);

/* Write a human readable description of a state transition rule trigger to malloc'd buffer
 *
 * Returns NULL on failure setting errno
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_state_rule_describe_trigger(uint8_t value);

/* Write a human readable description of a byte to malloc'd buffer
 *
 * Returns NULL on failure setting errno
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_state_rule_describe_match(uint8_t value);

/* Write a human readable description of a action to malloc'd buffer
 *
 * Returns NULL on failure setting errno
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_state_rule_describe_action(util_state_action_t value);


/* Specify state number for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing ptr
 * EINVAL: invalid args passed*/

int
utility_state_rule_set_state_num(
        util_state_rule_t *ptr,
        const util_state_number_t state_num  );


/* Specify index of next state rule for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing ptr
 * EINVAL: invalid args passed*/

int
utility_state_rule_set_next_state(
        util_state_rule_t *ptr,
        const util_state_number_t next_state  );

/* Specify parser action for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing ptr
 * EINVAL: invalid args passed*/

int
utility_state_rule_set_action(
        util_state_rule_t *ptr,
        const util_state_action_t action  );


/* Specify byte trigger for transition for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing ptr
 * EINVAL: invalid args passed*/

int
utility_state_rule_set_trigger(
        util_state_rule_t *ptr,
        const uint8_t trigger  );


/* Obtain trigger byte for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pch
 * EINVAL: invalid args passed*/
int
utility_state_rule_get_trigger(
	const util_state_rule_t *ptr,
	uint8_t *pch );

/* Obtain Action to perform when transitioning from one state transition rule to another
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing paction
 * EINVAL: invalid args passed*/
int
utility_state_rule_get_action(
	const util_state_rule_t *ptr,
	util_state_action_t *paction );

/* Obtain the number of the next state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pstate_num
 * EINVAL: invalid args passed*/
int
utility_state_rule_get_next_state_num(
	const util_state_rule_t *ptr,
	util_state_number_t *state_num);


/* Obtain the number of current state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pstate_num
 * EINVAL: invalid args passed*/
int
utility_state_rule_get_state_num(
	const util_state_rule_t *ptr,
	util_state_number_t *pstate_num);


/* Determine if 'action' is within accepted range
 * Returns 0 on failure setting errno
 * Returns 1 on success
 * EINVAL: 'action' is out of range*/
int
utility_state_rule_action_is_valid(
	util_state_action_t action);


/* return an entry in a list of state rules obtained from a previous call to utility_state_rule_alloc_list()
 * if 'action' is within accepted range
 * Returns 0 on failure setting errno
 * Returns 1 on success
 * EINVAL: 'action' is out of range*/
util_state_rule_t *
utility_state_rule_list_item(
        util_state_rule_t *pstate,
        util_state_counter_t offset
);

/* write 'max' entries starting as 'offset' in the list of state rules pointed at by 'plist' to 'fp'
 * Returns 0 on failure setting errno
 * Returns 1 on success
 * EINVAL: 'action' is out of range*/
int
utility_state_rule_write_list(
        FILE *fp,
        util_state_rule_t *plist,
        util_state_counter_t offset,
        util_state_counter_t max
);

/* Find an existing rule in a list of state transition rules
 *     obtained from a previous call to utility_state_table_alloc()
 *
 * Returns NULL on error setting errno
 * Returns a const pointer to a state transition rule on success
 * EEXIST: no entries in table
 * ENOENT: no matching entries found in table
 * EINVAL: invalid args passed*/
/* a Key structure for search rules list */

struct util_state_match_tag
{
	util_state_number_t state_num;	    /* a matching state */
	uint8_t byte;			    /* a matching trigger */
};


const util_state_rule_t *
utility_state_rule_find(util_state_match_t *needle, util_state_rule_t *haystack, ssize_t height);


/* Allocate a new table of state transition rules of size 'entries'
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/
util_state_table_t*
utility_state_table_open(
	util_state_counter_t entries);

/* close a table of state transition rules obtained from a previous call to utility_state_table_open()
 * 	or utility_state_table_load()
 * Returns -1 on error setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/

int
utility_state_table_close(
	util_state_table_t *pstatetab);

/* Insert a new rule into a table of state transition rules
 *     obtained from a previous call to utility_state_table_alloc()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * ENOSPC: no space to store new rule
 * EINVAL: invalid args passed*/
int
utility_state_table_insert(
        util_state_table_t *pstatetab,
        util_state_number_t state_num,
        uint8_t byte,
        util_state_action_t action,
        util_state_number_t next_state);

/* Obtain the state number of the first rule in a table of state transition rules
 *     obtained from a previous call to utility_state_table_alloc()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * EEXIST: no entries in table
 * EINVAL: invalid args passed*/
int
utility_state_table_first_state(
	util_state_table_t *pstatetab,
	 util_state_number_t *pfirst);

/* Find an existing rule in a table of state transition rules
 *     obtained from a previous call to utility_state_table_alloc()
 *
 * Returns NULL on error setting errno
 * Returns a const pointer to a state transition rule on success
 * EEXIST: no entries in table
 * ENOENT: no matching entries found in table
 * EINVAL: invalid args passed*/
const util_state_rule_t *
utility_state_table_find(
	util_state_table_t *pstatetab,
	 util_state_number_t state,
	 uint8_t byte);

/* Write a human readable description of all states in the state table to stderr
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_table_dump(
	util_state_table_t *pstatetab);

/* Allocate a table of state transition rules loaded from
 * 	a text file
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * EIO:    error during reading/writing
 * ENOENT: no matching entries found in table
 * EINVAL: invalid args passed*/
int
utility_state_table_load(
	const char *fname,
	util_state_table_t **ppstatetab,
        util_state_number_t *pfirst);


/* Allocate a new table of size 'entries' using 'plist' obtained from a previous call to utility_state_rule_alloc_list()
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc() or utility_state_table_alloc()*/
util_state_table_t *
utility_state_table_from_list(
	util_state_rule_t *plist,
	util_state_counter_t entries);

/* Allocate a new table of size 'entries' using 'plist' obtained from a previous call to utility_state_rule_alloc_list()
 * this table will use the built in rules for csv parsing
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc() or utility_state_table_alloc()*/
util_state_table_t *
utility_csv_rules_table(
        util_state_number_t *pfirst);

/* Obtain the number of used elements in a state table
 * Returns -1 on error setting errno
 * Returns 0 on success modifying pdst
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as
 *
 * */
int
utility_state_table_length(
        util_state_counter_t *pdst,
        util_state_table_t *pstatetab
);

int
utility_state_load_rule(
        util_state_rule_t *pstate,
         util_state_number_t state_num,
                uint8_t byte,
                util_state_action_t action,
                util_state_number_t next_state
);

#if defined( __cplusplus)
    }
#endif

#endif /* ndef UTILITY_STATE_H */
