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

    /* os specific startup/cleanup sequence mostly to work around window's lack of an always on network stack */
    int
        p2p_os_init(
                void
                )
        {
#if !defined (_WIN32)
            return p2p_os_init_without_handler();
#else
            /* register winsock cleanup proc */
            return p2p_os_init_with_handler();
#endif /* !defined (_WIN32) */
        }


    int
        p2p_os_init_without_handler(
                void
                )
        {
#if defined(_WIN32)
            /* sodding win32 fancy having to tell the os to load sockets, pizz akrap indeed */
            WSADATA wsaData;
            int err;
            /* we lazy initialise so should only get here once only;
             * when call needs retrying on socket usage */
            if(WSANOTINITIALISED == (err = p2p_get_errno()) )
                if(WSAStartup(MAKEWORD(2,0),&wsaData))
                    return util_eOk;

            p2p_set_errno(util_eError);
            return util_eError;
#else
            return 0;
#endif
        }


    /* os specific statup and termination signal handling interface */
    int
        p2p_os_init_with_handler(
                void
                )
        {
#if defined (_WIN32)
            if(!p2p_os_init_with_handler() && (0 == atexit(p2p_os_finit)))
                return util_eOk;

            p2p_os_finit();
            return util_eError;
#elif ! defined (_WIN32)

            return util_eOk;
#endif /* defined (_WIN32) */
        }

    void
        p2p_os_finit(
                void
                )
        {
#if(defined (_WIN32))
            while((0 != WSACleanup()))
            {
                int err = p2p_get_errno();
                if(WSANOTINITIALISED == err)
                {
                    /* should never have been called */
                    p2p_set_errno(util_eError);
                    break;
                }
            }
#endif
        }


    /* socket closing */
    int
        p2p_os_disconnect(
                util_sock_fd_t sd
                )
        {
#if defined(_WIN32) && defined(WSAID_DISCONNECTEX)
            int error = 0;
            GUID guid       = WSAID_DISCONNECTEX;

            /* the prototype and identifier for dl-loading the windows DisconnectEX function */
            LPFN_DISCONNECTEX disconnect_fptr = (LPFN_DISCONNECTEX)NULL;
            DWORD written   = 0;

#endif /* defined(_WIN32) && defined(WSAID_DISCONNECTEX) */

            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd))
                return util_eError;

            /* on POSIX  */
#if !defined(_WIN32)
            return (close((sd))) ? util_eError : util_eOk;
#endif

            /* on windows no DisconnectEx api  present */
#if defined(_WIN32) && !defined(WSAID_DISCONNECTEX)
            return (closesocket((sd))) ? util_eError : util_eOk;
#endif /* defined(_WIN32) && !defined(WSAID_DISCONNECTEX) */

            /* on windows use DisconnectEx api if present */
#if defined(_WIN32) && defined(WSAID_DISCONNECTEX)

            /* bind the DisconnectEx function from wherever it hides */
            error = WSAIoctl( (sd),
                    SIO_GET_EXTENSION_FUNCTION_POINTER,
                    &guid,sizeof(guid),
                    &disconnect_fptr,sizeof(disconnect_fptr),
                    &written,
                    NULL,NULL);

            /* binding the DisconnectEx function failed */
            if(error)
            {
                /* restore the error we just trashed  */
                error = p2p_get_errno();
                closesocket(sd);
                p2p_set_errno(error);
                return util_eError;
            }

            /* invoke the DisconnectEx function */
            if(disconnect_fptr && ( (*disconnect_fptr)( (sd), NULL, 0, 0) ) )
                return !(closesocket) ? util_eOk : util_eError;

            /* DisconnectEx failed, closethe socket anyway but preserve the original error */
            error = p2p_get_errno();
            p2p_set_errno(error);
