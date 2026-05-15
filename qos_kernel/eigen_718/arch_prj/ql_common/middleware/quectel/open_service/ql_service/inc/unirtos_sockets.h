
/**  @file
  quec_socket.h

  @brief
  TODO

*/

/*================================================================
  Copyright (c) 2022 Quectel Wireless Solution, Co., Ltd.  All Rights Reserved.
  Quectel Wireless Solution Proprietary and Confidential.
=================================================================*/
/*=================================================================

                        EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

WHEN              WHO         WHAT, WHERE, WHY
------------     -------     -------------------------------------------------------------------------------

=================================================================*/


#ifndef _UNIRTOS_LWIP_SOCKET_H_
#define _UNIRTOS_LWIP_SOCKET_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "lwip/api.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"
#include "lwip/ip4_addr.h"
#include "lwip/ip_addr.h"
#include "lwip/inet.h"

#include "lwip/udp.h"
#include "lwip/dns.h"
#include "lwip/inet_chksum.h"


extern int lwip_accept(int s, struct sockaddr *addr, socklen_t *addrlen);
typedef int (* _api_lwip_accept_t)(int s, struct sockaddr *addr, socklen_t *addrlen);

extern int lwip_bind(int s, const struct sockaddr *name, socklen_t namelen);
typedef int (* _api_lwip_bind_t)(int s, const struct sockaddr *name, socklen_t namelen);

extern int32_t lwip_getAckedSize(int nSocket);
typedef int32_t (* _api_lwip_getAckedSize_t)(int nSocket);

extern int lwip_bind_cid(int s, u8_t cid);
typedef int (* _api_lwip_bind_cid_t)(int s, u8_t cid);

extern int lwip_shutdown(int s, int how);
typedef int (* _api_lwip_shutdown_t)(int s, int how);

extern int lwip_close(int s);
typedef int (* _api_lwip_close_t)(int s);

extern int lwip_connect(int s, const struct sockaddr *name, socklen_t namelen);
typedef int (* _api_lwip_connect_t)(int s, const struct sockaddr *name, socklen_t namelen);

extern int lwip_listen(int s, int backlog);
typedef int (* _api_lwip_listen_t)(int s, int backlog);

extern int lwip_recv(int s, void *mem, size_t len, int flags);
typedef int (* _api_lwip_recv_t)(int s, void *mem, size_t len, int flags);

extern int lwip_read(int s, void *mem, size_t len);
typedef int (* _api_lwip_read_t)(int s, void *mem, size_t len);

extern int lwip_recvfrom(int s, void *mem, size_t len, int flags,struct sockaddr *from, socklen_t *fromlen);
typedef int (* _api_lwip_recvfrom_t)(int s, void *mem, size_t len, int flags,struct sockaddr *from, socklen_t *fromlen);

extern int lwip_send(int s, const void *dataptr, size_t size, int flags);
typedef int (* _api_lwip_send_t)(int s, const void *dataptr, size_t size, int flags);

extern int lwip_sendmsg(int s, const struct msghdr *message, int flags);
typedef int (* _api_lwip_sendmsg_t)(int s, const struct msghdr *message, int flags);

extern int lwip_sendto(int s, const void *dataptr, size_t size, int flags,const struct sockaddr *to, socklen_t tolen);
typedef int (* _api_lwip_sendto_t)(int s, const void *dataptr, size_t size, int flags,const struct sockaddr *to, socklen_t tolen);

extern int lwip_socket(int domain, int type, int protocol);
typedef int (* _api_lwip_socket_t)(int domain, int type, int protocol);

extern int lwip_socket_with_call_back(int domain, int type, int protocol, socket_callback callback);
typedef int (* _api_lwip_socket_with_call_back_t)(int domain, int type, int protocol, socket_callback callback);

extern int lwip_write(int s, const void *dataptr, size_t size);
typedef int (* _api_lwip_write_t)(int s, const void *dataptr, size_t size);

extern int lwip_writev(int s, const struct iovec *iov, int iovcnt);
typedef int (* _api_lwip_writev_t)(int s, const struct iovec *iov, int iovcnt);

