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
#ifndef LIB_UTILITY_H
#define LIB_UTILITY_H

#if defined( __cplusplus)
extern "C" {
#endif

/* feature test macros */
#include "config.h"

#if !defined (LIB_UTILITY_VERSION)
/* string version */
#define LIB_UTILITY_VERSION_STRING        PACKAGE_VERSION
#define LIB_UTILITY_MAKE_VERSION_NUMBER(major,minor,micro)(  \
                ((major) * 1000000)   + \
                ((minor) * 1000)      + \
                ((micro)) )
  /* (major * 1_000_000) +  (minor * 1_000) + (micro)  is same method sqlite uses */
#define LIB_UTILITY_VERSION_NUMBER (LIB_UTILITY_MAKE_VERSION_NUMBER(0,0,1))
#endif

#include "Utility_includes.h"
#include "Utility_types.h"
#include "Utility_string.h"
#include "Utility_errors.h"
#include "Utility_find.h"
#include "Utility_pack.h"
#include "Utility_vector.h"
#include "Utility_s_vector.h"
#include "Utility_p_vector.h"
#include "Utility_sz_vector.h"
#include "Utility_state.h"
#include "Utility_push_buffer.h"
#include "Utility_token.h"
#include "Utility_p2p_comms.h"

void utility_wrap_return_ptr(void *ptrval);

/* debug macro */
#define utility_wrap_return_ptr(ptrval)    \
    do { \
        \
        int old_errno = errno; \
        utility_fname_printf("Trace.log",   \
                ("%s:%04d %s() returned [%p]\n\n"), \
                 __FILE__,__LINE__, \
                 __func__,(ptrval)   \
         ); \
        errno = old_errno; \
         return (ptrval);   \
    }while(0)

#define utility_wrap_return_int(val)    \
    do { \
        \
        int old_errno = errno; \
        utility_fname_printf("Trace.log",   \
                ("%s:%04d %s() returned [%d]\n\n"), \
                 __FILE__,__LINE__, \
                 __func__,(val)   \
         ); \
        errno = old_errno; \
        return (val);   \
    }while(0)


void utility_trace(const char *msg, ...);
#define utility_trace(msg,args) \
    do { \
        int old_errno = errno; \
        utility_fname_printf("Trace.log", ("%s:%04d %s() " msg "\n" ), __FILE__,__LINE__,__func__,args); \
        errno = old_errno; \
    }while(0)


void utility_trace_return_ptr(void *ret, const char *msg, ...);
#define utility_trace_return_ptr(ret,msg,args) \
    do { \
        utility_trace(msg,args); \
        utility_wrap_return_ptr(ret);\
    }while(0)

void utility_trace_return_int(int ret, const char *msg, ...);
#define utility_trace_return_int(ret,msg,args) \
    do { \
        utility_trace(msg,args); \
        utility_wrap_return_int(ret);\
    }while(0)



#define NO_WARN_UNUSED_ARGUMENT(v) ((v) = (v))

/* Resize a chunk of memory obtained by a previous call to malloc()
 * The behaviour is different to stdlib realloc in that 'old' is always freed
 * Null pointers and zero sizes are not supported, use malloc/free directly if that behaviour is
 * ... desired.
 * This means that p = utility_realloc(p,size) is safe while as we all know
 * 		   p = realloc(p,size)      is a leak waiting to happen
 * On success: Returns a pointer to size bytes of uninitialized memory freeing 'old'
 * On Failure: Returns NULL on failure modifiying errno AND freeing old.
 * EINVAL: invalid args passed, 'old' is null or size is '0'
 * Function may fail and set errno for same reason as realloc()*/
void *utility_realloc(void *old,size_t size);

/* Sprintf that allocates enough memory for all passed args
 * Relies on vsnprintf actually being posixly correct,
 * on some systems (linux libc4) vsnprintf is defined as a sprintf wrapper which makes them broken
 * and this function unsafe, if you have one of thoses system, complain to your vendor
 * Returns NULL on error setting errno
 * Returns pointer to allocated copy of 's'  on success, the caller must free this memory
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reason as malloc()*/
char *utility_vsprintf(const char *fmt, va_list ap);

/* utility_vsprintf wrapper that allocates enough memory for argument list
 * Returns NULL on error setting errno
 * Returns pointer to allocated copy of 's'  on success, the caller must free this memory
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reason as utility_vsprintf()*/
char *utility_sprintf(const char *fmt,...);

/* utility_vfprintf wrapper that opens and closes fname for each write
 * Returns NULL on error setting errno
 * Returns pointer to allocated copy of 's'  on success, the caller must free this memory
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reason as utility_vsprintf()/fopen()/fflush()/fclose()*/
int
utility_fname_printf(
        const char *fname,
        const char *fmt,
        ...
);