#endif /* defined(_WIN32) && defined(WSAID_DISCONNECTEX) */
            return util_eError;
        }

    /* os resolver interface */
      int
        p2p_os_getaddrinfo_flags(
                uint32_t *dest_flags,
                util_addrinfo_t *hints
                )
        {
            uint32_t flag = 0;
            p2p_set_errno(util_eInvalidArguments);
            if(!hints || !dest_flags)
                return util_eError;

            /* check address family */
            switch (hints->ai_family)
            {
                case AF_INET:
                    flag |= util_eAI_PreferIPVFour;
                    break;
                case AF_INET6:
                    flag |= util_eAI_PreferIPVSix;
                    break;
                    /* not an ip address family */
                default:
                    flag &= util_eAI_AnyVersion;
            }

            switch (hints->ai_protocol)
            {
                case IPPROTO_TCP:
                    flag |= util_eAI_ByteStream;
                    break;

                case IPPROTO_UDP:
                    flag |= util_eAI_Datagram;
                    break;
                    /* not specified so strip flag bits */

                default:
                    flag &= util_eAI_AnyKind;
                    break;
            }

            switch (hints->ai_flags)
            {
                case AI_PASSIVE:
                    flag &= ~(util_eAI_ActiveOpen);
                    break;
                case AI_CANONNAME:
                    flag |= util_eAI_ActiveOpen;
                default:
                    /* probably a numeric ip addr
                     * should maybe check for this */
                    break;
            }

            switch (hints->ai_socktype)
            {
                case SOCK_STREAM:
                    flag |= util_eAI_ByteStream;
                    break;
                case SOCK_DGRAM:
                    flag |= util_eAI_Datagram;
                    break;
                    /* not specified so strip flag bits */
                default:
                    flag &= util_eAI_AnyKind;
                    break;
            }
            *dest_flags = flag;
            return util_eOk;
        }

      int
        p2p_os_getaddrinfo(
                const char *host,
                const char *service,
                util_addrinfo_t *hints,
                util_addrinfo_t **results,
                uint32_t flags
                )
        {
            int ret;
            p2p_set_errno(util_eInvalidArguments);
            if(!hints || !results )
                return util_eError;

            /* need to provide a way for caller to set platform specific flags */
            hints->ai_flags      = (UTILITY_AI_FLAGS(flags) | hints->ai_flags);
            hints->ai_family     = UTILITY_AI_FAMILY(flags);
            hints->ai_socktype   = UTILITY_AI_SOCK_TYPE(flags);

            if(flags & util_eAI_Datagram)
                hints->ai_protocol = IPPROTO_UDP;

            if(flags & util_eAI_ByteStream)
                hints->ai_protocol = IPPROTO_TCP;

            if(hints->ai_family == AF_INET6)
                host = UTILITY_AI_IPV6_HOSTNAME(flags,host);

            if(hints->ai_family == AF_INET)
                host = UTILITY_AI_IPV4_HOSTNAME(flags,host);

            p2p_set_errno(util_eInvalidArguments);
            if(!host && !service)
                return util_eError;

            /* on posix the errors are returned directly */
            ret = getaddrinfo(host, service, hints, results);
#if defined (_WIN32)
            if(ret)
            {
                /* posix style return of error but don't break windows code */
                ret = p2p_get_errno();
                RETURN_UNLESS_WINSOCK_IS_ALREADY_UP(util_eError,ret);
                p2p_set_errno(ret);
            }
#endif /*defined(_WIN32) */
            return ret;
        }


    int
        p2p_os_select(
                p2p_address_t *addr,
                fd_set *rs,
                fd_set *ws,
                fd_set *es,
                int *pnfds,
                int wait
                )
        {
            int err=0,
            nfds;
            struct timeval tv;

            p2p_set_errno(util_eInvalidArguments);
            if(!addr || !rs || !ws || !es || !pnfds)
                return util_eInvalidArguments;

            /* reset timer */
            tv = addr->sd_interval;

            /* clear out sets, this is slow as shit on windows */
            FD_ZERO(ws);
            FD_ZERO(rs);
            FD_ZERO(es);

            /* set for everything as errorfd get set on windows */
            FD_SET(addr->sd_sock,rs);
            FD_SET(addr->sd_sock,ws);
            FD_SET(addr->sd_sock,es);

            nfds = select(addr->sd_sock+1,rs,ws,es, ((wait) ? &tv : NULL)  );
            addr->sd_error = err = 0;

            if(util_eOk == nfds)
                return util_eTimedOut;

            if(SOCKET_ERROR == nfds)
            {
                err = p2p_get_errno();
                if(p2p_ignorable_error(err))
                    return util_eWouldBlock;

                return err;
            }
            *pnfds = nfds;
            return util_eOk;
        }

    int
        p2p_os_readv(
                p2p_address_t *addr,
                util_iov_t *iov,
                int nvecs,
                int flags
                )
        {
            int nbytes = 0;
            int err,nfds,would_wrap;
            size_t nfree;

            /* fast path consume existing bytes from fifo never leave userspace */
            util_rbuf_t *rbuf = (util_rbuf_t *)addr;
            rbuf->rb_rx_vec = *iov++;
            while(rbuf->obtained < rbuf->rb_rx_vec.iov_len)
            {
                /* check if there is more of this buffer to gather */
                nbytes = ((rbuf->rb_rx_bytes > rbuf->rb_rx_vec.iov_len) ?
                                (rbuf->rb_rx_bytes - rbuf->rb_rx_vec.iov_len) :
                                rbuf->rb_rx_bytes);

                /* copy over any data from previous iteration */
                if(nbytes)
                {
                    /* fill up this buffer prior to shifting to next */
                    memcpy(rbuf->rb_rx_vec.iov_base+rbuf->obtained, RECV_HEAD(rbuf->rb_rx_head,rbuf->rb_tx_vec.iov_base), nbytes);
                    rbuf->obtained      += nbytes;
                    rbuf->rb_rx_bytes   -= nbytes;
                    if(rbuf->obtained == rbuf->rb_rx_vec.iov_len)
                    {
                        rbuf->total  += rbuf->obtained;
                        if(--nvecs)
                        {
                            rbuf->obtained = 0;
                            rbuf->rb_rx_vec = *iov++;
                            nbytes = 0;
                        }
                        /* we're done */
                        else
                            return util_eOk;
                    }
                }

                /* wait */
                switch( (err=p2p_os_select(addr,&rbuf->rs,&rbuf->ws,&rbuf->es,&nfds,1)))
                {
                    case util_eWouldBlock:      /* fall through */
                    case util_eTimedOut:        /* fall through */
                        err = util_eWouldBlock; /* fall through */
                    default:
                        p2p_set_errno(err);
                        return util_eError;

                    case util_eOk:
                        if(FD_ISSET(addr->sd_sock,&rbuf->es))
                            goto handle_error;
                }

                /* check if there is anything to read and sufficent space in the buffer for all the pending data */
                if(FD_ISSET(addr->sd_sock,&rbuf->rs) &&
                        !p2p_os_socket_ioctl(addr->sd_sock,util_eSD_PendingBytes,(uintptr_t)&rbuf->rb_tx_bytes) &&
                        rbuf->rb_tx_bytes)
                {
                    nbytes = (rbuf->rb_rx_bytes+rbuf->rb_tx_bytes+1);
                    nfree  = rbuf->rb_tx_vec.iov_len;
                    would_wrap = ( (nfree - nbytes) > nfree);
                    if(would_wrap)
                    {
                        /* reduce read size to mamimum of buffer */
                        nfree = nbytes - nfree;
                        nbytes -= nfree;
                    }
                    /* nothing to read because nothing because buffer is full*/
                    if(!nbytes)
                        break;

                    FD_CLR(addr->sd_sock,&rbuf->rs);
                    /* got some data, see if that fills the user buffer, with a read nbytes into the buffer window */
                    nbytes = recv(  addr->sd_sock, RECV_HEAD(rbuf->rb_tx_head,rbuf->rb_tx_vec.iov_base), nbytes, flags );
                    if(SOCKET_ERROR != nbytes && nbytes > 0)
                    {
                        rbuf->rb_rx_bytes += nbytes;
                        continue;
                    }

                    /* connection was reset */
                    if(0 == nbytes)
                    {
                        p2p_set_errno(util_eConnReset);
                        return util_eError;
                    }

                    /* nothing to read because nothing because buffer is full*/
handle_error:
                    /* get and clear pending error */
                    err = p2p_get_errno();
                    if(!p2p_os_socket_ioctl(addr->sd_sock,util_eSD_PendingError,((uintptr_t)&nbytes)) && (err != nbytes))
                        err = nbytes;

                    FD_CLR(addr->sd_sock,&rbuf->es);
                    if(util_eInterupted == err)
                        continue;

                    if(util_eWouldBlock != err)
                    {
                        p2p_set_errno(err);
                        rbuf->rb_rx_bytes = rbuf->obtained;
                        return util_eError;
                    }
                    return util_eError;
                }
            }
                /* success */
                return util_eOk;
        } /* p2p_os_readv() */

    int
        p2p_os_writev(
                p2p_address_t *addr,
                util_iov_t *iov,
                int nvecs,
                int flags
                )
        {
            int nbytes = 0;
            int err,nfds;

            /* walk through passed vectors counting how much data to write */

            util_rbuf_t *rbuf = (util_rbuf_t *)addr;
            rbuf->rb_rx_vec = *iov++;

            rbuf->total = rbuf->obtained =0 ;
            for(nfds = 0; nfds < nvecs; ++nfds)
            {
                rbuf->total += iov[nfds].iov_len;
            }

            /* poll  */
            while(rbuf->obtained < rbuf->total )
            {
                /* poll so that we can actually do some work while socket becomes writable */
                err = p2p_os_select(addr,&rbuf->rs,&rbuf->ws,&rbuf->es,&nfds,!nvecs);
                switch( err)
                {

                    /* gather the contents of the iov structures into a logical record
                     * using a ring buffer as FIFO */
                    case util_eWouldBlock:
                    case util_eTimedOut:
                        if( ((rbuf->rb_tx_bytes) +  (rbuf->rb_rx_vec.iov_len)) < rbuf->rb_tx_vec.iov_len)
                        {
                            memcpy(
                                    RECV_HEAD(rbuf->rb_tx_head,rbuf->rb_tx_vec.iov_base),
                                    rbuf->rb_rx_vec.iov_base,
                                    rbuf->rb_rx_vec.iov_len
                                  );
                            rbuf->obtained      +=  rbuf->rb_rx_vec.iov_len;
                            if(nvecs)
                            {
                                rbuf->rb_rx_vec = *iov++;
                                --nvecs;
                            }
                        }
                        /* wait for our slot to write */
                        err = p2p_os_select(addr,&rbuf->rs,&rbuf->ws,&rbuf->es,&nfds,!nvecs);

                    default:
                        /* some thing is wrong here */
                        if(FD_ISSET(addr->sd_sock,&rbuf->es))
                        {
                            /* get and clear pending error */
                            err = p2p_get_errno();

                            if(!p2p_os_socket_ioctl(addr->sd_sock,
                                        util_eSD_PendingError,
                                        ((uintptr_t)&nbytes)) && (err != nbytes))
                            {
                                err = nbytes;
                            }
                            /* we've handled the errors so clear the notification */
                            FD_CLR(addr->sd_sock,&rbuf->es);
                            if(util_eInterupted == err)
                                continue;

                            if(util_eWouldBlock != err)
                            {
                                p2p_set_errno(err);
                                rbuf->rb_rx_bytes = rbuf->obtained;
                                return util_eError;
                            }
                        }

                        if(FD_ISSET(addr->sd_sock,&rbuf->ws))
                        {
                            if(! rbuf->total)
                                continue;

                            /* filled send buffer,
                             * write entire buffer window as one shot, this may not be all the
                             * structures at once if the ringbuffer is overloaded */
                            nbytes = send( addr->sd_sock,
                                    RECV_HEAD(rbuf->rb_rx_head,rbuf->rb_tx_vec.iov_base),
                                    rbuf->rb_tx_bytes, flags );
                            FD_CLR(addr->sd_sock,&rbuf->ws);
                            if(SOCKET_ERROR != nbytes && nbytes > 0)
                            {
                                rbuf->rb_tx_bytes -= nbytes;
                                continue;
                            }

                            /* connection was reset */
                            if(0 == nbytes)
                            {
                                p2p_set_errno(util_eConnReset);
                                return util_eError;
                            }
                        }
                }
            }
            /* success */
            return util_eOk;
        }
    int
        p2p_os_recv(
                util_sock_fd_t sd,
                size_t *rx_bytes,
                void    *buf,
                size_t  len,
                int     flags
                )
        {
            int nbytes = 0;
            int err;
            size_t obtained = 0;

            switch(obtained < len)
            {
                default: do {
                             nbytes = recv(sd, buf+obtained,len-obtained,flags);
                             if(0 == nbytes)
                             {
                                 /* connection was reset */
                                 p2p_set_errno(util_eConnReset);
                                 *rx_bytes = obtained;
                                 return util_eError;
                             }

                             /* on posix the two conditions are the same and should get folded into one  test */
                             if(SOCKET_ERROR == nbytes || 0>nbytes)
                             {
                                 nbytes = 0;
                                 err = p2p_get_errno();

                                 /* get and clear pending error */
                                 p2p_os_socket_ioctl(sd,util_eSD_PendingError,((uintptr_t)&nbytes));

                                 /* these might not be the same value if another error happened */
                                 if(err != nbytes)
                                     err = nbytes;

                                 if(util_eInterupted == err || util_eWouldBlock == err)
                                     continue;

                                 p2p_set_errno(err);
                                 *rx_bytes = obtained;
                                 return util_eError;
                             }
                             /* fall through */
                             case util_eOk:          if(obtained+nbytes == len)
                                                     {
                                                         *rx_bytes = obtained+nbytes;
                                                         break; /* jump out of duffs device */
                                                     }
                                                     obtained  += nbytes;
                                                     continue;
                         }while(1);
                             break; /* jump out of switch only get here on successful read of entire buffer */
            }
            /* success */
            return util_eOk;
        }


    int
        p2p_os_send(
                util_sock_fd_t sd,
                size_t *tx_bytes,
                void    *buf,
                size_t  len,
                int     flags
                )
        {
            int nbytes = 0;
            int err;
            size_t transmited = 0;

            switch(transmited < len)
            {
                default: do {
                             nbytes = send(sd, buf+transmited,len-transmited,flags);
                             if(0 == nbytes)
                             {
                                 /* connection was reset */
                                 p2p_set_errno(util_eConnReset);
                                 *tx_bytes = transmited;
                                 return util_eError;
                             }

                             /* on posix the two conditions are the same and should get folded into one  test */
                             if(SOCKET_ERROR == nbytes || 0>nbytes)
                             {
                                 nbytes = 0;
                                 err = p2p_get_errno();

                                 /* get and clear pending error */
                                 p2p_os_socket_ioctl(sd,util_eSD_PendingError,((uintptr_t)&nbytes));

                                 /* these might not be the same value if another error happened */
                                 if(err != nbytes)
                                     err = nbytes;

                                 if(util_eInterupted == err || util_eWouldBlock == err)
                                     continue;

                                 p2p_set_errno(err);
                                 *tx_bytes = transmited;
                                 return util_eError;
                             }
                             /* fall through */
                             case util_eOk:          if(transmited+nbytes == len)
                                                     {
                                                         *tx_bytes = transmited+nbytes;
                                                         return util_eOk; /* jump out of duffs device */
                                                     }
                                                     transmited  += nbytes;
                         }while(transmited < len);
                             break; /* jump out of switch only get here on successful read of entire buffer */
            }
            /* success */
            return util_eOk;
        }



    /* Get os socket descriptor
     *
     * returns 0 on success
     * returns <0 on failure possibly modifying errno
     * may fail for many reasons
     * */
    int
        p2p_os_sock_fd(
                util_sock_fd_t *dest,
                int domain,
                int type,
                int protocol
                )
        {
            util_sock_fd_t sd;
            p2p_set_errno(util_eInvalidArguments);
            if(!dest)
                return util_eError;

#if ! defined(_WIN32)
            sd = socket(domain,type,protocol);
#else
            sd = WSASocket(domain,type,protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
#endif
            if(utility_socket_invalid(sd))
            {
                domain = p2p_get_errno();
                RETURN_UNLESS_WINSOCK_IS_ALREADY_UP(util_eError,domain);
            }
            else
            {
                *dest = sd;
                p2p_set_errno(util_eOk);
                return util_eOk;
            }
            return util_eError;
        }

    /* Get os socket descriptor specialised by callback
     *
     * returns 0 on success
     * returns <0 on failure possibly modifying errno
     * may fail for many reasons
     * */
    int
        p2p_os_socket(
                util_sock_fd_t *dest_sd,
                util_addrinfo_t *node,
                p2p_specialise_func_t specialise
                )
        {
            int err;
            util_sock_fd_t sd;

            p2p_set_errno(util_eInvalidArguments);
            if(!dest_sd || !node || !specialise )
                return util_eError;

            do
            {
                if(util_eOk != (err = p2p_os_sock_fd(&sd,node->ai_family,node->ai_socktype,node->ai_protocol)))
                    err = p2p_get_errno();

            }while(util_eWouldBlock == err);

            if(err || utility_socket_invalid(sd) )
                return util_eError;

            /* yield to caller */
            switch((*specialise)(sd,node))
            {
                case util_eOk:
                    *dest_sd = sd;
                    return util_eOk;

                    /* only get here if something went wrong */
                default: /* fallthrough */
                case util_eError:
                    err = p2p_get_errno();
                    break;
            }

            /* only get here if something went wrong */
            p2p_socket_close(sd);
            p2p_set_errno(err);
            return util_eError;
        }

    /* Specialised os file descriptor for passive open (listening sockets)
     *
     * returns 0 on success
     * returns <0 on failure possibly modifying errno
     * may fail for many reasons
     * */
    int
        p2p_os_passive_open(
                util_sock_fd_t sd,
                util_addrinfo_t *node
                )
        {
            int err;
            p2p_set_errno(util_eInvalidArguments);
            if(!node || utility_socket_invalid(sd))
                return util_eError;

            /* listening sockets need to have passive flag set */
            if(0 == (node->ai_flags & AI_PASSIVE))
                return util_eError;

            /* want non-blocking sockets */
            if(!p2p_os_socket_ioctl(sd,util_eFD_NonBlocking,1))
                return util_eError;


            /* disable packet coalescing */
            if(!p2p_os_socket_ioctl(sd,util_eSD_PacketCoalesing,1))
                return util_eError;

            /* make socket port reusable needed for bsd  */
            if(0>p2p_os_socket_ioctl(sd,util_eSD_ReusePort,1))
            {
                /* this only meaningful on bsd so dont wory about
                 * lack of support on other platforms */
                err = p2p_get_errno();
                if(util_eOpNotSupp != err)
                    return util_eError;
            }

            /* make socket address reusable including on windows's broken stack  */
            if(0>p2p_os_socket_ioctl(sd,util_eSD_ReuseAddress,1))
            {
                /* this only really meaningful on posix so dont wory about
                 * lack of support on windows, it does the right thing according to stevens and the interweb */
                err = p2p_get_errno();
                if(util_eOpNotSupp != err)
                    return util_eError;
            }

            if(SOCKET_ERROR == (bind(sd,node->ai_addr,node->ai_addrlen)) || listen(sd,p2p_default_connection_backlog()))
                return util_eError;

            return util_eOk;
        }

    /* Specialised os file descriptor for active open (connecting sockets)
     *
     * returns 0 on success
     * returns <0 on failure possibly modifying errno
     * may fail for many reasons
     * */
    int
        p2p_os_active_open(
                util_sock_fd_t sd,
                util_addrinfo_t *node
                )
        {
            int err;
            int retry,nfds;

            fd_set readfds,
                   writefds,
                   errorfds;

            /* disable packet coalescing */
            if(!p2p_os_socket_ioctl(sd,util_eSD_PacketCoalesing,1))
                return util_eError;

            /* want non-blocking sockets */
            if(!p2p_os_socket_ioctl(sd,util_eFD_NonBlocking,1))
                return util_eError;

            /* non-blocking connect, should fail with EINPROGRESS */
            err = connect(sd, node->ai_addr, node->ai_addrlen);
            if(!err)
                return util_eOk;

            /* retry until timer expires or itervals */
            for(retry = p2p_default_connect_retry(); retry;)
            {
                switch( (err = p2p_select(sd,&readfds,&writefds,&errorfds, &nfds,1)))
                {
                    /*
                     *   If processing a connect call (nonblocking), connection attempt failed.
                     *   OOB data is available for reading (only if SO_OOBINLINE is disabled).
                     * http://msdn.microsoft.com/en-us/library/ms740141%28VS.85%29.aspx
                     */
                    case util_eOk:
                        if(util_eConnect == p2p_os_socket_ioctl( sd, util_eSD_PendingError,(uintptr_t)&err) && util_eConnect == err)
                            return util_eOk;

                        --retry;
                        continue;

                    case util_eTimedOut:
                        p2p_set_errno(util_eWouldBlock);
                        return util_eError;

                    default:
                        if(!p2p_ignorable_error(err))
                        {
                            p2p_set_errno(err);
                            return util_eError;
                        }

                        /* Windows and other BSD derived stacks,
                         * return 0 on success modifying err to indicate connection errors */
                        if(FD_ISSET(sd,&errorfds) || (FD_ISSET(sd,&readfds)) ||  (FD_ISSET(sd,&writefds)))
                        {
                            /* check for error on connecting discriptor */
                            if(util_eConnect == p2p_os_socket_ioctl( sd, util_eSD_PendingError,(uintptr_t)&err) && util_eConnect != err)
                            {
                                p2p_set_errno(util_eConnAborted);
                                return util_eError;
                            }
                            return util_eOk;
                        }
                }

                return util_eError;
            }
            p2p_set_errno(util_eConnAborted);
            return util_eError;
        }


    /* Set/Get socket options
     *
     * returns 0 on success
     * returns <0 on failure possibly modifying errno
     * may fail for many reasons
     * */
    int
        p2p_os_socket_ioctl(
                util_sock_fd_t sd,
                int flag,
                uintptr_t value
                )
        {
            int err;
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd))
                return util_eError;

            switch(flag)
            {
                /* close on exec flag */
                case util_eFD_CloseOnExec:
                    return p2p_os_socket_close_on_exec(sd,flag,(int)value);

                    /* non blocking i/o */
                case util_eFD_NonBlocking:
                    return p2p_os_socket_nonblock(sd,flag,(int)value);


                    /* nagle algorithm, used for packet coalescing */
                case util_eSD_TcpNoDelay:
                    return p2p_os_socket_nagle(sd,flag,(int)value);

                    /* corking used for in kernel packet coalescing */
                case util_eSD_TcpCork:
                        return p2p_os_socket_cork(sd,flag,(int)value);

                    /* corking used for in kernel packet coalescing */
                case util_eSD_TcpPush:
                        return p2p_os_socket_push(sd,flag,(int)value);

                    /* bsd implied on other platforms */
                case util_eSD_ReusePort:
                    return p2p_os_socket_reuse_port(sd,flag,(int)value);

                    /* ignored on windows due to winsock being braindamaged */
                case util_eSD_ReuseAddress:
                    return p2p_os_socket_reuse_addr(sd,flag,(int)value);

                    /* pending socket error */
                case util_eSD_PendingError:
                    if( ((int *)value))
                    {
                        err = p2p_os_socket_pending_error(sd,flag);
                        *((int *)value) = err;
                        return util_eOk;
                    }
                    p2p_set_errno(util_eInvalidArguments);
                    return util_eError;

                    /* bytes readable in a single call to recv */
                case util_eSD_PendingBytes:
                    if( ((int *)value))
                        return p2p_os_socket_pending_bytes(sd,flag,((int *)value));
                    p2p_set_errno(util_eInvalidArguments);
                    return util_eError;


                case util_eIP_MulticastInterface:
                    return p2p_os_socket_multicast(sd,flag,(struct in_addr*)value);

                case util_eSD_PacketCoalesing:
                    return p2p_os_socket_coalesing(sd,(int)value);

                default:
                    break;
            }
            p2p_set_errno(util_eOpNotSupp);
            return util_eError;
        }

    /* implementation of various socket options handled by p2p_os_socket_ioctl() */
    int
        p2p_os_socket_close_on_exec(
                util_sock_fd_t sd,
                int flag,
                int close_on_exec
                )
        {
            util_non_block_ioctl_t opts;
            int err =  util_eOpNotSupp;

            /* sanity check */
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || (util_eFD_CloseOnExec != flag))
                return util_eError;

            p2p_set_errno(util_eOpNotSupp);

            /* posix method recommended in stevens */
