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
#include "libUtility.h"

#if defined( __cplusplus)
extern "C" {
#endif

/* A State transition rule */
struct util_state_rule_tag
{
	util_state_number_t state_num;	    /* the state that marks the start of this rule */
	uint8_t byte;			    /* the trigger for selecting this rule */
	util_state_action_t action;	    /* the action taken when transitioning out of this rule*/
	util_state_number_t next_state;	    /* the state this rules transitions into */
};
#if defined (VERBOSE_STATES_DEBUG)
void  utility_state_trace(util_state_rule_t *pstate, const char *msg) ;
#define utility_state_trace(pstate,msg) \
    do { \
        int old_errno = errno; \
        utility_fname_printf("Trace.log",( \
            "%s:%04d " \
             msg    \
             " [%p]:\n" \
            "pstate->state_num  [%d]\n" \
            "pstate->byte       [%d]\n" \
            "pstate->action     [%d]\n" \
            "pstate->next_state [%d]\n\n" \
             ), \
            __func__,__LINE__,  \
            (pstate),(pstate)->state_num,(pstate)->byte, (pstate)->action,(pstate)->next_state \
        ); \
        errno = old_errno; \
    }while(0)

#else
    #undef utility_trace
    #define utility_trace(a,b)
    #define utility_state_trace(pstate,msg)
#endif /* ! defined VERBOSE_STATES_DEBUG */

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
	util_state_number_t next_state)    /* the state this rules transitions into */
{
util_state_rule_t *pstate;
	errno = EINVAL;

	if( !utility_state_rule_action_is_valid(action) || !(pstate = utility_state_rule_alloc_list(1)))
		utility_wrap_return_ptr(NULL);

	pstate->state_num = state_num;
	pstate->byte = byte;
	pstate->action = action;
	pstate->next_state = next_state;
        utility_state_trace(pstate,"Allocated state rule");
	return pstate;
}

/* Allocate a list of size 'entries' of  new state transition rule
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/
util_state_rule_t *
utility_state_rule_alloc_list(
        util_state_counter_t entries
        )
{
    return calloc(entries,sizeof(util_state_rule_t));
}

/* Free a state transition rules obtained from a previous call to utility_state_rule_alloc()
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_rule_free(
	util_state_rule_t *pstate)
{
	errno = EINVAL;
	if(!pstate)
		return util_eError;

        utility_state_trace(pstate,"Freeing state rule");
	free(pstate);
	return util_eOk;
}

/* Write a human readable description of a state transition rule to stderr
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_rule_dump_err(
	const util_state_rule_t *pstate)
{
    return utility_state_rule_fdump(stderr,pstate);
}

/* Write a human readable description of a state transition rule to logfile
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_rule_dump(
        const char *logfile,
        const char *mode,
        const util_state_rule_t *pstate)
{
    FILE *fp ;
    errno = EINVAL;
    if(!logfile || !mode || (*mode!='a' && *mode != 'w' ) || !(fp = fopen(logfile,mode)))
        return util_eError;

    utility_state_rule_fdump(fp,pstate);
    fclose(fp);
    return util_eOk;
}

/* Write a human readable description of a state transition rule to logfile
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_state_rule_fdump(
	FILE *fp,
	 const util_state_rule_t *pstate)
{
    char *message;
    errno = EINVAL;

   /* allocate friendly names  for values */
    if(!pstate || !fp ||  !(message = utility_state_rule_str(pstate,DETECT_ANY)))
        return util_eError;

    /* finally do the write and clean up the message */
    fprintf(fp,"%s\n",message);
    fflush(fp);
    free(message);
    return util_eOk;
}

