/** @file         qurl_default_fs.c
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-02 11:45:21
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
 */

#include "qurl_def_basic.h"
#include "qurl_default_fs.h"
#include "qurl_log.h"

#if (QURL_CFG_ENABLE_FS == 0)

int qurl_default_open(const char *pathname, int mode)
{
    QURL_UNUSED(pathname);
    QURL_UNUSED(mode);
    return -1;
}

int qurl_default_read(int fd, void *buf, long count)
{
    QURL_UNUSED(fd);
    QURL_UNUSED(buf);
    QURL_UNUSED(count);
    return -1;
}

int qurl_default_getsize(int fd)
{
    QURL_UNUSED(fd);
    return -1;
}

int qurl_default_lseek(int fd, long offset, int whence)
{
    QURL_UNUSED(fd);
    QURL_UNUSED(offset);
    QURL_UNUSED(whence);
    return -1;
}

int qurl_default_close(int fd)
{
    QURL_UNUSED(fd);
    return -1;
}

#endif /* QURL_CFG_ENABLE_FS */