#if(( !defined(_WIN32) && defined(F_SETFD) && defined(F_GETFD) && defined(FD_CLOEXEC)))
            flag = FD_CLOEXEC;
            /* the UNPv2 blessed rune for non-blocking unix */
            err = ((0 > (opts = fcntl(sd,F_GETFL, 0)))) ? p2p_get_errno() : 0 ;
            opts = ((close_on_exec) ? (opts & (~flag)) : ((opts) | flag));
            err = (!err && (0>fcntl(sd, F_SETFL,opts))) ? p2p_get_errno() : err ;

            /* Stevens UNPv2 recommends this for systems defined F_GETFD but not a FD_CLOEXEC constant */
#elif(( !defined(_WIN32) && defined(F_SETFD) && defined(F_GETFD) && !defined(FD_CLOEXEC)))
            err = (!err && (0>fcntl(sd, F_SETFD,close_on_exec))) ? p2p_get_errno() : err ;

            /* windows method from msdn */
#elif ( defined(_WIN32) && defined(HANDLE_FLAG_INHERIT))
            err = (GetHandleInformation((HANDLE)sd,&opts)) ? GetLastError() : 0;
            opts = ((close_on_exec ? (opts & (~flag)) : ((opts) | flag)));
            if(err)
            {
                RETURN_UNLESS_WINSOCK_IS_ALREADY_UP(util_eError,err);
            }
            err = (!err && (SetHandleInformation((HANDLE)sd,HANDLE_FLAG_INHERIT,opts))) ? GetLastError() : err;
