/** @file         qurl_platform_config.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-01 15:26:03
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_PLATFORM_CONFIG_H
#define QURL_PLATFORM_CONFIG_H

/** Platform side adds header file*/
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

/** Maximum number of concurrent services*/
#define QURL_CFG_CORE_SERVER_MAX                (15)

/** Error code base value (the highest bit of the system must be 1)*/
// #define QURL_CFG_ECODE_BASIC_NUM (0x80010000)
/** Event code base value (the highest bit of the system must be 1)*/
#define QURL_CFG_EVENT_CODE_BASIC_NUM           (0x80020000)

/*Protocol Family*/
#define QURL_CFG_ENABLE_IPV6                    0

/* LOG */
/** Enable QURL LOG*/
#define QURL_CFG_LOG                            1

/*Supported protocol types and functions*/
#define QURL_CFG_ENABLE_FS                      1
#define QURL_CFG_ENABLE_TLS_MBEDTLS             1
#define QURL_CFG_ENABLE_HTTP                    1
#define QURL_CFG_ENABLE_FTP                     1
#define QURL_CFG_ENABLE_SMTP                    1

/** Standard Library Related APIs*/
#define QURL_CFG_PORT_MALLOC_FUNC               malloc
#define QURL_CFG_PORT_FREE_FUNC                 free

/** OS: Description, FUNC has format type requirements*/
#define QURL_CFG_PORT_THREAD_CREATE_FUNC        qurl_platform_thread_create
#define QURL_CFG_PORT_THREAD_DELETE_FUNC        qurl_platform_thread_delete
#define QURL_CFG_PORT_SLEEP_MS_FUNC             qurl_platform_sleep_ms
#define QURL_CFG_PORT_MUTEX_CREATE_FUNC         qurl_platform_mutex_create
#define QURL_CFG_PORT_MUTEX_DELETE_FUNC         qurl_platform_mutex_delete
#define QURL_CFG_PORT_MUTEX_LOCK_FUNC           qurl_platform_mutex_lock
#define QURL_CFG_PORT_MUTEX_UNLOCK_FUNC         qurl_platform_mutex_unlock
#define QURL_CFG_PORT_EVENT_CREATE_FUNC         qurl_platform_event_create
#define QURL_CFG_PORT_EVENT_DELETE_FUNC         qurl_platform_event_delete
#define QURL_CFG_PORT_EVENT_WAIT_FUNC           qurl_platform_event_wait
#define QURL_CFG_PORT_EVENT_SEND_FUNC           qurl_platform_event_send
#define QURL_CFG_PORT_GET_THREAD_ID_FUNC        qurl_platform_get_pthread_id
#define QURL_CFG_PORT_TIME_NOW_FUNC             qurl_platform_time_now
/** SOCKET: Description, FUNC has format type requirements*/
#define QURL_CFG_PORT_SOCKET_RESOLVE_FUNC       qurl_platform_socket_dns_get
#define QURL_CFG_PORT_SOCKET_ADDRINFO_FREE_FUNC freeaddrinfo
#define QURL_CFG_PORT_SOCKET_SOCKET_FUNC        socket
#define QURL_CFG_PORT_SOCKET_BIND_FUNC          bind
#define QURL_CFG_PORT_SOCKET_LISTEN_FUNC        listen
#define QURL_CFG_PORT_SOCKET_ACCEPT_FUNC        accept
#define QURL_CFG_PORT_SOCKET_CONNECT_FUNC       connect
#define QURL_CFG_PORT_SOCKET_READ_FUNC          read
#define QURL_CFG_PORT_SOCKET_WRITE_FUNC         write
#define QURL_CFG_PORT_SOCKET_SELECT_FUNC        select
#define QURL_CFG_PORT_SOCKET_CLOSE_FUNC         close
#define QURL_CFG_PORT_SOCKET_SETSOCKOPT_FUNC    setsockopt
#define QURL_CFG_PORT_SOCKET_NONBLOCK_FUNC      qurl_platform_socket_nonblock
#define QURL_CFG_PORT_SOCKET_GET_ERRNO_FUNC     qurl_platform_socket_get_errno
#define QURL_CFG_PORT_SOCKET_OPT_SO_KEEPALIVE   SO_KEEPALIVE
#define QURL_CFG_PORT_SOCKET_OPT_TCP_KEEPIDLE   TCP_KEEPIDLE
#define QURL_CFG_PORT_SOCKET_OPT_TCP_KEEPINTVL  TCP_KEEPINTVL
#define QURL_CFG_PORT_SOCKET_OPT_TCP_KEEPCNT    TCP_KEEPCNT
#define QURL_CFG_PORT_IN6ADDR_ANY_INIT          in6addr_any

/** FS */
#define QURL_CFG_PORT_FOPEN_FUNC                open
#define QURL_CFG_PORT_FREAD_FUNC                read
#define QURL_CFG_PORT_FGETSIZE_FUNC             qurl_platform_fgetsize
#define QURL_CFG_PORT_FLSEEK_FUNC               lseek
#define QURL_CFG_PORT_FCLOSE_FUNC               close
#define QURL_CFG_PORT_F_O_RDONLY                O_RDONLY
#define QURL_CFG_PORT_F_O_RDWR                  O_RDWR
#define QURL_CFG_PORT_F_O_CREAT                 O_CREAT
#define QURL_CFG_PORT_F_O_EXCL                  O_EXCL
/** TLS */

#endif /* Head define end*/
