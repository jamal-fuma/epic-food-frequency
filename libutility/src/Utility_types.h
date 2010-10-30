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
#ifndef LIB_UTILITY_TYPES_H
#define LIB_UTILITY_TYPES_H

#if defined( __cplusplus)
extern "C" {
#endif

#include "libUtility.h"
#define UTILITY_NENTRIES(arr) (sizeof(arr)/ sizeof(arr[0]))

/* These functions should return <0 on failure and >=0 on success */
typedef int (*output_func_t)(const char *str);

/* opaque structure implementing a stack of pointers */
typedef struct utility_pstk PTR_STACK;

/* implements a Pushback buffer*/
typedef struct utility_push_buf PUSH_BUF;
#if !defined PRIu8
/* sodding windows doesn't defined inttypes macros
 *  this that should work correctly given printf promotion rules */
#define PRIu8 "u"
#define PRIu16 "u"
#define UTIL_PRI_64_PREFIX "IA64"    /* GCC will accept L or depricated ll*/
#define PRIu64  (UTIL_PRI_64_PREFIX)"u"
#define PRId64  (UTIL_PRI_64_PREFIX)"d"
#endif

#if (!defined(HAVE_STDINT_H) && defined(_MSC_VER) && defined (_WIN32) )
    typedef __int8               int8_t;
    typedef __int16              int16_t;
    typedef __int32              int32_t;
#if(defined(_WIN64)
    typedef __int64              int64_t;
    typedef unsigned int64_t     uint64_t;
    typedef __int64              intptr_t;
#else
    typedef int32_t              intptr_t;
#endif /* _MSC_VER && _WIN64 */
    typedef unsigned intptr_t    uintptr_t;
    typedef unsigned int8_t      uint8_t;
    typedef unsigned int16_t     uint16_t;
    typedef unsigned int32_t     uint32_t;
#endif /* _MSC_VER && _WIN32 */

typedef uintptr_t util_handle_t;
typedef struct iovec util_iov_t;
typedef struct addrinfo  util_addrinfo_t;
#if !defined(_WIN32) && !defined(INVALID_SOCKET)
#define INVALID_SOCKET util_eError
#endif /* !defined(_WIN32) && !defined(INVALID_SOCKET) */

#if !defined(_WIN32) && !defined(SOCKET_ERROR)
#define SOCKET_ERROR util_eError
#endif /* !defined(_WIN32) && !defined(INVALID_SOCKET) */

#if !defined(_WIN32) && !defined(INVALID_HANDLE)
#define INVALID_HANDLE util_eError
#endif /* !defined(_WIN32) && !defined(INVALID_HANDLE) */


#define ALIGN_8(v)    (((v)+(7L)) & (~7L))
#define ALIGN_8_SZ(v) ( (size_t )(ALIGN_8((v))))
#define ALIGN_8_ADDR(v)( (void *)(ALIGN_8((uintptr_t)(v))))

#if defined(_WIN32)
    typedef HANDLE          util_uid_t;
    typedef struct stat     util_stat_t;
    typedef SOCKET          util_sock_fd_t;
    typedef unsigned long   util_non_block_ioctl_t;
    typedef BOOL            util_tcp_nodelay_sockopt_t;
    typedef int             util_socklen_t;
    struct iovec
    {
        size_t iov_len;
        void *iov_base;
    };

    #define util_socket_error(sd)( (sd) == SOCKET_ERROR)
    #define utility_socket_invalid(sd)( (sd) == INVALID_SOCKET)

    #define p2p_set_errno(err)              (WSASetLastError((err)))
    #define p2p_get_errno()                 (WSAGetLastError())
#else
    typedef int                 util_sock_fd_t;
    typedef struct stat         util_stat_t;
    typedef socklen_t           util_socklen_t;
    typedef int                 util_non_block_ioctl_t;
    typedef int                 util_tcp_nodelay_sockopt_t;
    typedef uid_t               util_uid_t;

    #define utility_socket_invalid(sd)( (0>((sd))) )
    #define util_socket_error(sd)( utility_socket_invalid(sd) )

    #define p2p_set_errno(err) do { errno = (err); }while(0)
    #define p2p_get_errno() ((errno))
#endif /* !defined(_WIN32) */

/* map various address info parameters into a bitmask */
#define UTILITY_BIT_IS_SET(flag,mask)((mask) == (((flag) & (mask))))
#define UTILITY_BIT_IS_CLR(flag,mask)( !(UTILITY_BIT_IS_SET((flag),(mask))) )

#define UTILITY_AI_IPV4_BIT(flags) ( UTILITY_BIT_IS_SET((flags),util_eAI_PreferIPVFour) )
#define UTILITY_AI_IPV6_BIT(flags) ( UTILITY_BIT_IS_SET((flags),util_eAI_PreferIPVSix)  )
#define UTILITY_AI_DGRAM_BIT(flags) ( UTILITY_BIT_IS_SET((flags),util_eAI_Datagram)     )
#define UTILITY_AI_STREAM_BIT(flags) ( UTILITY_BIT_IS_SET((flags),util_eAI_ByteStream)  )
#define UTILITY_AI_CLIENT_BIT(flags) ( UTILITY_BIT_IS_SET((flags),util_eAI_ActiveOpen)  )
#define UTILITY_AI_SERVER_BIT(flags) ( (0 == UTILITY_AI_CLIENT_BIT(flags)) )


#define UTILITY_AI_VERSION(flags) ( ( (UTILITY_AI_IPV4_BIT(flags) ) ^ (UTILITY_AI_IPV6_BIT((flags)) ) ) )
#define UTILITY_AI_IPV6_HOSTNAME(flags,host) ((!(host) || !strcmp("",(host))) ? ( (UTILITY_AI_CLIENT_BIT((flags))) ?  "::" : "::1") : (host))
#define UTILITY_AI_IPV4_HOSTNAME(flags,host) ((!(host) || !strcmp("",(host))) ? ( (UTILITY_AI_CLIENT_BIT((flags))) ? "0.0.0.0" : "127.0.0.1") : (host))
#define UTILITY_AI_TYPE(flags)( ((UTILITY_AI_DGRAM_BIT((flags)) ^ UTILITY_AI_STREAM_BIT(flags)) ) )

#define UTILITY_AI_IPV4_HOSTNAME(flags,host) ((!(host) || !strcmp("",(host))) ? ( (UTILITY_AI_CLIENT_BIT((flags))) ? "0.0.0.0" : "127.0.0.1") : (host))
#define UTILITY_AI_FAMILY(flags) ( (UTILITY_AI_VERSION((flags)) ? ( UTILITY_AI_IPV4_BIT((flags)) ?  AF_INET : AF_INET6) : (AF_UNSPEC)))
#define UTILITY_AI_FLAGS(flags) (((UTILITY_AI_CLIENT_BIT((flags))) ? AI_PASSIVE: AI_CANONNAME))
#define UTILITY_AI_SOCK_TYPE(flags) ( UTILITY_AI_TYPE(((flags)) ? ( UTILITY_AI_DGRAM_BIT((flags)) ?  SOCK_DGRAM : SOCK_STREAM) : (0)))


#if !defined(HOST_NAME_MAX)
    #define HOST_NAME_MAX 256
#endif
    typedef char util_hostname_t[HOST_NAME_MAX+1];

#if defined(PATH_MAX)
    #define UTILITY_PATH_NAME_MAX PATH_MAX

#elif (defined(_WIN32)  && defined(_PATH_MAX))
    #define UTILITY_PATH_NAME_MAX _PATH_MAX
#else
    #error "PATH_MAX is not defined on this system"
#endif
    typedef char util_pathname_t[PATH_MAX+1];

#if defined( __cplusplus)
}
#endif

#endif /* ndef LIB_UTILITY_TYPES_H */
