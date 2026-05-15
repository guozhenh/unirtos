/** @file         qurl_select.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-09-25 14:56:42
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_def_in.h"
#include "qurl_log.h"
#include "qurl_code_in.h"
#include "qurl_core.h"
#include "qurl_select.h"

/**
 * @brief select
 * @details
 * @retval <0: Error code
 * @retval =0: Timeout
 * @retval >0: Number of file descriptors where events occurred
 */
qurl_ecode_t qurl_select(
    qurl_socket_t maxfd,     /* highest socket number */
    fd_set       *fds_read,  /* sockets ready for reading */
    fd_set       *fds_write, /* sockets ready for writing */
    fd_set       *fds_err,   /* sockets with errors */
    qurl_time_t   timeout_ms
)                            /* milliseconds to wait */
{
    qurl_ecode_t    ret = 0;
    struct timeval  pending_tv = {0};
    struct timeval *pending_tv_ptr = QOSA_NULL;

    if (timeout_ms < 0)
    {
        pending_tv_ptr = QOSA_NULL;
    }
    else if (timeout_ms == 0)
    {
        pending_tv_ptr = &pending_tv;
        pending_tv_ptr->tv_sec = 0;
        pending_tv_ptr->tv_usec = 0;
    }
    else
    {
        pending_tv_ptr = &pending_tv;
        pending_tv_ptr->tv_sec = (long)timeout_ms / 1000;
        pending_tv_ptr->tv_usec = ((long)timeout_ms % 1000) * 1000;
    }

    ret = qurl_socket_select(maxfd + 1, fds_read, fds_write, fds_err, pending_tv_ptr);
    return ret; /*Temporarily not converting*/
}

/**
 * @brief 
 * 
 * @param checkfd 
 * @param bitmaps 
 * @param timeout_ms 
 * @return The return value of qurl_ecode_t is actually the value of select.
 */
qurl_ecode_t qurl_socket_check(qurl_socket_t checkfd, unsigned char bitmaps, qurl_time_t timeout_ms)
{
    qurl_ecode_t    ret = 0;
    fd_set          fds_read = {0};
    fd_set          fds_write = {0};
    fd_set          fds_err = {0};
    struct timeval  pending_tv = {0};
    struct timeval *pending_tv_ptr = QOSA_NULL;

    if (checkfd == QURL_SOCKET_BAD || (!(bitmaps & QURL_SELECT_BIT_READ) && !(bitmaps & QURL_SELECT_BIT_WRITE) && !(bitmaps & QURL_SELECT_BIT_ERROR)))
    {
        return -1; /*Error code is -*/
    }

    FD_ZERO(&fds_read);
    FD_ZERO(&fds_write);
    FD_ZERO(&fds_err);

    if (bitmaps & QURL_SELECT_BIT_READ)
    {
        FD_SET(checkfd, &fds_read);
    }
    if (bitmaps & QURL_SELECT_BIT_WRITE)
    {
        FD_SET(checkfd, &fds_write);
    }
    if (bitmaps & QURL_SELECT_BIT_ERROR)
    {
        FD_SET(checkfd, &fds_err);
    }

    if (timeout_ms < 0)
    {
        pending_tv_ptr = QOSA_NULL;
    }
    else if (timeout_ms == 0)
    {
        pending_tv_ptr = &pending_tv;
        pending_tv_ptr->tv_sec = 0;
        pending_tv_ptr->tv_usec = 0;
    }
    else
    {
        pending_tv_ptr = &pending_tv;
        pending_tv_ptr->tv_sec = (long)timeout_ms / 1000;
        pending_tv_ptr->tv_usec = ((long)timeout_ms % 1000) * 1000;
    }

    ret = qurl_socket_select(checkfd + 1, &fds_read, &fds_write, &fds_err, pending_tv_ptr);

    if (ret > 0)
    {
        ret = 0;
        if ((bitmaps & QURL_SELECT_BIT_READ) && (FD_ISSET(checkfd, &fds_read)))
        {
            ret |= QURL_SELECT_BIT_READ;
        }
        if ((bitmaps & QURL_SELECT_BIT_WRITE) && (FD_ISSET(checkfd, &fds_write)))
        {
            ret |= QURL_SELECT_BIT_WRITE;
        }
        if ((bitmaps & QURL_SELECT_BIT_ERROR) && (FD_ISSET(checkfd, &fds_err)))
        {
            ret |= QURL_SELECT_BIT_ERROR;
        }
    }

    return ret; /*Temporarily not converting*/
}
