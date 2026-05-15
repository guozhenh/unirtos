/** @file         qurl_core.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-08-31 19:52:05
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#ifndef QURL_CORE_H
#define QURL_CORE_H

#include "qurl_def_in.h"

/** Protocol Link Configuration*/
/** The maximum number of sockets each protocol can have. For example, FTP has both a command connection and a data connection.*/
#define QURL_CORE_PROTOCOL_SOCKET_MAX  3

/** the write bits start at bit 16 for the *getsock() bitmap */
#define QURL_GETSOCK_WRITEBITSTART     16
/** no bits set */
#define QURL_GETSOCK_BLANK             0
/** set the bit for the given sock number to make the bitmap for writable */
#define QURL_GETSOCK_WRITESOCK_FUNC(x) (1 << (QURL_GETSOCK_WRITEBITSTART + (x)))
/** set the bit for the given sock number to make the bitmap for readable */
#define QURL_GETSOCK_READSOCK_FUNC(x)  (1 << (x))

/* internal API ================================================================= */
qurl_ecode_t qurl_core_sec_init(void);

qurl_ecode_t qurl_core_sec_deinit(void);

qurl_bit_t core_abort_get(qurl_core_ctrl_t *core_ptr);

qurl_bit_t core_timeout_check(qurl_core_ctrl_t *core_ptr);

void core_idle_timeout_feed_dog(qurl_core_ctrl_t *core_ptr);

qurl_bit_t core_idle_timeout_check_dog(qurl_core_ctrl_t *core_ptr);

#define QURL_CORE_CHECK_ABORT_FUNC(x)                                                                                                                          \
    do {                                                                                                                                                       \
        if (core_abort_get(x) == true)                                                                                                                         \
        {                                                                                                                                                      \
            return QURL_ECODE_ABORT;                                                                                                                           \
        }                                                                                                                                                      \
        if (core_timeout_check(x) == true)                                                                                                                     \
        {                                                                                                                                                      \
            return QURL_ECODE_PERFORM_TIMEOUT;                                                                                                                 \
        }                                                                                                                                                      \
    } while (0)

#define QURL_CORE_IDLE_TIMEOUT_CHECK_DOG_FUNC(x)                                                                                                               \
    do {                                                                                                                                                       \
        if (core_idle_timeout_check_dog(x) == true)                                                                                                            \
        {                                                                                                                                                      \
            return QURL_ECODE_PERFORM_TIMEOUT;                                                                                                                 \
        }                                                                                                                                                      \
    } while (0)

#endif /* Head define end*/