/* Write a human readable description of a state transition rule to malloc'd buffer
 *
 * Returns NULL on failure setting errno
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_state_rule_str(
        const util_state_rule_t *pstate,
        uint8_t ch)
{
    char * trigger, *action, *match, *message=NULL;
    util_state_number_t next=0, current=0;
    uint8_t  trigger_ch =0;
    util_state_action_t action_num =0;

    errno = EINVAL;
    if(!pstate || util_eError == utility_state_rule_get_trigger(pstate,&trigger_ch) ||
            util_eError == utility_state_rule_get_action(pstate,&action_num)       ||
            util_eError == utility_state_rule_get_next_state_num(pstate,&next)     ||
            util_eError == utility_state_rule_get_state_num(pstate,&current) )
        utility_wrap_return_ptr(NULL);

    /* allocate friendly names  for values */
    match = utility_state_rule_describe_match(ch);
    action = utility_state_rule_describe_action(action_num);
    trigger = utility_state_rule_describe_trigger(trigger_ch);

    if(trigger && action && match)
        message = utility_sprintf("%s state=%-5d next=%-5d %s %s\n",match, current, next, trigger, action);

    /* release friendly names  & park the pointers */
    free(action);   action  = NULL;
    free(trigger);  trigger = NULL;
    free(match);    match   = NULL;

    return message;
}

/* Write a human readable description of a state transition rule trigger to malloc'd buffer
 *
 * Returns NULL on failure setting errno
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_state_rule_describe_trigger(uint8_t value)
{
    static const uint8_t trigger_values[] = {
        '\n',
        '\r',
        '\'',
        '\t',
        DETECT_ANY,
        DETECT_DIGIT,
        DETECT_XDIGIT,
        DETECT_ALPHA,
        DETECT_ALNUM,
        DETECT_WSPACE,
        DETECT_BLANK
    };

    static const char *trigger_names[] = {
        "'\\n'       \t",
        "'\\r'       \t",
        "'\\''       \t",
        "'\\t'       \t",
        "'DETECT_ANY'\t",
        "'DETECT_DIGIT'\t",
        "'DETECT_XDIGIT'\t",
        "'DETECT_ALPHA'\t",
        "'DETECT_ALNUM'\t",
        "'DETECT_WSPACE'\t",
        "'DETECT_BLANK'\t"
    };

    /* search for symbolic name of value */
    return utility_find_name_by_u8_str(
            "trigger",
            value,
            trigger_names,
            trigger_values,
            ( sizeof( trigger_names)/ sizeof( trigger_names[0] ) )
            );
}

/* Write a human readable description of a byte to malloc'd buffer
 *
 * Returns NULL on failure setting errno
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_state_rule_describe_match(uint8_t value)
{
    static const char *match_names[] = {
        "\\n",
        "\\r",
        "\\'",
        "\\t",
        "\\\"",
        "*'",
        "\\",
        "/",
    };

    static const uint8_t match_values[] = {
        DETECT_NEWLINE,
        DETECT_CARRIAGE_RETURN,
        DETECT_SQUOTE,
        '\t',
        DETECT_DQUOTE,
        DETECT_STAR,
        DETECT_BSLASH,
        DETECT_SLASH,
    };

    /* search for symbolic name of value */
    /* return the literal value unless we find a symbol */
    return utility_find_name_by_u8_str("match",value,
            match_names,
            match_values,
            ( sizeof( match_names)/ sizeof( match_names[0] ) ) );
}

