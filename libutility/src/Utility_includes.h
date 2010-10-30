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
#ifndef LIB_UTILITY_INCLUDES_H
#define LIB_UTILITY_INCLUDES_H

#if defined( __cplusplus)
extern "C" {
#endif

#include <config.h>

#if defined(_WIN32) && !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
    /* other wise we don't getaddrinfo */
    #define _WIN32_WINNT 0x0501
    #include <process.h>  /* getpid() */

    #include <sys/types.h> /* stat()/fstat() */
    #include <sys/stat.h>
#endif /* defined(_WIN32) && !defined(WIN32_LEAN_AND_MEAN) */

#if defined(HAVE_SYS_MMAN_H)
#include <sys/mman.h>
#endif

#if defined(HAVE_ARPA_INET_H)
#include <arpa/inet.h>
#endif

#if defined(HAVE_ASSERT_H)
#include <assert.h>
#endif

#if defined HAVE_LIBGEN_H
#include <libgen.h>  /* posix version of basename */
#endif

#if defined(HAVE_PWD_H)
    #include <pwd.h>
#elif !defined(_WIN32)
    #error "Missing <pwd.h> header"
#endif /* defined(HAVE_PWD_H) &&  (!defined(_WIN32) && (defined(HAVE_GETPWUID) && defined(HAVE_GETUID))) */

#if defined(HAVE_NETDB_H)
#include <netdb.h>
#endif

#if defined(HAVE_NETINET_IN_H)
#include <netinet/in.h>

#if defined(HAVE_SYS_FCNTL_H)
#include <sys/fcntl.h>
#endif

#if defined(HAVE_SYS_SELECT_H)
#include <sys/select.h>
#endif

#if defined(HAVE_SYS_SOCKET_H)
#include <sys/socket.h>
#endif

#if defined(HAVE_SYS_TYPES_H)
#include <sys/types.h>
#endif

/* solaris triggers for FIONREAD see http://www.mail-archive.com/classpath-patches@gnu.org/msg10173.html */
#if defined(HAVE_SYS_IOCTL_H)
#define BSD_COMP
#include <sys/ioctl.h>
#endif

#if defined(HAVE_SYS_FILIO_H)
    #include <sys/filio.h>
#endif

#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

/* use a flag as suggested http://lists.mplayerhq.hu/pipermail/ffmpeg-user/2005-November/001493.html */
#if defined(HAVE_INTTYPES_H)
#include <inttypes.h>
#elif defined(WIN32) && !defined(__MINGW32__) && !defined(__CYGWIN__)
#define EMULATE_INTTYPES
#endif

#endif /* !defined win32 */

#if !defined(_WIN32) && !defined(HAVE_SYS_MMAN_H)
    #error "NO: <sys/mman.h>, bailing"
#endif

#if defined(_WIN32) && !defined(WIN32_LEAN_AND_MEAN)
    #define WIN32_LEAN_AND_MEAN
#endif /* defined(_WIN32) && !defined(WIN32_LEAN_AND_MEAN) */

#if defined(_WIN32) && defined(WIN32_LEAN_AND_MEAN)
    #include <windows.h>
#if defined(_WIN32) && !defined(HAVE_WINSOCK2_H)
    #error "winsock header missing"
#elif defined(_WIN32) && defined(HAVE_WINSOCK2_H)
    #include <winsock2.h>
#endif
    #include <ws2tcpip.h>
    #include <winsock2.h>
#endif

#if defined(_WIN32) && (!defined(HAVE_WINDOWS_H) || !defined(HAVE_WINSOCK2_H) || !defined(HAVE_WS2TCPIP_H))
    #error "Error: missing windows platform headers"
#endif /*  !defined(_WIN32) && !defined(HAVE_STDINT_H) && !defined(HAVE_SYS_TYPES_H) */



#if !defined(_WIN32) && !defined(HAVE_STDINT_H) && !defined(HAVE_SYS_TYPES_H)
    #error "NO: <stdint.h> or <sys/types.h> and not windows, bailing" __LINE__ __FILE__
#endif /*  !defined(_WIN32) && !defined(HAVE_STDINT_H) && !defined(HAVE_SYS_TYPES_H) */

#if !defined(HAVE_STDINT_H)  && !defined(HAVE_SYS_TYPES_H)
    #error "NO: <stdint.h> or <sys/types.h>, bailing" __LINE__ __FILE__
#endif

#if (!defined(HAVE_STDINT_H)  && defined(_WIN32) && !defined(_MSC_VER) )
    #error "NO: <stdint.h> on windows and not vc++ bailing" __LINE__ __FILE__
#endif

#if defined(HAVE_CTYPE_H)
#include <ctype.h>
#endif

#if defined(HAVE_ERRNO_H)
#include <errno.h>
#endif

#if defined(HAVE_FCNTL_H)
#include <fcntl.h>
#endif

#if defined(HAVE_TIME_H)
#include <time.h>
#endif


#if defined(HAVE_LIMITS_H)
#include <limits.h>
#else
#error "limits.h header missing" __LINE__ __FILE__ __DATE__
#endif

#if defined(HAVE_NETDB_H)
#include <netdb.h>
#endif

#if defined(HAVE_NETINET_IN_H)
#include <netinet/in.h>
#endif

#if defined(HAVE_NETINET_IP_H)
    #include <netinet/ip.h>
#endif

#if defined(HAVE_NETINET_TCP_H)
#include <netinet/tcp.h>
#endif

#if defined(HAVE_STDARG_H)
#include <stdarg.h>
#endif

#if defined(HAVE_STDINT_H)
#include <stdint.h>
#endif

#if defined(HAVE_STDIO_H)
#include <stdio.h>
#endif

#if defined(HAVE_STDLIB_H)
#include <stdlib.h>
#endif

#if defined(HAVE_STRING_H)
#include <string.h>
#endif

/* curl uses this method to detect platforms that define O_NONBLOCK but dont honour it */
#if ((defined(sun) || defined(__sun__) || defined(__SUNPRO_C) ||  defined(__SUNPRO_CC) )  && ( \
            !(defined(__SVR4) || defined(__srv4__)) || \
            ((defined(_AIX) || defined(__xlC__)) && !defined(_AIX41)) || defined(__BEOS__)))
#error "platforms that defines O_NONBLOCK but dont honour it"
#endif


#if defined( __cplusplus)
}
#endif

#endif /* ndef LIB_UTILITY_INCLUDES_H */