#endif
            return (err) ? util_eError : util_eOk;
        }

    int
        p2p_os_socket_nonblock(
                util_sock_fd_t sd,
                int flag,
                int nonblocking
                )
        {
            util_non_block_ioctl_t opts;
            int err =  util_eOpNotSupp;

            /* sanity check */
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || (util_eFD_NonBlocking != flag))
                return util_eError;

            p2p_set_errno(util_eOpNotSupp);
            /* posix method recommended in stevens */
#if(( !defined(_WIN32) && defined(F_SETFL) && defined(F_GETFL) && defined(O_NONBLOCK)))
            flag = O_NONBLOCK;
            /* bsd method from openssl */
#elif(( !defined(_WIN32) && defined(F_SETFL) && defined(F_GETFL) && defined(FNDELAY)))
            flag = O_FNDELAY;

            /* bsd method from apache */
#elif(( !defined(_WIN32) && defined(F_SETFL) && defined(F_GETFL) && defined(O_NDELAY)))
            flag = O_NDELAY;
#endif

#if(( !defined(_WIN32) && defined(F_SETFL) && defined(F_GETFL)))
            /* the UNPv2 blessed rune for non-blocking unix */
            err = ((0 > (opts = fcntl(sd,F_GETFL, 0)))) ? p2p_get_errno() : 0 ;
            opts = ((nonblocking) ? (opts & (~flag)) : ((opts) | flag));
            err = (!err && (0>fcntl(sd, F_SETFL,opts))) ? p2p_get_errno() : err ;
            /* windows method from msdn */