/* Write a human readable description of a action to malloc'd buffer
 *
 * Returns NULL on failure setting errno
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_state_rule_describe_action(util_state_action_t value)
{
    static const char *action_names[] = {
        "TRIGGER_PUSH_HOLD",
        "TRIGGER_PUSH_BACK",
        "TRIGGER_POP_HOLD",
        "TRIGGER_EXCHANGE",
        "TRIGGER_HOLD",
        "TRIGGER_FLUSH_N",
        "TRIGGER_FLUSH",
        "TRIGGER_FLUSH_HOLD",
        "TRIGGER_PRINT",
        "TRIGGER_ESCAPE",
        "TRIGGER_SPACE",
        "TRIGGER_NEWLINE",
        "TRIGGER_ERROR",
        "TRIGGER_UNGET",
        "TRIGGER_DELETE",
        "TRIGGER_SLASH"
    };

    static const util_state_action_t action_values[] = {
        TRIGGER_PUSH_HOLD,
        TRIGGER_PUSH_BACK,
        TRIGGER_POP_HOLD,
        TRIGGER_EXCHANGE,
        TRIGGER_HOLD,
        TRIGGER_FLUSH_N,
        TRIGGER_FLUSH,
        TRIGGER_FLUSH_HOLD,
        TRIGGER_PRINT,
        TRIGGER_ESCAPE,
        TRIGGER_SPACE,
        TRIGGER_NEWLINE,
        TRIGGER_ERROR,
        TRIGGER_UNGET,
        TRIGGER_DELETE,
        TRIGGER_SLASH
    };

    /* search for symbolic name of value */
    /* return the literal value unless we find a symbol */
    return utility_find_name_by_u16_str("action",value,
            action_names,
            action_values,
            ( sizeof( action_names)/ sizeof( action_names[0] ) ) );

}

/* Obtain trigger byte for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pch
 * EINVAL: invalid args passed*/
int
utility_state_rule_get_trigger(
	const util_state_rule_t *ptr,
	uint8_t *pch )
{
	errno = EINVAL;
	if(!ptr || !pch)
		return util_eError;

	*pch = ptr->byte;
	return util_eOk;
}

/* Specify state number for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pch
 * EINVAL: invalid args passed*/

int
utility_state_rule_set_state_num(
        util_state_rule_t *ptr,
        const util_state_number_t state_num  )
{
    errno = EINVAL;
    if(!ptr)
        return util_eError;

    ptr->state_num  = state_num ;
    return util_eOk;
}


/* Specify index of next state rule for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pch
 * EINVAL: invalid args passed*/

int
utility_state_rule_set_next_state(
        util_state_rule_t *ptr,
        const util_state_number_t next_state  )
{
    errno = EINVAL;
    if(!ptr)
        return util_eError;

    ptr->next_state  = next_state ;
    return util_eOk;
}


/* Specify parser action for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pch
 * EINVAL: invalid args passed*/

int
utility_state_rule_set_action(
        util_state_rule_t *ptr,
        const util_state_action_t action  )
{
    errno = EINVAL;
    if(!ptr || !utility_state_rule_action_is_valid(action))
        return util_eError;

    ptr->action  = action ;
    return util_eOk;
}


/* Specify byte trigger for transition for  state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pch
 * EINVAL: invalid args passed*/

int
utility_state_rule_set_trigger(
        util_state_rule_t *ptr,
        const uint8_t trigger  )
{
    errno = EINVAL;
    if(!ptr)
        return util_eError;

    ptr->byte  = trigger ;
    return util_eOk;
}

/* Obtain Action to perform when transitioning from one state transition rule to another
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing paction
 * EINVAL: invalid args passed*/
int
utility_state_rule_get_action(
	const util_state_rule_t *ptr,
	util_state_action_t *paction )
{
	errno = EINVAL;
	if(!ptr || !paction)
		return util_eError;

	*paction = ptr->action;
	return util_eOk;
}

/* Obtain the number of the next state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pstate_num
 * EINVAL: invalid args passed*/
int
utility_state_rule_get_next_state_num(
	const util_state_rule_t *ptr,
	util_state_number_t *state_num)
{
	errno = EINVAL;
	if(!ptr || !state_num)
		return util_eError;

	*state_num = ptr->next_state;
	return util_eOk;
}
/* Obtain the number of current state transition rule
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success modifing pstate_num
 * EINVAL: invalid args passed*/
int
utility_state_rule_get_state_num(
	const util_state_rule_t *ptr,
	util_state_number_t *pstate_num)
{
	errno = EINVAL;
	if(!ptr || !pstate_num)
		return util_eError;
	*pstate_num = ptr->state_num;
	return util_eOk;
}

