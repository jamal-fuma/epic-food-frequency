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

    int
        p2p_nslookup(
                p2p_address_t *addr,
                const char *host,
                uint32_t flags,
                ...
                )
        {
            va_list ap;
            struct addrinfo *head,*node;
            int err, retry;

            void *service=NULL;

            /* big enough for the largest decimal representation of uint16_t */
            char numeric_service[7+1] = {0};

            va_start(ap,flags);
            service = va_arg(ap,void *);
            if(service && (flags & util_eAI_NumericPort))
            {
                /* the interface to getaddrinfo requires a string so we convert
                 * from loop friendly numbers to user friendly strings */
                snprintf(numeric_service, (sizeof(numeric_service)-1), ("%" PRIu16), (*(uint16_t *)service));
                service  = numeric_service;
            }
            va_end(ap);

            /* normally insist on only being passed empty strings not nulls
             * but  this time we silently convert since we have a zeroed buffer anyways */
            p2p_set_errno(util_eInvalidArguments);
            if(!service)
                service = numeric_service;

            /* retry transient errors */
            for(err=util_eWouldBlock,retry=5; (util_eWouldBlock == err && --retry); err = p2p_get_errno())
            {
                /* resolve host and service */
                if( (err = p2p_getaddrinfo(&head,host,((const char *)service),flags)))
                    continue;

                /* walk returned addresses */
                for(node = head, err=0; (node && err); node = node->ai_next)
                {
                    util_sock_fd_t sd = INVALID_SOCKET;

                    /* try for a decent socket with proper ioctls set */
                    if(p2p_socket_open(node,&sd))
                        continue;

                    /* cache transient info prior to shared cleanup */
                    addr->sd_sock   = sd;
                    addr->sd_addr   = *node->ai_addr;
                    addr->sd_len    = node->ai_addrlen;
                    addr->sd_flags  = flags;
                    addr->sd_error  = util_eOk;
                    addr->sd_interval.tv_sec = p2p_default_interval_secs();
                    addr->sd_interval.tv_usec = p2p_default_interval_msecs();
                    err = 0;
                    freeaddrinfo(head);
                    return util_eOk;
                }
                freeaddrinfo(head);
            }
            p2p_set_errno(err);
            return util_eError;
        }


    int
        p2p_getaddrinfo(
                util_addrinfo_t **dest,
                const char *host,
                const char *service,
                uint32_t flags
                )
        {
            util_addrinfo_t *results,
                            hints;
            int error,
                reason;

            p2p_set_errno(util_eInvalidArguments);
            if(dest || (!host && !service))
                return util_eError;

            memset(&hints,0,sizeof(hints));
            error = p2p_os_getaddrinfo(host, service, &hints, &results,flags);
            if(!error)
            {
                /* caller must freeaddrinfo */
                *dest = results;
                return util_eOk;
            }

            reason = p2p_get_errno();
            switch(reason)
            {
                /*  A temporary failure in name resolution occurred */
                case util_eWouldBlock:
                case util_eAI_Again:
                    p2p_set_errno(util_eWouldBlock);
                    return util_eError;

                    /* the domain (ai_family) is unsupported or the flags are invalid  */
                case util_eAI_FlagInvalid:
                case util_eAI_FamilyInvalid:
                case util_eAI_SocketTypeInvalid:
                    p2p_set_errno(util_eInvalidArguments);
                    return util_eError;

                    /*   A nonrecoverable failure in name resolution occurred. */
                case util_eAI_Fail:
                    p2p_set_errno(util_eConnAborted);
                    return util_eError;

                    /* out of memory for getaddrinfo */
                case util_eAI_NoMemory:
                    p2p_set_errno(ENOMEM);
                    return util_eError;

                    /* success but not found for getaddrinfo */
                case util_eAI_PortNotFound:
                case util_eAI_HostNotFound:
                    p2p_set_errno(reason);
                    return util_eError;
                default:
                    p2p_set_errno(reason);
            }
            return error;
        }


    /* Opens a socket using the specifed mode resolving name/service with getaddrinfo()
     *
     * return  0 on success modifiying *psd to the newly created socket
     * return -1 on failure
     * EINVAL: invalid or NULL arguments
     * function may fail and set errno for same reason as
     * socket()
     * bind()
     * make_socket_reusable()
     * listen()
     * make_socket_non_blocking()
     * disable_packet_coalescing()
     * */
    int
        p2p_open(
                util_sock_fd_t *psd,
                const char *host,
                const char *service,
                uint32_t flags
                )
        {
            p2p_address_t addr;
            p2p_address_t *paddr = &addr;

            /*  pass empty strings not nulls damn it*/
            p2p_set_errno(util_eInvalidArguments);
            if(!psd || !host || !service)
                return util_eError;

            /* use ipv4 stream sockets unless user overides us */
            memset(&addr,0,sizeof(addr));
            if(util_eError == (p2p_nslookup(paddr,host,((0==flags) ? p2p_default_flags() : flags),service )))
                return util_eError;

            *psd = paddr->sd_sock;
            return util_eOk;
        }

    /* Opens a socket using a node returned from a previous call to getaddrinfo()
     *
     * return  0 on success modifiying *psd to the newly created socket
     * return -1 on failure
     * EINVAL: invalid or NULL arguments
     * function may fail and set errno for same reason as
     * socket()
     * bind()
     * make_socket_reusable()
     * listen()
     * make_socket_non_blocking()
     * disable_packet_coalescing()
     * */
    int
        p2p_socket_open(
                util_addrinfo_t *node,
                util_sock_fd_t *psd
                )
        {
            util_sock_fd_t sd;
            int err = util_eInvalidArguments;

            /* FIXME:
             * Assumes getaddrinfo sets AI_PASSIVE correctly */
            p2p_set_errno(util_eInvalidArguments);
            if((node && psd) && (util_eOk ==  (err = ( (node->ai_flags & AI_PASSIVE) ?
                                /* listening sockets */
                                p2p_os_socket(&sd,node,p2p_os_passive_open) :
                                /* active or connecting sockets */
                                p2p_os_socket(&sd,node,p2p_os_active_open) ))))
            {
                *psd = sd;
            }
            return err;
        }

    /* Closes a socket obtained from a call to p2p_open
     *
     * returns 0 on success
     * returns <0 on failure possibly modifying errno
     * may fail for any of the reasons specified for p2p_close_socket()
     * util_eInvalidArguments: invalid arguments*/
    int
        p2p_close(
                util_sock_fd_t sd
                )
        {
            return p2p_socket_close(sd);
        }

    /* Closes a socket obtained from a call to p2p_socket_open
     *
     * returns 0 on success
     * returns <0 on failure possibly modifying errno
     * may fail for any of the reasons specified for shutdown(),close(),
     * util_eInvalidArguments: invalid arguments*/
    int
        p2p_socket_close(
                util_sock_fd_t sd
                )
        {
            int err;
            if(shutdown(sd,util_eShutReadWrite))
            {
                err = p2p_get_errno();
                return util_eError;
            }
            /* this is a hard disconnect and should set the FIN bit and force dropping the connection */
            return p2p_os_disconnect(sd);
        }

    /* Writes 'len' bytes from the buffer 'buf' to socket 'sd'
     *
     * returns number of bytes written on success
     * return <0 on failure, setting errno for any of the reasons given for writev
     * util_eInvalidArguments: invalid or NULL arguments */
    int
        p2p_writev(
                util_sock_fd_t sd,
                size_t *tx_bytes,
                util_iov_t *iov
                )
        {
            ssize_t ret;
            /* sanity check */
#if( !defined(_WIN32) && defined(HAVE_WRITEV))

            /* this called WSASend on windows */
            if(0>(ret = writev(sd, iov, 1)))
                return util_eError;

            tx_bytes = (size_t)ret;

#else
            /* swap this out once the emulation layer is working */
            if(0>(ret = p2p_os_send(sd,tx_bytes,iov->iov_base, iov->iov_len,0)))
                return util_eError;

#endif
            return util_eOk;
        }


    /* Reads 'len' bytes into the buff from socket 'sd'
     *
     * returns number of bytes read on success in rx_bytes (unmodifed on error)
     * return <0 on failure, setting errno for any of the reasons given for
     * readv() or
     * p2p_os_recv()
     * EINVAL: invalid or NULL arguments */
    int
        p2p_read(
                util_sock_fd_t sd,
                size_t *rx_bytes,
                void *buf,
                size_t len
                )
        {
            util_iov_t vec;

            /* sanity check */
            errno = util_eInvalidArguments;
            if(0>sd || !buf || !rx_bytes || !len)
                return util_eError;

            vec.iov_base = buf;
            vec.iov_len = len;

            return p2p_readv(sd,rx_bytes,&vec);
        }

    /* Write 'len' bytes to socket 'sd'
     *
     * returns number of bytes written on success in tx_bytes (unmodifed on error)
     * return <0 on failure, setting errno for any of the reasons given for
     * writev() or
     * p2p_os_send()
     * EINVAL: invalid or NULL arguments */
    int
        p2p_write(
                util_sock_fd_t sd,
                size_t *tx_bytes,
                void *buf,
                size_t len
                )
        {
            util_iov_t vec;

            /* sanity check */
            errno = util_eInvalidArguments;
            if(0>sd || !buf || !tx_bytes || !len)
                return util_eError;

            vec.iov_base = buf;
            vec.iov_len = len;

            return p2p_writev(sd,tx_bytes,&vec);
        }




    /* Reads 'len' bytes into the iov from socket 'sd'
     *
     * returns number of bytes read on success in rx_bytes (unmodifed on error)
     * return <0 on failure, setting errno for any of the reasons given for
     * readv() or
     * p2p_os_recv
     * EINVAL: invalid or NULL arguments */
    int
        p2p_readv(
                util_sock_fd_t sd,
                size_t *rx_bytes,
                util_iov_t *iov
                )
        {
            ssize_t ret;

            /* sanity check */
            errno = util_eInvalidArguments;
            if(0>sd || !iov || !rx_bytes)
                return util_eError;

#if( !defined(_WIN32) || defined(HAVE_READV))

            /* this called WSARecv on windows */
            if(0>(ret = readv(sd, iov, 1)))
                return util_eError;
            *rx_bytes = (size_t)ret;

#else
            /* swap this out once the emulation layer is working */
            if(0>(ret = p2p_os_recv(sd,rx_bytes,iov->iov_base, iov->iov_len,0)))
                return util_eError;

#endif
            return util_eOk;
        }

   int
        p2p_select(
                util_sock_fd_t sd,
                fd_set *rs,
                fd_set *ws,
                fd_set *es,
                int *pnfds,
                int wait
                )
        {

        p2p_address_t addr;

        p2p_set_errno(util_eInvalidArguments);
        if(!utility_socket_invalid(sd) || !rs || !ws || !es || !pnfds)
                return util_eInvalidArguments;

        memset(&addr,0,sizeof(addr));
        addr.sd_interval.tv_sec = p2p_default_interval_secs();
        addr.sd_interval.tv_usec = p2p_default_interval_msecs();
        addr.sd_sock = sd;

        return p2p_os_select(&addr,rs,ws,es,pnfds,wait);
        }


    /******************************************888
     *
     *
     *   helpers
     *
     ****************************************************
     */
    inline int
        p2p_ignorable_error(
                const int err
                )
        {
            /* util_eInterupted is only an error if we are not catching signals
             * we can always safely ignore the rest of these */
            return( util_eInterupted    == err ||
                    util_eConnAborted   == err ||
                    util_eProtocol      == err ||
                    util_eWouldBlock    == err || util_eAgain == err );
        }

    inline uint32_t
        p2p_default_flags(
                void
                )
        {
            /* FIXME: tunable with env variable */
            return (
                    util_eAI_PreferIPVFour | util_eAI_ByteStream    |
                    util_eAI_ActiveOpen  | util_eFD_NonBlocking
                   );
        }

    inline long
        p2p_default_interval(
                void
                )
        {
            /* FIXME: tunable with env variable */
            return 1000L;
        }

    inline int
        p2p_default_connect_retry(
                void
                )
        {
            /* FIXME: tunable with env variable */
            return 10;
        }
   inline int
        p2p_default_connection_backlog(
                void
                )
        {
            /* FIXME: tunable with env variable */
            return 10;
        }

#if defined( __cplusplus)
}
#endif
