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

#define MAX_STATE_LEN 63

    /* A Table of state rules */
    struct util_state_table_tag
    {
        util_state_rule_t *plist;		    /* a list of rules */
        util_state_counter_t entries;	    /* the length of the list */
        util_state_counter_t used;	    /* the number of rules which are non-empty */
    };
#if defined (VERBOSE_STATES_DEBUG)
    void utility_state_table_trace_return_int(int ret, const char *msg, ...);
    void utility_state_table_trace(util_state_table_t *ptr,const char *msg);
    void utility_state_table_trace_return_ptr(void *ret, const char *msg, ...);


#define utility_state_table_trace(pstatetab,msg) \
    do { \
        int old_errno = errno; \
        utility_fname_printf("Trace.log",( \
                    "%s:%04d " \
                    msg    \
                    " [%p]:\n" \
                    "pstatetab->entries  [%d]\n" \
                    "pstatetab->used     [%d]\n" \
                    "pstatetab->plist    [%p]\n\n" \
                    ), \
                __func__,__LINE__,  \
                (pstatetab),(pstatetab)->entries,(pstatetab)->used,(pstatetab)->plist \
                ); \
        errno = old_errno; \
    }while(0)

#define utility_state_table_trace_return_ptr(ret,msg,args) \
    do { \
        utility_state_table_trace(msg,args); \
        utility_wrap_return_ptr(ret);\
    }while(0)

#define utility_state_table_trace_return_int(ret,msg,args) \
    do { \
        utility_state_table_trace(msg,args); \
        utility_wrap_return_int(ret);\
    }while(0)

#else
    #undef utility_trace
    #define utility_trace(a,b)
    #define utility_state_table_trace(pstatetab,msg)
    #define utility_state_table_trace_return_ptr(ret,msg,args) return (ret)
    #define utility_state_table_trace_return_int(ret,msg,args) return (ret)
#endif /* ! defined VERBOSE_STATES_DEBUG */

    /* number of states required to bootstrap state (csv) loading */
