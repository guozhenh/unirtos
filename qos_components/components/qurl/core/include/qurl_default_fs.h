/** @file         qurl_default_fs.h
 *  @brief        Brief description: Encapsulates the default file system API
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-02 11:45:40
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_DEFAULT_FS_H
#define QURL_DEFAULT_FS_H

#include "qurl_config.h"

#if (QURL_CFG_ENABLE_FS == 0)

int qurl_default_open(const char *pathname, int mode);

int qurl_default_read(int fd, void *buf, long count);

int qurl_default_getsize(int fd);

int qurl_default_lseek(int fd, long offset, int whence);

int qurl_default_close(int fd);

#endif /* QURL_CFG_ENABLE_FS */

#endif /* Head define end*/