#elif( defined(_WIN32) && defined(FIONBIO))
            opts = (0 != nonblocking);
            err = ioctlsocket(sd,FIONBIO,&opts);
            if(err)
            {
                err = p2p_get_errno();
                RETURN_UNLESS_WINSOCK_IS_ALREADY_UP(util_eError,err);
            }

            /* use this on other platforms with from ssh/apache/and openssl
             * see http://h71000.www7.hp.com/wizard/wiz_1066.html */
#elif (defined(FIONBIO))
            opts = (0 != nonblocking);
            err = ioctl(sd, FIONBIO, &opts) ? p2p_get_errno() : 0;

            /* other socket implementations from apache (BEOS) */
#elif defined(SO_NONBLOCK)
            err  = p2p_set_sol_sockopt(sd,SO_NONBLOCK,value);
#endif
            return (err) ? util_eError : util_eOk;
        }

    int
        p2p_os_socket_nagle(
                util_sock_fd_t sd,
                int flag,
                int skip
                )
        {
            /* sanity check */
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || (util_eSD_TcpNoDelay != flag))
                return util_eError;

#if(defined TCP_NODELAY)
            return p2p_set_tcp_sockopt(sd, TCP_NODELAY, skip);
#endif

            p2p_set_errno(util_eOpNotSupp);
            return util_eError;
        }

    int
        p2p_os_socket_cork(
                util_sock_fd_t sd,
                int flag,
                int corked
                )
        {
            /* sanity check */
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || (util_eSD_TcpCork != flag))
                return util_eError;

