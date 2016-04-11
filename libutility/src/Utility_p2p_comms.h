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
 */

#ifndef LIB_UTILITY_P2P_COMMS_H
#define LIB_UTILITY_P2P_COMMS_H

#if defined( __cplusplus)
extern "C" {
#endif

#include "libUtility.h"

#define COMM_PORT 6666
#define P2P_CONNECT util_eConnect
#define P2P_ACCEPT  util_eAccept

#define MAKE_IPV4_ADDR(a,b,c,d)  htonl( (((a) & 0xff) <<24) | \
					(((b) & 0xff) <<16) | \
					(((c) & 0xff) << 8) | \
					(((d) & 0xff)) )

#define RETURN_UNLESS_WINSOCK_IS_ALREADY_UP(retval,err) \
    do {                                                \
        if(WSANOTINITIALISED == (err))                  \
        {                                               \
            /* must retry as winsock wasn't up yet, */  \
            if(p2p_os_init())                              \
            {                                           \
                p2p_set_errno(util_eConnAborted);       \
                return (retval);                        \
            }                                           \
            p2p_set_errno(util_eWouldBlock);            \
        }                                               \
    }while(0)

#if(!defined(_WIN32))
#undef RETURN_UNLESS_WINSOCK_IS_ALREADY_UP
#define RETURN_UNLESS_WINSOCK_IS_ALREADY_UP(retval,err)
#endif

#define RECV_WINDOW (8096)  /* must be a power of two */
#define RECV_MASK  ((RECV_WINDOW)-1)
#define RECV_HEAD(head,buf)  ((buf)+(((head) + 1) & (RECV_MASK)))

#define p2p_default_interval_secs() (   (p2p_default_interval() /  1000) )
#define p2p_default_interval_msecs()(   ((p2p_default_interval() %  1000)*1000) )


typedef struct p2p_address_tag p2p_address_t;
typedef struct util_rbuf_tag util_rbuf_t;

typedef int (*p2p_specialise_func_t)
        (
        util_sock_fd_t,
        util_addrinfo_t*
        );


struct p2p_address_tag
{
    util_sock_fd_t    sd_sock;
    socklen_t         sd_len;
    struct sockaddr   sd_addr;
    uint32_t          sd_flags;
    struct timeval    sd_interval;
    int               sd_error;
};


struct util_rbuf_tag
{
    p2p_address_t addr;

    util_iov_t rb_rx_vec;
    util_iov_t rb_tx_vec;

    size_t  rb_tx_bytes;
    size_t  rb_rx_bytes;
    size_t  rb_tx_head;
    size_t  rb_rx_head;

    fd_set rs;
    fd_set ws;
    fd_set es;

    size_t obtained;
    size_t total;
};

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
        );

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
        );

int
p2p_nslookup(
        p2p_address_t *paddr,
        const char *host,
        uint32_t flags,
        ...
        );

int
p2p_getaddrinfo(
        util_addrinfo_t **dest,
        const char *host,
        const char *service,
        uint32_t flags
        );

/* Closes a socket obtained from a call to p2p_open
 *
 * returns 0 on success
 * returns <0 on failure possibly modifying errno
 * may fail for any of the reasons specified for p2p_close_socket()
 * util_eInvalidArguments: invalid arguments*/
int
p2p_close(
        util_sock_fd_t sd
        );

/* Closes a socket obtained from a call to p2p_socket_open
 *
 * returns 0 on success
 * returns <0 on failure possibly modifying errno
 * may fail for any of the reasons specified for shutdown(),close(),
 * util_eInvalidArguments: invalid arguments*/
int
p2p_socket_close(
        util_sock_fd_t sd
        );

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
        );


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
        );

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
        );

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
        );

   int
        p2p_select(
                util_sock_fd_t sd,
                fd_set *rs,
                fd_set *ws,
                fd_set *es,
                int *pnfds,
                int wait
                );

inline int
p2p_ignorable_error(
        const int err
        );

inline uint32_t
p2p_default_flags(
        void
        );

inline long
p2p_default_interval(
        void
        );

inline int
p2p_default_connect_retry(
        void
        );

 inline int
        p2p_default_connection_backlog(
                void
                );

int
p2p_nslookup(
        p2p_address_t *paddr,
        const char *host,
        uint32_t flags,
        ...
        );

/* os specific wrappers */


int
p2p_os_init(
        void
        );

int
p2p_os_init_with_handler(
        void
        );

int
p2p_os_init_without_handler(
        void
        );

void
p2p_os_finit(
        void
        );


int
p2p_os_disconnect(
        util_sock_fd_t sd
        );


/* os resolver interface */
int
p2p_os_getaddrinfo_flags(
        uint32_t *dest_flags,
        util_addrinfo_t *hints
        );

int
p2p_os_getaddrinfo(
        const char *host,
        const char *service,
        util_addrinfo_t *hints,
        util_addrinfo_t **results,
        uint32_t flags
        );