/* Open a file with logging and filesystem checks on the filename
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as fopen()*/
FILE *
utility_fopen(
        const char *fname,
        const char *mode,
        int should_exist
);

/* Reopen a file with logging and filesystem checks on the filename
 *
 * Returns NULL on error setting errno
 * Returns valid file pointer on success
 *
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as
 * freopen()
 * utility_fopen */
FILE *
utility_freopen(
        const char *fname,
        const char *mode,
        FILE       *old_fp,
        int should_exist
        );


/* Obtains upto len-1 bytes from stdin and nul terminate the result
 * Returns <0 on error setting errno
 * Returns number of bytes read on success,
 * A return value of 0 means EOF was encountered
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as fgets()*/
int
utility_gets(
        char *dest,
        size_t len
        );

/* Obtains upto len-1 bytes from fp and nul terminate the result
 * Returns <0 on error setting errno
 * Returns number of bytes read on success,
 * A return value of 0 means EOF was encountered
 * util_eInvalidArguments: invalid args passed
 * Function may fail and set errno for same reason as fgets()*/
int
utility_fgets(
        char *dest,
        size_t len,
        FILE *fp
        );

/* Allow handling of variadic arguments without cluttering code
 * 'func' is called with a string containing the sprintf'd arguments
 * 'func' is expected to return <0 on failure and >=0 on success
 * Returns <0 on error setting errno
 * Returns return value from 'func' on success,
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reason as utility_sprintf()*/
int utility_output(output_func_t func,const char *fmt,va_list ap);

/* Prints varadic arguments using an output_func and calls utility_gets to get the response
 * 'output_func' is called with a string containing the utility_sprintf'd arguments
 * 'output_func' is expected to return <0 on failure and >=0 on success
 * Returns <0 on error setting errno
 * Returns return value from 'output_func' on success,
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reason as utility_output() or utility_gets*/
int utility_input(output_func_t output_func,char *dest, int len, const char *fmt,...);

/* Printf that uses an output_func to do the write
 * 'output_func' is called with a string containing the utility_sprintf'd arguments
 * 'output_func' is expected to return <0 on failure and >=0 on success
 * Returns <0 on error setting errno
 * Returns return value from 'output_func' on success,
 * EINVAL: invalid args passed
 * Function may fail and set errno for same reason as utility_output()*/
int utility_printf(output_func_t output_func, const char *fmt,...);

/* Call a func for each element in the list
 * returns index of entry that caused func to return non zero
 * returns -1 if the whole list was traversed without func returning non-zero
 * returns -2 setting errno on error
 * EINVAL: invalid arguments */
int utility_for_each_in_list( void *plist,size_t entry_size, size_t nentries,
	       		      int (*fptr)(void *parg));

/* Call a func for each element in the list, passing the current index to 'fptr'
 * returns index of entry that caused func to return non zero
 * returns -1 if the whole list was traversed without func returning non-zero
 * returns -2 setting errno on error
 * EINVAL: invalid arguments */
int utility_for_each_in_list_with_index( void *plist,size_t entry_size, size_t nentries,
			  int (*fptr)(void *parg,size_t index));

/* Call a func for each element in the list, passing 'state' to 'fptr'
 * returns index of entry that caused func to return non zero
 * returns -1 if the whole list was traversed without func returning non-zero
 * returns -2 setting errno on error
 * EINVAL: invalid arguments */
int utility_for_each_in_list_with_state( void *plist,size_t entry_size, size_t nentries,void *state,
			  int (*fptr)(void *parg,void *pstate));


/* Call a func for each element in the list, passing the current index and 'state' to 'fptr'
 * returns index of entry that caused func to return non zero
 * returns -1 if the whole list was traversed without func returning non-zero
 * returns -2 setting errno on error
 * EINVAL: invalid arguments */
int utility_for_each_in_list_with_index_and_state( void *plist,size_t entry_size, size_t nentries,void *state,
			  int (*fptr)(void *parg,size_t index,void *pstate));

/* Reverse a byte string in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * EINVAL: invalid args passed*/
uint8_t *utility_reverse_ui8(uint8_t *s,int len);

/* Reverse a uint16_t array in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * EINVAL: invalid args passed*/
uint16_t *utility_reverse_ui16(uint16_t *s,int len);

/* Reverse a uint32_t array in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * EINVAL: invalid args passed*/
uint32_t *utility_reverse_ui32(uint32_t *s,int len);

/* Reverse a uint64_t array in place
 * Returns NULL on error setting errno
 * Returns 's' on success
 * EINVAL: invalid args passed*/
