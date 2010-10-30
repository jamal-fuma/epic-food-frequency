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
#ifndef LIB_UTILITY_ERRORS_H
#define LIB_UTILITY_ERRORS_H

#if defined( __cplusplus)
extern "C" {
#endif

#include "Utility_includes.h"

/* these are seperate so sign of enum can  be selected explictly
 * if an signed emum is desired set first member to -1
 * if an unsigned is desired make sure all values are >=0 simples */

    /* This is quite possibly a stupid Idea, but the premise to make windows socket errors look like
 * unix errnos */


enum {
    util_eConnect = 0,
    util_eAccept = 1,
#if defined(_WIN32)
    util_eShutReadOnly                  = SD_RECEIVE,
    util_eShutWriteOnly                 = SD_SEND,
    util_eShutReadWrite                 = SD_BOTH,
    /* posix name */
#elif ! defined(_WIN32) && (defined(SHUT_RD) && defined(SHUT_WR))
    util_eShutReadOnly                  = SHUT_RD,
    util_eShutWriteOnly                 = SHUT_WR,
    util_eShutReadWrite                 = SHUT_RDWR,
#else
    /* allmost everybody uses these even windows */
    util_eShutReadOnly                  = 0,
    util_eShutWriteOnly                 = 1,
    util_eShutReadWrite                 = 2,
#endif
};

enum  {
    util_eAI_Mask               = (~(1UL)),
    util_eAI_ActiveOpen         = (1<<1),
    util_eAI_PreferIPVFour      = (1<<2),
    util_eAI_PreferIPVSix       = (1<<3),
    util_eAI_AnyVersion         = (util_eAI_PreferIPVSix|util_eAI_PreferIPVFour),

    util_eAI_Datagram           = (1<<4),
    util_eAI_ByteStream         = (1<<5),
    util_eAI_AnyKind            = (util_eAI_Datagram|util_eAI_ByteStream),
    util_eAI_NumericAddr        = (1<<6),
    util_eAI_NumericPort        = (1<<7),

    util_eFD_NonBlocking        = (1<<8),   /* O_NBLOCK */
    util_eFD_Asynchronous       = (1<<9),
    util_eFD_CloseOnExec        = (1<<10),  /* FD_CLOEXC */

    util_eSD_TcpCork            = (1<<11),  /* TCP_CORK */
    util_eSD_TcpPush            = (1<<12),  /* TCP_PUSH */
    util_eSD_TcpNoDelay         = (1<<13),  /* TCP_NODELAY*/
    util_eSD_ReuseAddress       = (1<<14),  /* SO_REUSEADDR */
    util_eSD_ReusePort          = (1<<15),  /* SO_REUSEPORT */
    util_eSD_PendingError       = (1<<16),  /* SO_ERROR */
    util_eSD_PendingBytes       = (1<<17),  /* FIONREAD */

/* IP multicasting socketoption
 *  IPPROTO_IP, IP_MULTICAST_IF, IP_MULTICAST_TTL, IP_ADD_MEMBERSHIP, */

    /* ip multicasting stuff */
    util_eIP_MulticastInterface = ((util_eFD_Asynchronous | util_eAI_Datagram | util_eSD_PendingError | util_eSD_PendingBytes)),

    util_eSD_PacketCoalesing    = (1<<20), /* toggles TcpPush/TcpCork/TcpNoDelay as a single operation */
    util_eSD_AddressReuse       = (1<<22), /* toggles TcpPush/TcpCork/TcpNoDelay as a single operation */
 };
enum { util_eError = -1, util_eOk = 0,


#if defined(_WIN32)


/* windows version of EINVAL */

#if defined EINVAL && defined(WSAEINVAL) && EINVAL != WSAEINVAL
    util_eInvalidArguments              = WSAEINVAL,
#elif defined EINVAL
    util_eInvalidArguments              = EINVAL,
#else
        #error "EINVAL is not defined, something is broken"
#endif /* defined EINVAL ||  (defined EINVAL && defined(WSAEINVAL) && EINVAL != WSAEINVAL)) */



/* windows version of EAGAIN */
#if defined WSAEAGAIN
    util_eAgain                         = WSEAGAIN,
#elif defined EAGAIN && defined(EWOULDBLOCK) && EAGAIN != EWOULDBLOCK
    util_eAgain                         = EAGAIN,
#elif defined(WSAEWOULDBLOCK)
    util_eAgain                         = WSAEWOULDBLOCK,
#elif defined(EWOULDBLOCK)
    util_eAgain                         = EWOULDBLOCK,
#else
    util_eAgain                         = (10035),
#if defined (WARN_ON_WINSOCK)
    #warning "unable to find definition for WSAEAGAIN"
    #warning "using definition from 'http://msdn.microsoft.com/en-us/library/ms740668%28v=VS.85%29.aspx'"
#endif /* defined (WARN_ON_WINSOCK) */
#endif  /* defined EAGAIN ||  (defined EAGAIN && defined(WSAEAGAIN) && EAGAIN != WSAEAGAIN)) */


/* windows version of EPROTO */
#if defined WSAPROTO
    util_eProtocol                      = WSAEPROTO,
#elif defined EPROTO
    util_eProtocol                      = EPROTO,
#else
    util_eProtocol                      = (20006),
#if defined (WARN_ON_WINSOCK)
    #warning "unable to find definition for WSAEPROTO"
    #warning "using definition from 'http://msdn.microsoft.com/en-us/library/ms740668%28v=VS.85%29.aspx'"
#endif /* defined (WARN_ON_WINSOCK) */
#endif /* defined EPROTO ||  (defined EPROTO && defined(WSAEPROTO) && EPROTO != WSAEPROTO)) */

    util_eAddrInUse 			= WSAEADDRINUSE,
    util_eAddrNotAvail 			= WSAEADDRNOTAVAIL,
    util_eAfNoSupport 			= WSAEAFNOSUPPORT,
    util_eAlready 			= WSAEALREADY,
    util_eConnAborted 			= WSAECONNABORTED,
    util_eConnRefused 			= WSAECONNREFUSED,
    util_eConnReset 			= WSAECONNRESET,
    util_eDestAddrReq 			= WSAEDESTADDRREQ,
    util_eDquot 			= WSAEDQUOT,
    util_eHostDown 			= WSAEHOSTDOWN,
    util_eHostUnreach 			= WSAEHOSTUNREACH,
    util_eInProgress 			= WSAEINPROGRESS,
    util_eInterupted 			= WSAEINTR,
    util_eIsConn 			= WSAEISCONN,
    util_eLoop 			        = WSAELOOP,
    util_eMsgSize 			= WSAEMSGSIZE,
    util_eNameTooLong 			= WSAEINTR,
    util_eNetDown 			= WSAENETDOWN,
    util_eNetReset 			= WSAENETRESET,
    util_eNetUnreach 			= WSAENETUNREACH,

/* windows version of ENOBUFS */
#if defined WSAENOBUFS
    util_eNoBufs                        = WSAENOBUFS,
#elif defined ENOBUFS
    util_eNoBufs                        = ENOBUFS,
#else
    util_eNoBufs                        = (10055),
#if defined (WARN_ON_WINSOCK)
    #warning "unable to find definition for WSAENOBUFS"
    #warning "using definition from 'http://msdn.microsoft.com/en-us/library/ms740668%28v=VS.85%29.aspx'"
#endif /* defined (WARN_ON_WINSOCK) */
#endif /* defined ENOBUFS ||  (defined ENOBUFS && defined(WSAENOBUFS) && ENOBUFS != WSAENOBUFS)) */

    util_eNoProtoOpt 			= WSAENOPROTOOPT,
    util_eNotConn 			= WSAENOTCONN,
    util_eNotEmpty 			= WSAENOTEMPTY,
    util_eNotSock 			= WSAENOTSOCK,
    util_eOpNotSupp 			= WSAEOPNOTSUPP,
    util_ePfNoSupport 			= WSAEPFNOSUPPORT,
    util_eProclim 		        = WSAEPROCLIM,
    util_eProtoNoSupport 		= WSAEPROTONOSUPPORT,
    util_ePrototype 		        = WSAEPROTOTYPE,
    util_eRemote 		        = WSAEREMOTE,
    util_eShutdown 		        = WSAESHUTDOWN,
    util_eSocktNoSupport 		= WSAESOCKTNOSUPPORT,
    util_eStale 			= WSAESTALE,
    util_eTimedOut 			= WSAETIMEDOUT,
    util_eTooManyRefs 			= WSAETOOMANYREFS,
    util_eUsers 			= WSAEUSERS,
    util_eWouldBlock 			= WSAEWOULDBLOCK,

/* getaddrinfo errors on windows */

    /*   A recoverable failure in name resolution occurred. */
    util_eAI_Again  			= WSATRY_AGAIN,

    /*   A nonrecoverable failure in name resolution occurred. */
    util_eAI_Fail  			= WSANO_RECOVERY,

    /* hints.ai_flags included AI_CANONNAME and name was NULL.
     * or other incompatible argument/flag combinations */
    util_eAI_FlagInvalid  		= WSAEINVAL,

    /* The ai_socktype member of the pHints parameter is not supported */
    util_eAI_SocketTypeInvalid  	= util_eSocktNoSupport,

    /* The hints.ai_family is not supported on this system e.g. unix sockets on windows */
    util_eAI_FamilyInvalid  		= util_eAfNoSupport,

    /*    A memory allocation failure occurred */
    util_eAI_NoMemory  			= WSA_NOT_ENOUGH_MEMORY,

    /* name resolution failure or both host and service were null */
    util_eAI_HostNotFound               = WSAHOST_NOT_FOUND,

    /* can't resolve service to a port for the specified socket type */
    util_eAI_PortNotFound               = WSATYPE_NOT_FOUND,
#elif !defined(_WIN32)
    util_eInvalidArguments              = EINVAL,
    util_eAgain                         = EAGAIN,
    util_eProtocol                      = EPROTO,
    util_eAddrInUse 			= EADDRINUSE,
    util_eAddrNotAvail 			= EADDRNOTAVAIL,
    util_eAfNoSupport 			= EAFNOSUPPORT,
    util_eAlready 			= EALREADY,
    util_eConnAborted 			= ECONNABORTED,
    util_eConnRefused 			= ECONNREFUSED,
    util_eConnReset 			= ECONNRESET,
    util_eDestAddrReq 			= EDESTADDRREQ,
    util_eDquot 			= EDQUOT,
    util_eHostDown 			= EHOSTDOWN,
    util_eHostUnreach 			= EHOSTUNREACH,
    util_eInProgress 			= EINPROGRESS,
    util_eInterupted 			= EINTR,
    util_eIsConn 			= EISCONN,
    util_eLoop 			        = ELOOP,
    util_eMsgSize 			= EMSGSIZE,
    util_eNameTooLong 			= EFAULT,
    util_eNetDown 			= ENETDOWN,
    util_eNetReset 			= ENETRESET,
    util_eNetUnreach 			= ENETUNREACH,
    util_eNoBufs 			= ENOBUFS,
    util_eNoProtoOpt 			= ENOPROTOOPT,
    util_eNotConn 			= ENOTCONN,
    util_eNotEmpty 			= ENOTEMPTY,
    util_eNotSock 			= ENOTSOCK,
    util_eOpNotSupp 			= EOPNOTSUPP,
    util_ePfNoSupport 			= EPFNOSUPPORT,

#if defined EPROCLIM
    util_eProclim 		        = EPROCLIM,
#endif
    util_eProtoNoSupport 		= EPROTONOSUPPORT,
    util_ePrototype 		        = EPROTOTYPE,
    util_eRemote 		        = EREMOTE,
    util_eShutdown 		        = ESHUTDOWN,
    util_eSocktNoSupport 		= ESOCKTNOSUPPORT,
    util_eStale 			= ESTALE,
    util_eTimedOut 			= ETIMEDOUT,
    util_eTooManyRefs 			= ETOOMANYREFS,
    util_eUsers 			= EUSERS,
    util_eWouldBlock 			= EWOULDBLOCK,

/* getaddrinfo errors on POSIX */

    /*   A recoverable failure in name resolution occurred. */
    util_eAI_Again  			= EAI_AGAIN,

    /*   A nonrecoverable failure in name resolution occurred. */
    util_eAI_Fail  			= EAI_FAIL,

    /* hints.ai_flags included AI_CANONNAME and name was NULL.
     * or other incompatible argument/flag combinations */
    util_eAI_FlagInvalid  		= EAI_BADFLAGS,

    /* The ai_socktype member of the pHints parameter is not supported */
    util_eAI_SocketTypeInvalid  	= EAI_SOCKTYPE,

    /* The hints.ai_family is not supported on this system e.g. unix sockets on windows */
    util_eAI_FamilyInvalid  		= EAI_FAMILY,

    /*    A memory allocation failure occurred */
    util_eAI_NoMemory  			= EAI_MEMORY,

    /* name resolution failure or both host and service were null */
    util_eAI_HostNotFound               = EAI_NONAME,

    /* can't resolve service to a port for the specified socket type */
    util_eAI_PortNotFound               = EAI_SERVICE,
#endif /* !defined(_WIN32) */
};
#if defined( __cplusplus)
    }
#endif

#endif /* ndef LIB_UTILITY_ERRORS_H */