int
p2p_os_select(
        p2p_address_t *addr,
        fd_set *rs,
        fd_set *ws,
        fd_set *es,
        int *pnfds,
        int wait
        );

 int
        p2p_os_recv(
                util_sock_fd_t sd,
                size_t *rx_bytes,
                void    *buf,
                size_t  len,
                int     flags
                );
int
        p2p_os_send(
                util_sock_fd_t sd,
                size_t *tx_bytes,
                void    *buf,
                size_t  len,
                int     flags
                );

int
        p2p_os_writev(
                p2p_address_t *addr,
                util_iov_t *iov,
                int nvecs,
                int flags
                );

  int
        p2p_os_readv(
                p2p_address_t *addr,
                util_iov_t *iov,
                int nvecs,
                int flags
                );

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
    );

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
        );

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
        );

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
        );


/** os socket ioctl interface */

int
p2p_os_socket_ioctl(
    util_sock_fd_t sd,
    int flag,
    uintptr_t value
    );


/** socket ioctls  */

int
p2p_os_socket_close_on_exec(
    util_sock_fd_t sd,
    int flag,
    int close_on_exec
    );

int
p2p_os_socket_nonblock(
    util_sock_fd_t sd,
    int flag,
    int nonblocking
    );

int
p2p_os_socket_nagle(
    util_sock_fd_t sd,
    int flag,
    int skip
    );

int
p2p_os_socket_cork(
    util_sock_fd_t sd,
    int flag,
    int corked
    );

int
p2p_os_socket_push(
    util_sock_fd_t sd,
    int flag,
    int corked
    );

int
p2p_os_socket_reuse_port(
        util_sock_fd_t sd,
        int flag,
        int reuse
        );
int
p2p_os_socket_reuse_addr(
        util_sock_fd_t sd,
        int flag,
        int reuse
        );

int
p2p_os_socket_pending_bytes(
        util_sock_fd_t sd,
        int flag,
        int *dest
        );

int
p2p_os_socket_pending_error(
    util_sock_fd_t sd,
    int flag
    );

int
p2p_os_socket_multicast(
    util_sock_fd_t sd,
    int flag,
    struct in_addr *addr
);

/* Toggle packet coalescing to speed up small packet I/O
 *  util_eSD_PacketCoalesing:
 * returns  0 on success
 * returns -1 failure setting errno for any of the reasons given for setsockopt()
 * util_eInvalidArguments:  sd < 0 */
int
p2p_os_socket_coalesing(
        util_sock_fd_t sd,
        int off
        );

/* Toggle address reuse in the correct way for posix and winsocks broken bsd implementation
 *  util_eSD_AddressReuse
 * returns  0 on success
 * returns -1 failure setting errno for any of the reasons given for setsockopt()
 * util_eInvalidArguments:  sd < 0 */
int
p2p_os_socket_addr_reuse(
        util_sock_fd_t sd,
        int off
        );


#if defined (P2P_OBSOLETE_API)
/* Opens a passive or active connection to the address specified in paddr
 * PASSIVE) paddr and psize are modified to the addresses and address size of the client
 * ACTIVE ) paddr and psize are treated as if they were declared const
 *
 * returns a connected socket descriptor on success
 * returns -1 on failure setting errno for any of the reasons given for
 * connect(),bind(),listen(),accept(),socket(),fcntl(),setsockopt()
 *
 * EINVAL : invalid (NULL ? ) parameters */
util_sock_fd_t
p2p_open(
        struct sockaddr_in *paddr,
        socklen_t *psize,
        int passive
        );

/* Reads 'len' bytes into the buffer 'buf' from socket 'sd'
 *
 * returns number of bytes read on success
 * return <0 on failure, setting errno for any of the reasons given for readv
 * EINVAL: invalid or NULL arguments */
ssize_t
p2p_read(
    util_sock_fd_t sd,
    const void *buf,
    size_t len
    );


/* Writes 'len' bytes from the buffer 'buf' to socket 'sd'
 *
 * returns number of bytes written on success
 * return <0 on failure,
 * setting errno for any of the reasons given for writev
 * EINVAL: invalid or NULL arguments */
ssize_t
p2p_write(
    util_sock_fd_t sd,
    const void *buf,
    size_t len
    );

/* libp2p.c */
/* test/sock_reader.c */
/* test/sock_writer.c */

#endif

#define p2p_set_tcp_sockopt(sd,flag,value) (p2p_os_sockopt_set((sd),IPPROTO_TCP,(flag),(value)))
#define p2p_set_udp_sockopt(sd,flag,value) (p2p_os_sockopt_set((sd),IPPROTO_UDP,(flag),(value)))
#define p2p_set_sol_sockopt(sd,flag,value) (p2p_os_sockopt_set((sd),SOL_SOCKET,(flag),(value)))

int
p2p_os_sockopt_set(
    util_sock_fd_t sd,
    int level,
    int flag,
    int value
    );

int
p2p_os_sockopt_get(
    util_sock_fd_t sd,
    int level,
    int flag,
    int *err_ptr
    );

#if defined( __cplusplus)
}
#endif

#endif /* ndef UTILITY_P2P_COMMS_H */