/* Determine if 'action' is within accepted range
 * Returns 0 on failure setting errno
 * Returns 1 on success
 * EINVAL: 'action' is out of range*/
int
utility_state_rule_action_is_valid(
	util_state_action_t action)
{
	errno = EINVAL;
	return ( !( (~TRIGGER_UNKNOWN) & action) || TRIGGER_NEWLINE == action || TRIGGER_SLASH == action);
}

/* return an entry in a list of state rules obtained from a previous call to utility_state_rule_alloc_list()
 * if 'action' is within accepted range
 * Returns 0 on failure setting errno
 * Returns 1 on success
 * EINVAL: 'action' is out of range*/
util_state_rule_t *
utility_state_rule_list_item(
        util_state_rule_t *pstate,
        util_state_counter_t offset)
{
    return (pstate) ? &pstate[offset] : NULL;
}

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
        )
{
size_t nelem;
    p2p_set_errno(util_eInvalidArguments);
    if(!plist || !fp)
        return util_eError;

    nelem = fwrite( utility_state_rule_list_item(plist,offset), sizeof(util_state_rule_t), max, fp);
    if(nelem != max)
        return util_eError;

    return util_eOk;
}

const util_state_rule_t *
utility_state_rule_find(util_state_match_t *needle, util_state_rule_t *haystack, ssize_t height)
{
    errno = EINVAL;

    if(!needle || !haystack )
        utility_wrap_return_ptr(NULL);
    
    else
    {

        util_state_rule_t *it=haystack, *end = NULL;
        uint8_t byte                =  needle->byte;
        util_state_number_t state   = needle->state_num;

        for(end = (haystack + height); it != end; ++it)
        {
            if(state == it->state_num)
            {
                if(byte == it->byte)
                    return it;

                switch(it->byte)
                {
                    case DETECT_XDIGIT :
                        if(isxdigit(byte))
                            return it;
                        continue;

                    case DETECT_DIGIT  : 
                        if(isdigit(byte))
                            return it;
                        continue;

                    case DETECT_ALPHA  : 
                        if(isalpha(byte))
                            return it;
                        continue;


                    case DETECT_ALNUM  : 
                        if(isalnum(byte))
                            return it;
                        continue;

                    case DETECT_WSPACE :
                        if(isspace(byte))
                            return it;
                        continue;

                    case DETECT_BLANK  :
                        if(isblank(byte))
                            return it;
                        continue;

                    case DETECT_ANY :
                        return it;
                }
            }
        }
    }
    errno = ENOENT;
    return NULL;

#if(0)
    /* hotspot */
    ret = utility_for_each_in_list_with_state(haystack,
            sizeof(util_state_rule_t),
            height,
            needle,
            utility_state_rule_is_match);
    return (ret >= 0) ? utility_state_rule_list_item(haystack,ret) : NULL;
#endif
}

int
utility_state_load_rule(
        util_state_rule_t *pstate,
         util_state_number_t state_num,
                uint8_t byte,
                util_state_action_t action,
                util_state_number_t next_state
)
{
    utility_trace("Setting up rule [%p] from input",pstate);
    if(pstate && (
                util_eOk == utility_state_rule_set_state_num( utility_state_rule_list_item(pstate,0),state_num) &&
                util_eOk == utility_state_rule_set_trigger( utility_state_rule_list_item(pstate,0),byte) &&
                util_eOk == utility_state_rule_set_action( utility_state_rule_list_item(pstate,0),action) &&
                util_eOk == utility_state_rule_set_next_state( utility_state_rule_list_item(pstate,0),next_state)) )
    {
        utility_state_trace(pstate,"Setting up rule from input, succeded");
        return  util_eOk;
    }
    return  util_eError;
}
#if defined( __cplusplus)
    }
#endif