extern int lwip_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,struct timeval *timeout);
typedef int (* _api_lwip_select_t)(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,struct timeval *timeout);

extern int lwip_ioctl(int s, long cmd, void *argp);
typedef int (* _api_lwip_ioctl_t)(int s, long cmd, void *argp);

extern int lwip_fcntl(int s, int cmd, int val);
typedef int (* _api_lwip_fcntl_t)(int s, int cmd, int val);

extern struct hostent *lwip_gethostbyname(const char *name);
typedef struct hostent *(* _api_lwip_gethostbyname_t)(const char *name);

extern int lwip_gethostbyname_r(const char *name, struct hostent *ret, char *buf,size_t buflen, struct hostent **result, int *h_errnop);
typedef int (*_api_lwip_gethostbyname_r_t)(const char *name, struct hostent *ret, char *buf,size_t buflen, struct hostent **result, int *h_errnop);

extern void lwip_freeaddrinfo(struct addrinfo *ai);
typedef void (*_api_lwip_freeaddrinfo_t)(struct addrinfo *ai);

extern int lwip_getaddrinfo(const char *nodename,const char *servname,const struct addrinfo *hints,struct addrinfo **res);
typedef int (*_api_lwip_getaddrinfo_t)(const char *nodename,const char *servname,const struct addrinfo *hints,struct addrinfo **res);

extern u32_t ipaddr_addr(const char *cp);
typedef u32_t (*_api_ipaddr_addr_t)(const char *cp);

extern int ip4addr_aton(const char *cp, ip4_addr_t *addr);
typedef int (*_api_ip4addr_aton_t)(const char *cp, ip4_addr_t *addr);

extern char *ip4addr_ntoa(const ip4_addr_t *addr);
typedef char *(*_api_ip4addr_ntoa_t)(const ip4_addr_t *addr);

extern char *ip4addr_ntoa_r(const ip4_addr_t *addr, char *buf, int buflen);
typedef char *(*_api_ip4addr_ntoa_r_t)(const ip4_addr_t *addr, char *buf, int buflen);

extern int ip6addr_aton(const char *cp, ip6_addr_t *addr);
typedef int (*_api_ip6addr_aton_t)(const char *cp, ip6_addr_t *addr);

extern char *ip6addr_ntoa_r(const ip6_addr_t *addr, char *buf, int buflen);
typedef char *(*_api_ip6addr_ntoa_r_t)(const ip6_addr_t *addr, char *buf, int buflen);

extern int lwip_getaddrinfowithcid(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res, uint8_t cid);
typedef int (*_api_lwip_getaddrinfowithcid_t)(const char *nodename, const char *servname, const struct addrinfo *hints, struct addrinfo **res, uint8_t cid);

extern int lwip_getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);
typedef int (*_api_lwip_getsockopt_t)(int s, int level, int optname, void *optval, socklen_t *optlen);

extern u16_t lwip_htons(u16_t x);
typedef u16_t (*_api_lwip_htons_t)(u16_t x);

extern u32_t lwip_htonl(u32_t x);
typedef u32_t (*_api_lwip_htonl_t)(u32_t x);

extern int lwip_setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
typedef int (*_api_lwip_setsockopt_t)(int s, int level, int optname,const void *optval, socklen_t optlen);

extern int lwip_getsockname(int s, struct sockaddr *name, socklen_t *namelen);
typedef int (*_api_lwip_getsockname_t)(int s, struct sockaddr *name, socklen_t *namelen);

extern u16_t inet_chksum(const void *dataptr, u16_t len);
typedef u16_t (*_api_inet_chksum_t)(const void *dataptr, u16_t len);

extern int lwip_getpeername(int s, struct sockaddr *name, socklen_t *namelen);
typedef int (*_api_lwip_getpeername_t)(int s, struct sockaddr *name, socklen_t *namelen);

extern int sock_get_errno(int s);
typedef int (*_api_sock_get_errno_t)(int s);

extern int32_t lwip_alloc_server_port(u8_t type);
typedef int32_t (*_api_lwip_alloc_server_port_t)(u8_t type);

#ifdef __cplusplus
} /*"C" */
#endif

#endif