#define LOADER_LEN 70
    /* Allocate a new table of state transition rules of size 'entries'
     *
     * Returns NULL on error setting errno
     * Returns valid pointer on success
     * EINVAL: invalid args passed
     * function may fail and set errno for same reasons as malloc()*/
    static util_state_table_t*
        utility_state_table_alloc(
                util_state_counter_t entries
                );

    /* Allocate rules for state transition table rules of size 'entries'
     *
     * Returns NULL on error setting errno
     * Returns valid pointer on success
     * EINVAL: invalid args passed
     * function may fail and set errno for same reasons as malloc()*/
    static util_state_table_t *
        utility_state_table_alloc_rules(
                util_state_table_t *pstatetab
                );

    /* close a table of state transition rules obtained from a previous call to utility_state_table_open()
     * 	or utility_state_table_load()
     * Returns -1 on error setting errno
     * Returns 0 on success
     * EINVAL: invalid args passed*/

    static int
        utility_state_table_free(
                util_state_table_t *pstatetab
                );



    /* Open a new table of state transition rules of size 'entries'
     *
     * Returns NULL on error setting errno
     * Returns valid pointer on success
     * EINVAL: invalid args passed
     * function may fail and set errno for same reasons as malloc()*/
    util_state_table_t*
        utility_state_table_open(
                util_state_counter_t entries
                )
        {
            util_state_table_t *pstatetab;
            /* 1) allocate the container */
            pstatetab = utility_state_table_alloc(entries);
            if(!pstatetab)
                utility_trace_return_ptr(NULL,"Problem, opening state table [%p] failed",pstatetab);

            /* 2) allocate the rules */
            if(!(utility_state_table_alloc_rules(pstatetab)))
            {
                /* 3) allocating rules for table failed, so cleanup and error out */
                free(pstatetab);
                pstatetab = NULL;
                utility_trace_return_ptr(NULL,"Problem, opening state table, state table zeroed [%p], failed",pstatetab);
            }

            /* 4) and we're out of here, with a fresh table */
            utility_state_table_trace_return_ptr(pstatetab,"Opened statetable [%p]",pstatetab);
        }

    /* Allocate a new table of state transition rules of size 'entries'
     *
     * Returns NULL on error setting errno
     * Returns valid pointer on success
     * EINVAL: invalid args passed
     * function may fail and set errno for same reasons as malloc()*/
    static util_state_table_t*
        utility_state_table_alloc(
                util_state_counter_t entries
                )
        {
            util_state_table_t *pstatetab = NULL;

            /* 1) allocate an empty container */
            errno = EINVAL;
            if(entries <= 0  || !(pstatetab = calloc(1,sizeof(util_state_table_t))))
                utility_trace_return_ptr(NULL,"Problem, allocating state table %s","failed");

            /* 2) set members to sane defaults */
            pstatetab->entries = entries;

            /* 3) and we're out of here, with the rest of the work happening in utility_state_table_open() */
            utility_state_table_trace_return_ptr(pstatetab,pstatetab,"Allocated state table");
        }


    /* Allocate rules for state transition table rules of size 'entries'
     *
     * Returns NULL on error setting errno
     * Returns valid pointer on success
     * EINVAL: invalid args passed
     * function may fail and set errno for same reasons as malloc()*/
    static util_state_table_t *
        utility_state_table_alloc_rules(util_state_table_t *pstatetab)
        {
            errno = EINVAL;
            if(!pstatetab)
                utility_trace_return_ptr(NULL,"Problem, allocating rules for state table aborted as table is [%p]",NULL);

            utility_state_table_trace(pstatetab,"opening state table, creating rules");
            if(!(pstatetab->plist = utility_state_rule_alloc_list(pstatetab->entries)))
                /* 2) allocating rules for table failed, so error out */
                utility_state_table_trace_return_ptr(NULL,pstatetab,"Problem, creating rules failed");

            /* 3) split if we've allocated rules for table */
            utility_state_table_trace_return_ptr(pstatetab,pstatetab,"Opened state table");
        }


    /* Free a table of state transition rules obtained from a previous call to utility_state_table_alloc()
     * 	or utility_state_table_load()
     * Returns -1 on error setting errno
     * Returns 0 on success
     * EINVAL: invalid args passed*/

    int
        utility_state_table_close(
                util_state_table_t *pstatetab)
        {
            return utility_state_table_free(pstatetab);
        }


    /* Free a table of state transition rules obtained from a previous call to utility_state_table_alloc()
     * 	or utility_state_table_load()
     * Returns -1 on error setting errno
     * Returns 0 on success
     * EINVAL: invalid args passed*/

    static int
        utility_state_table_free(
                util_state_table_t *pstatetab)
        {

            errno = EINVAL;

            if(!pstatetab)
                return util_eError;

            utility_state_rule_free(pstatetab->plist);
            free(pstatetab);
            return util_eOk;
        }

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
                util_state_number_t next_state
                )
        {
            errno = EINVAL;
            utility_trace("opening state table [%p] for insert",pstatetab);
            if(!pstatetab)
                return util_eError;

            errno = ENOSPC;
            if( (pstatetab->used+1 < pstatetab->entries))
            {
                util_state_rule_t *pstate = utility_state_rule_list_item(pstatetab->plist,pstatetab->used);
                if(util_eOk == utility_state_load_rule(pstate,state_num,byte,action,next_state))
                {
                    ++pstatetab->used;
                    return util_eOk;
                }
            }
            return util_eError;
        }

    /* Obtain the state number of the first rule in a table of state transition rules
     *     obtained from a previous call to utility_state_table_alloc()
     *
     * Returns -1 on error setting errno
     * Returns 0 on success
     * EEXIST: no entries in table
     * EINVAL: invalid args passed*/
    int
        utility_state_table_first_state(util_state_table_t *pstatetab, util_state_number_t *pfirst)
        {
            errno = EINVAL;
            utility_trace("opening state table [%p] for finding first entry",pstatetab);
            if(!pstatetab || !pfirst)
                return util_eError;

            /* no entries so no point going futher */
            errno = EEXIST;
            if(!pstatetab->used)
            {
                utility_trace("Problem, state table [%p] is empty so finding first entry failed",pstatetab);
                return util_eError;
            }
            else
            {
                util_state_rule_t *pstate;
                pstate = utility_state_rule_list_item(pstatetab->plist,0);
                if(!pstate)
                    return util_eError;

                return utility_state_rule_get_state_num(pstate,pfirst) ;
            }
            return util_eError;
        }

    /* Find an existing rule in a table of state transition rules
     *     obtained from a previous call to utility_state_table_alloc()
     *
     * Returns NULL on error setting errno
     * Returns a const pointer to a state transition rule on success
     * EEXIST: no entries in table
     * ENOENT: no matching entries found in table
     * EINVAL: invalid args passed*/
    const util_state_rule_t *
        utility_state_table_find(util_state_table_t *pstatetab, util_state_number_t state, uint8_t byte)
        {
            util_state_match_t match;

            errno = EINVAL;
            if(!pstatetab)
                utility_wrap_return_ptr(NULL);

            /* no entries so no point going futher */
            errno = EEXIST;
            if(!pstatetab->used)
                utility_wrap_return_ptr(NULL);

            match.state_num = state;
            match.byte 	= byte;

            return utility_state_rule_find( &match,pstatetab->plist, pstatetab->used);
        }

    /* Write a human readable description of all states in the state table to stderr
     *
     * Returns -1 on failure setting errno
     * Returns 0 on success
     * EINVAL: invalid args passed*/
    int
        utility_state_table_dump(util_state_table_t *pstatetab)
        {
            util_state_counter_t idx;
            char *msg;
            errno = EINVAL;
            utility_trace("Opening state table [%p] for dumping",pstatetab);
            if(!pstatetab)
                return util_eError;

            /* no entries so no point going futher */
            if(!pstatetab->used)
                return util_eOk;

            utility_state_table_trace(pstatetab,"Dumping state table");
            for(idx=0; idx < pstatetab->used-1; ++idx)
            {
                msg = utility_state_rule_str(
                        utility_state_rule_list_item(pstatetab->plist,idx),'a');
                if(msg)
                {
                    utility_trace("Dumping state %s",msg);
                    free(msg);
                }
            }
            utility_state_table_trace(pstatetab,"Dumping state table completed ");
            return util_eOk;
        }


    /* Allocate a new table of size 'entries' using 'plist' obtained from a previous call to utility_state_rule_alloc_list()
     * Returns NULL on error setting errno
     * Returns valid pointer on success
     * EINVAL: invalid args passed
     * function may fail and set errno for same reasons as malloc() or utility_state_table_alloc()*/
    util_state_table_t *
        utility_state_table_from_list(util_state_rule_t *plist,util_state_counter_t entries)
        {
            util_state_table_t *pstatetab;

            if(!plist && entries > 0)
                return utility_state_table_alloc(entries);

            if(plist && entries > 0 && (pstatetab = malloc(sizeof(util_state_table_t))) )
            {
                pstatetab->plist   = plist;
                pstatetab->entries = pstatetab->used = entries;
                return pstatetab;
            }
            utility_wrap_return_ptr(NULL);
        }

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
                util_state_table_t *pstatetab)
        {
            errno = EINVAL;
            utility_trace("opening state table [%p] for storing length",pstatetab);
            if(!pstatetab || !pdst)
                return util_eError;

            utility_state_table_trace(pstatetab,"storing state table length");
            *pdst = pstatetab->used;
            return util_eOk;
        }

    static int match_start(util_state_rule_t *pstate, int *rules);
    static int match_comment(util_state_rule_t *pstate, int *rules);
    static int match_multiline(util_state_rule_t *pstate, int *rules);
    static int match_identifier(util_state_rule_t *pstate, int *rules);

    static int match_value(util_state_rule_t *pstate, int *rules);
    static int match_hex_prefix(util_state_rule_t *pstate, int *rules);
    static int match_hex_value(util_state_rule_t *pstate, int *rules);
    static int match_decimal_value(util_state_rule_t *pstate, int *rules);
    static int match_dquote_value(util_state_rule_t *pstate, int *rules);
    static int match_escaped_dquote(util_state_rule_t *pstate, int *rules);
    static int match_delim(util_state_rule_t *pstate, int *rules);
    static int match_squote_value(util_state_rule_t *pstate, int *rules);
    static int match_escaped_squote(util_state_rule_t *pstate, int *rules);

    /* Allocate a new table of size 'entries' using 'plist' obtained from a previous call to utility_state_rule_alloc_list()
     * this table will use the built in rules for csv parsing
     * Returns NULL on error setting errno
     * Returns valid pointer on success
     * EINVAL: invalid args passed
     * function may fail and set errno for same reasons as malloc() or utility_state_table_alloc()*/
    util_state_table_t *
        utility_csv_rules_table(
                util_state_number_t *pfirst)
        {
            util_state_table_t *table;
            util_state_rule_t *plist;
            int off;
            int nrules;

            plist = utility_state_rule_alloc_list(LOADER_LEN);

            return ( pfirst &&  plist &&
                    /* bootstrap loading log external states from a csv file */
                    (util_eOk == match_start( utility_state_rule_list_item(plist,off=0),&nrules)) &&
                    (util_eOk == match_comment( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_multiline( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_identifier( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_value( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_hex_prefix( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_hex_value( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_decimal_value( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_dquote_value( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_escaped_dquote( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_delim( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_squote_value( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    (util_eOk == match_escaped_squote( utility_state_rule_list_item(plist,off+=nrules),&nrules)) &&
                    /* make sure we have a starting state otherwise we can do nothing */
                    (util_eOk == utility_state_table_first_state((table = utility_state_table_from_list(plist,off+nrules)),pfirst)) ) ? table : NULL ;
        }

#define START               100
#define COMMENT             110
#define MULTILINE           111
#define IDENTIFIER          130
#define DOUBLE_QUOTE_VAL    150
#define ESCAPED_DOUBLE_QUOTE (DOUBLE_QUOTE_VAL+1)
#define DELIM               152
#define SINGLE_QUOTE_VAL    160
#define ESCAPED_SINGLE_QUOTE (SINGLE_QUOTE_VAL+1 )
#define HEX_PREFIX          141
#define HEX_VAL             142
#define VALUE               200
#define DECIMAL_VAL         143

    static int
        match_start(util_state_rule_t *pstate, int *rules)
        {
            /* START */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),   START) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),     '#') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),      TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),  COMMENT) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),   START) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),     DETECT_ALNUM) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),      TRIGGER_EXCHANGE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),  VALUE) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,2),   START) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,2),     DETECT_NEWLINE) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,2),      TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,2),  START) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,3),   START) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,3),     DETECT_WSPACE) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,3),      TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,3),  START) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,4),START) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,4),DETECT_DQUOTE) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,4),TRIGGER_EXCHANGE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,4),VALUE) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,5),START) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,5),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,5),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,5),START) )
                        )
                        {
                            *rules = 6;
                            return util_eOk;
                        }
            return util_eError;
        }

    static int
        match_comment(util_state_rule_t *pstate, int *rules)
        {
            /* COMMENT */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),COMMENT) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),DETECT_NEWLINE) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_EXCHANGE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),START) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),COMMENT) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),DETECT_BSLASH) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),MULTILINE) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,2),COMMENT) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,2),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,2),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,2),COMMENT) ) )
            {
                *rules = 3;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_multiline(util_state_rule_t *pstate, int *rules)
        {
            /* MULTILINE */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),MULTILINE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),DETECT_NEWLINE) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),START) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),MULTILINE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),MULTILINE) ))
            {
                *rules = 2;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_identifier(util_state_rule_t *pstate, int *rules)
        {
            /* IDENTIFIER */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),IDENTIFIER) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),DETECT_ALNUM) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),IDENTIFIER) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),IDENTIFIER) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),'_') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),IDENTIFIER) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,2),IDENTIFIER) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,2),DETECT_CARRIAGE_RETURN) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,2),TRIGGER_EXCHANGE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,2),START) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,3),IDENTIFIER) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,3),DETECT_NEWLINE) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,3),TRIGGER_PRINT) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,3),START) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,4),IDENTIFIER) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,4),'(') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,4),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,4),IDENTIFIER) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,5),IDENTIFIER) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,5),'{') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,5),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,5),IDENTIFIER)
                        &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,6),IDENTIFIER) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,6),',') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,6),TRIGGER_PRINT) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,6),START) &&

                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,7),IDENTIFIER) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,7),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,7),TRIGGER_EXCHANGE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,7),START) ) )
                        {
                            *rules = 8;
                            return util_eOk;
                        }
            return util_eError;
        }

    static int
        match_value(util_state_rule_t *pstate, int *rules)
        {
            /* VALUE */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),VALUE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),'"') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),DOUBLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),VALUE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),'\'') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),SINGLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,2),VALUE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,2),'0') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,2),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,2),HEX_PREFIX) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,3),VALUE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,3),DETECT_DIGIT) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,3),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,3),DECIMAL_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,4),VALUE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,4),DETECT_BLANK) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,4),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,4),VALUE) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,5),VALUE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,5),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,5),TRIGGER_ERROR) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,5),START) ))
                        {
                            *rules = 6;
                            return util_eOk;
                        }
            return util_eError;
        }

    static int
        match_hex_prefix(util_state_rule_t *pstate, int *rules)
        {
            /* HEX_PREFIX */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),HEX_PREFIX) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),'X') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),HEX_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),HEX_PREFIX) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),'x') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),HEX_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,2),HEX_PREFIX) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,2),DETECT_DIGIT) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,2),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,2),DOUBLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,3),HEX_PREFIX) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,3),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,3),TRIGGER_EXCHANGE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,3),IDENTIFIER) ))
            {
                *rules = 4;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_hex_value(util_state_rule_t *pstate, int *rules)
        {
            /* HEX VALUE */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),HEX_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),DETECT_XDIGIT) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),HEX_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),HEX_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_PRINT) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),START) ) )
            {
                *rules = 2;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_decimal_value(util_state_rule_t *pstate, int *rules)
        {
            /* DECIMAL INTEGER VALUE */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),DECIMAL_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),DETECT_DIGIT) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),DECIMAL_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),DECIMAL_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_PRINT) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),START) ))
            {
                *rules = 2;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_dquote_value(util_state_rule_t *pstate, int *rules)
        {
            /* DOUBLE QUOTED VALUE */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),DOUBLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),'"') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_FLUSH) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),START) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),DOUBLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),'\\') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),ESCAPED_DOUBLE_QUOTE) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,2),DOUBLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,2),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,2),TRIGGER_HOLD) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,2),DOUBLE_QUOTE_VAL) ) )
            {
                *rules = 3;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_escaped_dquote(util_state_rule_t *pstate, int *rules)
        {
            /* ESCAPED_DQUOTE */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),ESCAPED_DOUBLE_QUOTE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),'\n') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),DOUBLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),ESCAPED_DOUBLE_QUOTE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_ESCAPE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),DOUBLE_QUOTE_VAL) ) )
            {
                *rules = 2;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_delim(util_state_rule_t *pstate, int *rules)
        {
            errno = EINVAL;
            /* DELIM */
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),DELIM) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),',') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_PRINT) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),START) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),DELIM) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_ERROR) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),START) ) )
            {
                *rules = 2;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_squote_value(util_state_rule_t *pstate, int *rules)
        {
            /* SINGLE QUOTED VALUE */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),SINGLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),'\'') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_ESCAPE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),START) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),SINGLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),'\\') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),ESCAPED_SINGLE_QUOTE) ) )
            {
                *rules = 2;
                return util_eOk;
            }
            return util_eError;
        }

    static int
        match_escaped_squote(util_state_rule_t *pstate, int *rules)
        {
            /* ESCAPED_SQUOTE */
            errno = EINVAL;
            if(rules && pstate && (
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),ESCAPED_SINGLE_QUOTE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),'\n') &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),TRIGGER_DELETE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),SINGLE_QUOTE_VAL) &&
                        util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,1),ESCAPED_SINGLE_QUOTE) &&
                        util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,1),DETECT_ANY) &&
                        util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,1),TRIGGER_ESCAPE) &&
                        util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,1),SINGLE_QUOTE_VAL) ) )
            {
                *rules = 2;
                return util_eOk;
            }
            return util_eError;
        }

#if defined( __cplusplus)
}
#endif