#if(defined TCP_CORK)
            return p2p_set_tcp_sockopt(sd, TCP_CORK, corked);
#endif

            /* shut the compiler up on platforms where this is unimplemented */
            corked = util_eOpNotSupp;
            p2p_set_errno(corked);
            return util_eError;
        }

    int
        p2p_os_socket_push(
                util_sock_fd_t sd,
                int flag,
                int corked
                )
        {
            /* sanity check */
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || (util_eSD_TcpPush != flag))
                return util_eError;

#if(defined TCP_PUSH)
            return p2p_set_tcp_sockopt(sd, TCP_PUSH, corked, sizeof(corked));
#endif
            /* shut the compiler up on platforms where this is unimplemented */
            corked = util_eOpNotSupp;
            p2p_set_errno(corked);
            return util_eError;
        }


    int
        p2p_os_socket_reuse_port(
                util_sock_fd_t sd,
                int flag,
                int reuse
                )
        {
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || util_eSD_ReusePort != flag)
                return util_eError;

#if defined(SO_REUSEPORT) && !defined(_WIN32)
            return p2p_set_tcp_sockopt(sd, SO_REUSEADDR, reuse);
#endif
            /* shut the compiler up on platforms where this is unimplemented */
            reuse = util_eOpNotSupp;
            p2p_set_errno(reuse);
            return util_eError;
        }

    int
        p2p_os_socket_reuse_addr(
                util_sock_fd_t sd,
                int flag,
                int reuse
                )
        {
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || util_eSD_ReuseAddress != flag)
                return util_eError;

            /* this is really broken under windows */
