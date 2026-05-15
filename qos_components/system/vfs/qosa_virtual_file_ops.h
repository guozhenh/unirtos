/*****************************************************************/ /**
* @file qosa_virtual_file_ops.h
* @brief
* @author larson.li@quectel.com
* @date 2023-06-01
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-06-01 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __VFS_OPS_H__
#define __VFS_OPS_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_virtual_file.h"
#include "qosa_spi.h"

#define QOSA_VFS_PREFIX_MAX     (15)

#define QOSA_VFS_NEED_REAL_PATH (0x0001)
typedef struct
{
    int flags;
    int (*umount)(void *fs);
    int (*remount)(void *fs, unsigned flags);
    int (*open)(void *fs, const char *path, int flags, int /*mode_t*/ mode);
    int (*close)(void *fs, int fd);
    qosa_ssize_t (*read)(void *fs, int fd, void *dst, qosa_size_t size);
    qosa_ssize_t (*write)(void *fs, int fd, const void *data, qosa_size_t size);
    long /*off_t*/ (*lseek)(void *fs, int fd, long /*off_t*/ offset, int mode);
    int (*fstat)(void *fs, int fd, struct qosa_vfs_stat_t *st);
    int (*stat)(void *fs, const char *path, struct qosa_vfs_stat_t *st);
    int (*truncate)(void *fs, const char *path, long length);
    int (*ftruncate)(void *fs, int fd, long length);
    int (*unlink)(void *fs, const char *path);
    int (*rename)(void *fs, const char *src, const char *dst);

    QOSA_VFS_DIR *(*opendir)(void *fs, const char *name);
    struct qosa_vfs_dirent_t *(*readdir)(void *fs, QOSA_VFS_DIR *pdir);
    int (*readdir_r)(void *fs, QOSA_VFS_DIR *pdir, struct qosa_vfs_dirent_t *entry, struct qosa_vfs_dirent_t **out_dirent);
    long (*telldir)(void *fs, QOSA_VFS_DIR *pdir);
    void (*seekdir)(void *fs, QOSA_VFS_DIR *pdir, long loc);
    int (*closedir)(void *fs, QOSA_VFS_DIR *pdir);
    int (*mkdir)(void *fs, const char *name, int /*mode_t*/ mode);
    int (*rmdir)(void *fs, const char *name);
    int (*fsync)(void *fs, int fd);

    int (*statvfs)(void *fs, const char *path, struct qosa_vfs_statvfs_t *buf);
    int (*fstatvfs)(void *fs, int fd, struct qosa_vfs_statvfs_t *buf);
    qosa_ssize_t (*file_write)(void *fs, const char *path, const void *data, qosa_size_t size);
} qosa_vfs_ops_t;

typedef struct
{
    qosa_int32_t (*init)(qosa_spi_port_e port, qosa_spi_clk_e clk);
    qosa_int32_t (*read)(qosa_spi_port_e port, qosa_uint8_t *data, qosa_uint32_t addr, qosa_uint32_t len);
    qosa_int32_t (*write)(qosa_spi_port_e port, qosa_uint8_t *data, qosa_uint32_t addr, qosa_uint32_t len);
    qosa_int32_t (*erase)(qosa_spi_port_e port, qosa_uint32_t addr, qosa_uint32_t size);
    qosa_int32_t (*set_cs)(qosa_uint32_t pin_num, qosa_uint8_t pin_func);
    qosa_int32_t (*read_id)(qosa_spi_port_e port, qosa_uint8_t *id);
    qosa_uint8_t format_opt;
} qosa_ext_flash_t;

qosa_int32_t qosa_vfs_register(const char *base_path, const qosa_vfs_ops_t *ops, void *ctx);
qosa_int32_t qosa_vfs_unregister(const char *base_path);
qosa_int32_t qosa_ext_flash_register(qosa_ext_flash_t *ops);
#endif /* __VFS_OPS_H__ */