uint64_t *utility_reverse_ui64(uint64_t *s, int len);

/* Allocate a new stack of pointers of size 'entries'
 *
 * Returns NULL on error setting errno
 * Returns valid pointer on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as malloc()*/
PTR_STACK *utility_pstk_alloc(int16_t entries);

/* Free a stack of pointers obtained from a previous call to utility_pstk_alloc()
 * if 'free_func' is not NULL, then 'free_func' will be called with each entry *once*
 * It is the callers responsibility to make sure this behaviour doesn't result in double free
 * i.e. if this can happen, then pass NULL as the 'free_func' argument after
 * calling utility_pstk_pop() in a loop
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int utility_pstk_free(PTR_STACK *ptr, void (*free_func)(void *));

/* Flush a stack of pointers obtained from a previous call to utility_pstk_alloc()
 * if 'free_func' is not NULL, then 'free_func' will be called with each entry *once*
 * It is the callers responsibility to make sure this behaviour doesn't result in double free
 * i.e. if this can happen, then pass NULL as the 'free_func' argument after
 * calling utility_pstk_pop() in a loop
 *
 * Returns -1 on error setting errno
 * Returns 0 on success
 * EINVAL: invalid args passed*/
int
utility_pstk_flush(
        PTR_STACK *ptr,
        void (*free_func)(void *)
        );

/* Store a pointer
 *
 * Returns -1 on failure setting errno
 * Returns 0 on success
 * ENOMEM: not enough room to store 'pval'
 * EINVAL: invalid args passed*/
int utility_pstk_push(PTR_STACK *ptr, const void *pval);

/* Destructively retrieve the last value pushed
 *
 * Returns -1 on error setting errno
 * Returns 0 on success storing in '*pval'
 * EBADSLT: stack is empty
 * EINVAL: invalid args passed*/
int utility_pstk_pop(PTR_STACK *ptr, void **pval);

/* Non destructively retrieve the last value pushed
 *
 * Returns -1 on error setting errno
 * Returns 0 on success storing in '*pval'
 * EBADSLT: stack is empty
 * EINVAL: invalid args passed*/
int utility_pstk_peek(PTR_STACK *ptr, void **pval);

/* Obtain the size of a file on disk as reported by stat
 * may give inaccurate results in the case of NFS files
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_filesize(
        size_t *dest_sz,
        const char *fname
        );

/* Flush stdio buffer
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_fflush(
        FILE *fp
        );

/* Determine if fp refers to a terminal
 *
 * Returns -1 on failure setting errno
 * Returns  0 on succcess modifiying dest to non-zero if isatty
 *
 * EINVAL : fp or dest is null
 * function may fail and set errno for same reasons as
 * fileno
 * istty()*/
int
utility_isatty(
        FILE *fp,
        int *dest
        );

/* Obtain the size of a file on disk as reported by stat
 * may give inaccurate results in the case of NFS files
 *
 * Returns -1 on failure setting errno
 * Returns size of file on success
 * util_eInvalidArguments: invalid args passed*/
int
utility_fp_filesize(
        size_t *dest_sz,
        FILE *fp
        );

/* Determine if fname resolves to a regular file (i.e implements  [ -f fname ]
 *
 * Returns -1 on failure setting errno
 * Returns  0 on successfully discovering fname is not a file (i.e. doesn't exist or is a directory)
 * Returns  1 on successfully discovering fname is a file (i.e. exists and is a regular file)
 * EINVAL: invalid args passed*/
int utility_file_exists(const char *fname);

/* Determine if dname resolves to a directory (i.e implements  [ -d dname ]
 *
 * Returns -1 on failure setting errno
 * Returns  0 on successfully discovering dname is not a directory (i.e. doesn't exist or a file/fifo/link etc)
 * Returns  1 on successfully discovering dname is a directory (i.e. exists and is a directory)
 * EINVAL: invalid args passed*/
int utility_dir_exists(const char *dname);

/* Allocate an unique temp name using pid/ppid and system clock for entropy
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced tmp_name on success
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *utility_tmp_name(void);

/* Allocate an unique temp name using prefix, pid/ppid and system clock for entropy
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced tmp_name on success
 * function may fail and set errno for same reasons as utility_sprintf()*/
char * utility_tmp_name_prefixed(const char *prefix);

/* Allocate an unique logfile name using prefix, pid/ppid and system clock for entropy
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced tmp_name on success
 * function may fail and set errno for same reasons as utility_sprintf()*/
char *
utility_logfile_name(const char *prefix);