#if defined(SO_REUSEADDR) && !defined(_WIN32)
            return p2p_set_tcp_sockopt(sd, SO_REUSEADDR, reuse);

#elif (defined(SO_REUSEADDR)  && defined(_WIN32))
            /* this is default behaviour in windows screwy as it seems
             * SO_REUSEADDR exclusive seems to be the thing
             * but a socket library requiring root to run is crap */
            return util_eOk;
#endif
            p2p_set_errno(util_eOpNotSupp);
            return util_eError;
        }

    int
        p2p_os_socket_pending_bytes(
                util_sock_fd_t sd,
                int flag,
                int *dest
                )
        {
            int nbytes,
                err,
                retry=5;
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || util_eSD_PendingBytes != flag)
                return util_eError;

            /* retrive number of pending bytes spinning a few times to allow for
             * latency in between a socket being readable and the flag being set.
             *
             * Solaris is supposed to suffer from this issue */
#if defined(FIONREAD)
            nbytes = err = 0;
            switch(err)
            {
#if defined(_WIN32)
                u_long arg=0;
#endif
                case 0: do {
                            if(nbytes > 0 || 0 == --retry )
                            {
                                *dest = nbytes;
                                return util_eOk;
                            }

                            nbytes = 0;
#if defined(_WIN32)
                            /* windows */
                            err = ioctlsocket( sd, FIONREAD, &arg);
                            nbytes  =  (arg > INT_MAX) ? INT_MAX : (int)arg;
#else /* Posix stuff */

                            /* posix */
                            err = ioctl( sd, FIONREAD, &nbytes );
#endif
                            continue;
                            default:
                            err = nbytes = 0;
                            continue;

                        }while(1);
            }
#endif /* ndef FIONREAD */
            p2p_set_errno(util_eOpNotSupp);
            return err;
        }


    int
        p2p_os_socket_pending_error(
                util_sock_fd_t sd,
                int flag
                )
        {
            int err;
            int len = sizeof(int);
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || util_eSD_PendingError != flag)
                return util_eError;

            /* grab and clear the pending socket error  */
            err = util_eOpNotSupp;
#if defined(SO_ERROR)
            err = p2p_os_sockopt_get(sd,SOL_SOCKET,SO_ERROR,&len);
            /* clear pending socket error as we've consumed it and
             * it makes behavior consitenet, poissibly incorrect but consitenet*/
            len = sizeof(int);
            flag = 0;
            err = p2p_os_sockopt_set(sd,SOL_SOCKET,SO_ERROR,0);
#endif
            return err;
        }

    int
        p2p_os_socket_multicast(
                util_sock_fd_t sd,
                int flag,
                struct in_addr *addr
                )
        {
            int err;
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd) || util_eIP_MulticastInterface != flag)
                return util_eError;

            /* enable ip multicasting using addr as the local iface */
            err = util_eOpNotSupp;
#if defined(IP_MULTICAST_IF)
            err = setsockopt(sd,IPPROTO_IP, IP_MULTICAST_IF,(char *)addr,sizeof(*addr));
#endif
            return err;
        }

    /* Toggle packet coalescing to speed up small packet I/O
     *
     * returns  0 on success
     * returns -1 failure setting errno for any of the reasons given for setsockopt()
     * util_eInvalidArguments:  sd < 0 */
    int
        p2p_os_socket_coalesing(
                util_sock_fd_t sd,
                int off
                )
        {
            int opts;

            /* sanity check */
            p2p_set_errno(util_eInvalidArguments);
            if(utility_socket_invalid(sd))
                return util_eError;

            /* want exactly 1 for some systems other are happy with non-zero */
            off = (off) ? 1 : 0;

#if (defined TCP_CORK)
            /* enable sending of partial frames to avoid packet coalessing */
            opts = !off;
            if(0>p2p_os_socket_ioctl(sd,util_eSD_TcpCork,opts) && util_eOpNotSupp != p2p_get_errno())
                return util_eError;
#endif
#if (defined TCP_PUSH)
            /* sending of partial frames to avoid packet coalessing */
            opts = !off;
            if(0>p2p_os_socket_ioctl(sd,util_eSD_TcpPush,opts) && util_eOpNotSupp != p2p_get_errno())
                return util_eError;
#endif

            /* nagling */
#if (defined TCP_NODELAY)
            opts = off;
            if(0>p2p_os_socket_ioctl(sd,util_eSD_TcpNoDelay,opts) && util_eOpNotSupp != p2p_get_errno())
                return util_eError;
#endif
            return util_eOk;
        }

int
p2p_os_sockopt_set(
    util_sock_fd_t sd,
    int level,
    int flag,
    int value
    )
{
    int err  = util_eOpNotSupp;
    socklen_t slen = sizeof(err);

    p2p_set_errno(util_eInvalidArguments);
    if(utility_socket_invalid(sd))
        return util_eInvalidArguments;

    err = util_eOpNotSupp;
    p2p_set_errno(err);

#if defined(_WIN32)
    err = setsockopt(sd,level,flag,(char *)&value,(int)slen);
#else
    err = setsockopt(sd,level,flag,&value,slen);
#endif

    return err;
}



int
p2p_os_sockopt_get(
    util_sock_fd_t sd,
    int level,
    int flag,
    int *err_ptr
    )
{
    int err;
    int value = util_eOpNotSupp;
    socklen_t slen = sizeof(err);

    p2p_set_errno(util_eInvalidArguments);
    if(utility_socket_invalid(sd) || !err_ptr)
        return util_eInvalidArguments;

#if defined(_WIN32)
    *err_ptr = getsockopt(sd,level,flag,(char *)&value,(int *)&slen);
#else
    *err_ptr = getsockopt(sd,level,flag,&value,&slen);
#endif
    return value;
}


#if defined( __cplusplus)
}
#endif