/* Open a file in "w+" mode using the name from utility_tmp_name(name is freed)
 * Returns NULL on failure setting errno
 * Returns fp opened in "w+" on success
 * function may fail and set errno for same reasons as utility_tmp_name()/ or fopen()*/
FILE *utility_tmp_file(void);

/* Allocate an array of bytes copied from 'fname'
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced copy of file on success
 * EINVAL: invalid args passed*/
void *utility_slurp(const char *fname);

/* Allocate an array of bytes copied from 'fname'
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced copy of file on success
 * EINVAL: invalid args passed
 * ENOENT: empty file
 * function may fail and set errno for same reasons as
 * utility_fslurp() */
void *utility_slurp_with_sz(size_t *dst_sz,const char *fname);

/* Call a func for each element in the list of lines returned for slurping file and
 * treating result as a newline terminated array of char *, i.e. argv style
 * returns -1 setting errno on error
 * returns -2 prologue canceled file operation
 * returns -3 visitor canceled file operation
 * EINVAL: invalid arguments */
int
utility_fname_visit(
        size_t *dst_nlines,
        const char *filename,
        void *state,
        int (*prologue)(const char *filename,size_t nlines,void *state),
        int (*visitor)(void *parg,size_t index, void *state)
        );

/* mallocs a directory name calling dirname() under unix and _splitpath_s() under windows */
char *
utility_dir_name(
        const char *path
        );

/* resolve 'base' to a directory and appends 'filename'
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
    utility_mk_filename(
            const char *base,
            const char *filename
            );

/* opens 'fname' in 'mode' and mmap into current address space
 * Caller must call mmunmap with the same 'size' argument as used in this call
 * Returns NULL on failure setting errno
 * Returns pointer to mmap'd file on success
 * EINVAL: invalid args passed*/
void *
utility_map_file(
        const char *fname,  /* file to map (file is created if it doen't exist*/
        int mode,           /* one of O_RDONLY, O_WRONLY or O_RDWR */
        size_t size         /* size of the mapping */
);

/* Allocate an array of bytes copied from 'fp'
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced copy of file on success
 * EINVAL: invalid args passed
 * ENOENT: empty file
 * function may fail and set errno for same reasons as
 * malloc()
 * ftell()/fseek()/utility_fp_filesize()/fread()*/
void *utility_fslurp(FILE *fp);

/* Allocate an array of bytes copied from 'fp'
 * Caller must free memory
 * Returns NULL on failure setting errno
 * Returns pointer to malloced copy of file on success
 * EINVAL: invalid args passed
 * ENOENT: empty file
 * function may fail and set errno for same reasons as
 * malloc()
 * ftell/fseek/utility_fp_filesize/fread*/
void *utility_fslurp_with_sz(FILE *fp,size_t *psz);


int
utility_approximately_zero(
        const double v,
        const double delta
        );
int
utility_same_double(
        const double a,
        const double b
        );

int
utility_diff_within_delta(
        const double diff,
        const double delta
        );

int
utility_round(
        double *dst_double,
        long  *dst_long,
        double dbl,
        const double pad
        );

int
utility_strtod(
        double *dst,
        const char *s
        );

double
utility_fabs(
        double v
        );

/* resolve user home directory and appends 'filename'
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_home_filename(
        const char *filename
        );

/* resolve user home directory and appends 'filename'
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_home_dirname(
        const char *dirname
        );

/* resolve user home directory and appends 'filename'
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_home_dir_filename(
        const char *dirname,
        const char *filename
        );

/* resolve user home directory and user id (uid)
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_get_uid_and_home_dir(
        util_uid_t *dest_uid,
        const char *suffix
        );

/* Determine last modified time of path 
 *
 * Returns -1 on failure setting errno
 * Returns  0 on successfully discovering dname is not a directory (i.e. doesn't exist or a file/fifo/link etc)
 * Returns  1 on successfully discovering dname is a directory (i.e. exists and is a directory)
 * util_eInvalidArguments: invalid args passed
 * function may fail and set errno for same reasons as
 * utility_stat */
int
utility_last_modified_at(
        const char *filename,
        time_t *dest
        );

/* join directory and 'filename' with platform suitable delimiter
 * Returns NULL on failure setting p2p_get_errno()
 * Returns pointer to malloced result on success
 * EINVAL: invalid args passed
 * function may fail and set errno for same reasons as utility_dir_name()*/
char *
utility_mk_dir_filename(
        const char *dirname,
        const char *filename
        );


/* cross platform variant */
int
utility_mkdir(
        const char * dir,
        mode_t mode
        );

#if defined( __cplusplus)
    }
#endif

/*
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#endif /* ndef LIB_UTILITY_H */

/* libUtility.c */
