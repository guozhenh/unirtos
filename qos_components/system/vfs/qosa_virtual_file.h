/*****************************************************************/ /**
* @file qosa_virtual_file.h
* @brief Virtual file system interface for UniRTOS
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

#ifndef __VIRTUAL_FILE_H__
#define __VIRTUAL_FILE_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_system_config.h"
#include "qosa_system_vfs_cfg.h"

#ifdef CONFIG_QOSA_VFS_SYS_STAT_H
#include CONFIG_QOSA_VFS_SYS_STAT_H
#endif /* CONFIG_QOSA_VFS_SYS_STAT_H */

#ifdef CONFIG_QOSA_VFS_SYS_FCNTL_H
#include CONFIG_QOSA_VFS_SYS_FCNTL_H
#endif /* CONFIG_QOSA_VFS_SYS_FCNTL_H */

/**
 * the maximum absolute file path including mount point, terminate \0
 */
#define QOSA_VFS_PATH_MAX    (192)

/**
 * read only flag used in \p qosa_vfs_remount
 */
#define QOSA_MS_RDONLY       1

#define QOSA_VFS_DT_UNKOWN   0  ///< unknown type
#define QOSA_VFS_DT_DIR      4  ///< directory
#define QOSA_VFS_DT_REG      8  ///< regular file

/** Open file in read-only mode */
#define QOSA_VFS_O_RDONLY    CONFIG_QOSA_VFS_O_RDONLY
/** Open file in write-only mode */
#define QOSA_VFS_O_WRONLY    CONFIG_QOSA_VFS_O_WRONLY
/** Open file in read-write mode. The above three flags are mutually exclusive, that is, they cannot be used at the same time, but can be combined with the following flags using OR(|) operator. */
#define QOSA_VFS_O_RDWR      CONFIG_QOSA_VFS_O_RDWR
/** If the file to be opened does not exist, automatically create the file. */
#define QOSA_VFS_O_CREAT     CONFIG_QOSA_VFS_O_CREAT
/**
 * If O_CREAT is also set, this instruction will check whether the file exists. If the file does not exist, it will create the file,
 * otherwise it will cause the file to fail to open. In addition, if O_CREAT and O_EXCL are set at the same time, and the file to be opened is
 * a symbolic link, it will cause the file to fail to open.
 */
#define QOSA_VFS_O_EXCL      CONFIG_QOSA_VFS_O_EXCL
/** If the file to be opened is a terminal device, it will not treat the terminal as a process control terminal. */
#define QOSA_VFS_O_NOCTTY    CONFIG_QOSA_VFS_O_NOCTTY
/** If the file exists and is opened in writable mode, this flag will clear the file length to 0, and the original data stored in the file will also disappear. */
#define QOSA_VFS_O_TRUNC     CONFIG_QOSA_VFS_O_TRUNC
/** When reading and writing files, it will start moving from the end of the file, that is, the written data will be added to the end of the file in an additional way. */
#define QOSA_VFS_O_APPEND    CONFIG_QOSA_VFS_O_APPEND
/** Open file in non-blocking mode, that is, it will return to the process immediately regardless of whether there is data to read or wait. */
#define QOSA_VFS_O_NONBLOCK  CONFIG_QOSA_VFS_O_NONBLOCK
/** Same as O_NONBLOCK. */
#define QOSA_VFS_O_NDELAY    CONFIG_QOSA_VFS_O_NDELAY
/** Open file in synchronous mode. */
#define QOSA_VFS_O_SYNC      CONFIG_QOSA_VFS_O_SYNC
/** If the file pointed to by the parameter pathname is a symbolic link, it will cause the file to fail to open. */
#define QOSA_VFS_O_NOFOLLOW  CONFIG_QOSA_VFS_O_NOFOLLOW
/** If the file pointed to by the parameter pathname is not a directory, it will cause the file to fail to open. */
#define QOSA_VFS_O_DIRECTORY CONFIG_QOSA_VFS_O_DIRECTORY

// Permission control
/** 00700 permission, represents that the file owner has readable, writable and executable permissions. */
#define QOSA_VFS_S_IRWXU     CONFIG_QOSA_VFS_S_IRWXU
/** Or S_IREAD, 00400 permission, represents that the file owner has readable permission. */
#define QOSA_VFS_S_IRUSR     CONFIG_QOSA_VFS_S_IRUSR
/** Or S_IWRITE, 00200 permission, represents that the file owner has writable permission. */
#define QOSA_VFS_S_IWUSR     CONFIG_QOSA_VFS_S_IWUSR
/** Or S_IEXEC, 00100 permission, represents that the file owner has executable permission. */
#define QOSA_VFS_S_IXUSR     CONFIG_QOSA_VFS_S_IXUSR
/** 00070 permission, represents that the file user group has readable, writable and executable permissions. */
#define QOSA_VFS_S_IRWXG     CONFIG_QOSA_VFS_S_IRWXG
/** 00040 permission, represents that the file user group has readable permission. */
#define QOSA_VFS_S_IRGRP     CONFIG_QOSA_VFS_S_IRGRP
/** 00020 permission, represents that the file user group has writable permission. */
#define QOSA_VFS_S_IWGRP     CONFIG_QOSA_VFS_S_IWGRP
/** 00010 permission, represents that the file user group has executable permission. */
#define QOSA_VFS_S_IXGRP     CONFIG_QOSA_VFS_S_IXGRP
/** 00007 permission, represents that other users have readable, writable and executable permissions. */
#define QOSA_VFS_S_IRWXO     CONFIG_QOSA_VFS_S_IRWXO
/** 00004 permission, represents that other users have readable permission */
#define QOSA_VFS_S_IROTH     CONFIG_QOSA_VFS_S_IROTH
/** 00002 permission, represents that other users have writable permission. */
#define QOSA_VFS_S_IWOTH     CONFIG_QOSA_VFS_S_IWOTH
/** 00001 permission, represents that other users have executable permission. */
#define QOSA_VFS_S_IXOTH     CONFIG_QOSA_VFS_S_IXOTH

/** Represents this is a directory */
#define QOSA_VFS_S_IFDIR     CONFIG_QOSA_VFS_S_IFDIR
/** Represents this is a regular file */
#define QOSA_VFS_S_IFREG     CONFIG_QOSA_VFS_S_IFREG

// File offset
/** Parameter offset is the new read/write position */
#define QOSA_VFS_SEEK_SET    CONFIG_QOSA_VFS_SEEK_SET
/** Increase offset displacement from current read/write position */
#define QOSA_VFS_SEEK_CUR    CONFIG_QOSA_VFS_SEEK_CUR
/** Move read/write position to the end of the file and then increase offset displacement */
#define QOSA_VFS_SEEK_END    CONFIG_QOSA_VFS_SEEK_END

struct qosa_vfs_stat_t
{
    qosa_uint64_t st_dev;   /*!< Device ID */
    qosa_uint64_t st_ino;   /*!< i-node number */
    qosa_uint64_t st_mode;  /*!< File type and permissions */
    qosa_uint64_t st_nlink; /*!< Number of hard links */
    qosa_uint64_t st_uid;   /*!< Owner's user ID */
    qosa_uint64_t st_gid;   /*!< Owner's group ID */
    qosa_uint64_t st_rdev;  /*!< Device ID (if file is a device file)*/
    qosa_uint64_t st_size;  /*!< File size (in bytes)*/
    qosa_uint64_t st_spare1;
    qosa_uint64_t st_spare2;
    qosa_uint64_t st_spare3;
    qosa_uint64_t st_blksize; /*!< I/O operation block size */
    qosa_uint64_t st_blocks;  /*!< Number of blocks occupied by file */
    qosa_uint64_t st_spare4[2];
};

/**
 * @struct QOSA_VFS_DIR
 * @brief Used to provide application layer with DIR handle type
 */
typedef struct
{
    qosa_int16_t fs_index;  /*!< Internal file system index */
    qosa_int16_t _reserved; /*!< Reserved bits */
} QOSA_VFS_DIR;

/**
 * @struct  qosa_vfs_dirent_t
 * @brief dirent data structure, same effect as POSIX interface's struct dirent
 */
struct qosa_vfs_dirent_t
{
    qosa_int32_t  d_ino;       /*!< inode number, file system implementation can use it for any purpose */
    unsigned char d_type;      /*!< File type */
    char          d_name[256]; /*!< File name */
};
/**
 * @struct  qosa_vfs_statvfs_t
 * @brief stat data structure, same effect as POSIX interface's struct statvfs
 */
struct qosa_vfs_statvfs_t
{
    unsigned long f_bsize;   /*!< file system block size */
    unsigned long f_frsize;  /*!< fragment size */
    unsigned long f_blocks;  /*!< size of fs in f_frsize units */
    unsigned long f_bfree;   /*!< free blocks in fs */
    unsigned long f_bavail;  /*!< free blocks avail to non-superuser */
    unsigned long f_files;   /*!< total file nodes in file system */
    unsigned long f_ffree;   /*!< free file nodes in fs */
    unsigned long f_favail;  /*!< avail file nodes in fs */
    unsigned long f_fsid;    /*!< file system id */
    unsigned long f_flag;    /*!< mount flags */
    unsigned long f_namemax; /*!< maximum length of filenames */
};

/**
 * @brief unmount file system by file path
 *        \p path must be absolute path, starting with '/'. It can be mount point
 *        or arbitrary file or directory under the mount point.
 *
 * @param[in] const char * path
 *            file path
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error
 *          - EINVAL: invalid parameter
 *          - ENOENT: there are no mount points
 */
qosa_int32_t qosa_vfs_umount(const char *path);

/**
 * @brief remount file system with flags
 * The only supported flags are \p MS_READONLY.
 * \p path must be a mount point.
 *
 * @param[in] const char * path
 *            mount point path
 *
 * @param[in] unsigned flags
 *            file system mount flags
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error
 *          - ENOENT: \p path is not a mount point
 *          - ENOSYS: the file system doesn't support remount
 *          - others: refer to file system implementation
 */
qosa_int32_t qosa_vfs_remount(const char *path, unsigned flags);

/**
 * @brief open and possibly create a file
 *        It is the same as open a file with QOSA_VFS_O_WRONLY, QOSA_VFS_O_CREAT, QOSA_VFS_O_TRUNC.
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] qosa_int32_t mode
 *            file mode, may be ignored by file system implementation
 *
 * @return qosa_int32_t
 *      - file descriptor on success on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_creat(const char *path, qosa_int32_t mode);

/**
 * @brief open and possibly create a file
 *        Refer to man (2) open
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] qosa_int32_t flags
 *            QOSA_VFS_O_CREAT, VFS_O_ACCMODE, QOSA_VFS_O_TRUNC, ......
 *
 * @param[in] int mode
 *            file mode, may be ignored by file system implementation
 *
 * @return qosa_int32_t
 *      - file descriptor on success on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_open(const char *path, qosa_int32_t flags);

/**
 * @brief close a file descriptor
 *        Refer to man (2) close
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_close(qosa_int32_t fd);

/**
 * @brief read from a file descriptor
 *        Refer to man (2) read
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @param[out] void * buf
 *             buf for read
 *
 * @param[in] qosa_size_t count
 *            byte count
 *
 * @return qosa_ssize_t
 *      - the number of bytes read on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_ssize_t qosa_vfs_read(qosa_int32_t fd, void *buf, qosa_size_t count);

/**
 * @brief write to a file descriptor
 *        Refer to man (2) write
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @param[in] const void * buf
 *            buf for write
 *
 * @param[in] qosa_size_t count
 *            byte count
 *
 * @return qosa_ssize_t
 *      - the number of bytes written on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_ssize_t qosa_vfs_write(qosa_int32_t fd, const void *buf, qosa_size_t count);

/**
 * @brief reposition read/write file offset
 *        Refer to man (2) lseek
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @param[in] qosa_int64_t offset
 *            offset according to the directive whence
 *
 * @param[in] qosa_int32_t whence
 *            SEEK_SET, SEEK_CUR, SEEK_END
 *
 * @return qosa_int64_t
 *      - offset location as measured in bytes from the beginning of
 *        the file on success.
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int64_t qosa_vfs_lseek(qosa_int32_t fd, qosa_int64_t offset, qosa_int32_t whence);

/**
 * @brief get file status
 *        Refer to man (2) fstat
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] struct qosa_vfs_stat_t * st
 *            file status
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_fstat(qosa_int32_t fd, struct qosa_vfs_stat_t *st);

/**
 * @brief get file status
 *        Refer to man (2) stat
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] struct qosa_vfs_stat_t * st
 *            file status
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_stat(const char *path, struct qosa_vfs_stat_t *st);

/**
 * @brief truncate a file to a specified length
 *        Refer to man (2) truncate
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] long length
 *            file length
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_truncate(const char *path, long length);

/**
 * @brief truncate a file to a specified length
 *        Refer to man (2) ftruncate
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @param[in] long length
 *            file length
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_ftruncate(qosa_int32_t fd, long length);

/**
 * @brief truncate a file to a specified length
 *        Refer to man (2) link. It is just a placeholder, and no file systems
 *        support it now.
 *
 * @param[in] const char * oldpath
 *            old file path
 *
 * @param[in] const char * newpath
 *            new file path
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_link(const char *oldpath, const char *newpath);

/**
 * @brief delete a name and possibly the file it refers to
 *        Refer to man (2) unlink
 *
 * @param[in] const char * pathname
 *            file path name
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_unlink(const char *pathname);

/**
 * @brief change the name or location of a file
 *        Refer to man (2) rename
 *        \p oldpath and \p newpath must be on the same mounted file system.
 *
 * @param[in] const char * oldpath
 *            old file path
 *
 * @param[in] const char * newpath
 *            new file path
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 *          - EXDEV: oldpath and newpath are not on the same mounted
 *            file system.
 */
qosa_int32_t qosa_vfs_rename(const char *oldpath, const char *newpath);

/**
 * @brief synchronize a file's in-core state with storage device
 *        Refer to man (2) fsync
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_fsync(qosa_int32_t fd);

/**
 * @brief manipulate file descriptor
 *        Refer to man (2) fcntl. It is just a placeholder, and no file systems
 *        support it now.
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @param[in] qosa_int32_t cmd
 *            command
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_fcntl(qosa_int32_t fd, qosa_int32_t cmd);

/**
 * @brief control device
 *        Refer to man (2) ioctl. It is just a placeholder, and no file systems
 *        support it now.
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @param[in] unsigned long request
 *            command
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_ioctl(qosa_int32_t fd, unsigned long request);

/**
 * @brief open a directory
 *        Refer to man (3) opendir
 *
 * @param[in] const char * name
 *            directory name
 *
 * @return QOSA_VFS_DIR *
 *      -  success, return a pointer to the directory stream.
 *      -  error,  NULL is returned
 */
QOSA_VFS_DIR *qosa_vfs_opendir(const char *name);

/**
 * @brief read a directory
 *        Refer to man (3) readdir
 *
 * @param[in] QOSA_VFS_DIR * dirp
 *            directory pointer returned by \p qosa_vfs_opendir
 *
 * @return struct qosa_vfs_dirent_t *
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
struct qosa_vfs_dirent_t *qosa_vfs_readdir(QOSA_VFS_DIR *dirp);

/**
 * @brief read a directory
 *        Refer to man (3) readdir_r
 *
 * @param[in] QOSA_VFS_DIR * dirp
 *            directory pointer returned by \p qosa_vfs_opendir
 *
 * @param[out] struct qosa_vfs_dirent_t * entry
 *            dirent buffer
 *
 * @param[out] struct qosa_vfs_dirent_t * * result
 *            pointer to dirent buffer
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_readdir_r(QOSA_VFS_DIR *dirp, struct qosa_vfs_dirent_t *entry, struct qosa_vfs_dirent_t **result);

/**
 * @brief return current location in directory stream
 *        Refer to man (3) telldir
 *
 * @param[in] QOSA_VFS_DIR * pdir
 *            directory pointer returned by \p qosa_vfs_opendir
 *
 * @return long
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
long qosa_vfs_telldir(QOSA_VFS_DIR *pdir);

/**
 * @brief set the position of the next readdir() call in the directory stream
 *        Refer to man (3) seekdir
 *
 * @param[in] QOSA_VFS_DIR * pdir
 *            directory pointer returned by \p qosa_vfs_opendir
 *
 * @param[in] long loc
 *            location returned by \p qosa_vfs_telldir
 *
 */
void qosa_vfs_seekdir(QOSA_VFS_DIR *pdir, long loc);

/**
 * @brief reset directory stream
 *        Refer to man (3) rewinddir
 *
 * @param[in] QOSA_VFS_DIR * pdir
 *            directory pointer returned by \p qosa_vfs_opendir
 *
 */
void qosa_vfs_rewinddir(QOSA_VFS_DIR *pdir);

/**
 * @brief close a directory
 *        Refer to man (3) closedir
 *
 * @param[in] QOSA_VFS_DIR * pdir
 *            directory pointer returned by \p qosa_vfs_opendir
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_closedir(QOSA_VFS_DIR *pdir);

/**
 * @brief create a directory
 *        Refer to man (2) mkdir
 *
 * @param[in] const char * pathname
 *            directory path name
 *
 * @param[in] qosa_int32_t mode
 *            create mode
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_mkdir(const char *pathname, qosa_int32_t mode);

/**
 * @brief delete a directory
 *        Refer to man (2) rmdir
 *
 * @param[in] const char * pathname
 *            directory path name
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error. Refer to file system implementation for errno.
 */
qosa_int32_t qosa_vfs_rmdir(const char *pathname);

/**
 * @brief change working directory
 *        Refer to man (2) chdir
 *        NOTE: in RTOS system, working directory is a global concept. It is
 *        dangerous to change working directory. Don't call \p qosa_vfs_chdir
 *        unless it is absolutely needed, and you really know what you are
 *        doing.
 *
 * @param[in] const char * path
 *            directory path name
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error
 */
qosa_int32_t qosa_vfs_chdir(const char *path);

/**
 * @brief get current working directory
 *        Refer to man (3) getcwd
 *
 * @param[out] char * buf
 *             buffer for result
 *
 * @param[in] qosa_size_t size
 *            buffer size
 *
 * @return char *
 *      - current working directory on success
 *      - NULL on error
 */
char *qosa_vfs_getcwd(char *buf, qosa_size_t size);

/**
 * @brief return the canonicalized absolute pathname
 *        Refer to man (3) realpath
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] char * resolved_path
 *            buffer for canonicalized absolute pathname
 *
 * @return char *
 *      - \p resolved_path on success
 *      - NULL on error
 */
char *qosa_vfs_realpath(const char *path, char *resolved_path);

/**
 * @brief get filesystem statistics
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] struct qosa_vfs_statvfs_t * buf
 *            filesystem statistics
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error
 */
qosa_int32_t qosa_vfs_statvfs(const char *path, struct qosa_vfs_statvfs_t *buf);

/**
 * @brief get filesystem statistics
 *        Refer to man (3) fstatvfs
 *
 * @param[in] qosa_int32_t fd
 *            file descriptor
 *
 * @param[in] struct qosa_vfs_statvfs_t * buf
 *            filesystem statistics
 *
 * @return qosa_int32_t
 *      - 0 on success
 *      - -1 on error
 */
qosa_int32_t qosa_vfs_fstatvfs(qosa_int32_t fd, struct qosa_vfs_statvfs_t *buf);

/**
 * @brief get mounted file system count
 *
 * @return  mounted file system count
 */
qosa_int32_t qosa_vfs_mount_count(void);

/**
 * @brief get mounted file system mount points
 *
 * @param[in] char * * mp
 *            buffer for mount points
 *
 * @param[in] qosa_size_t count
 *            buffer count
 *
 * @return qosa_int32_t
 *         count of get mount points
 */
qosa_int32_t qosa_vfs_mount_points(char **mp, qosa_size_t count);

/**
 * @brief helper to get canonicalized absolute pathname
 *        It is similar to \p qosa_vfs_realpath, just with specified base path name.
 *
 * @param[in] const char * base_path
 *            base path name
 *
 * @param[in] const char * path
 *            path related to base_path
 *
 * @param[in] char * resolved_path
 *            buffer for canonicalized absolute pathname
 *
 * @return char *
 *      - \p resolved_path on success
 *      - NULL on error
 */
char *qosa_vfs_resolve_path(const char *base_path, const char *path, char *resolved_path);

/**
 * @brief create directory with all parents
 *
 * @param[in] const char * path
 *            path of directory to be created
 *
 * @param[in] qosa_int32_t mode
 *            create mode
 *
 * @return qosa_int32_t
 *      - 0 on succes
 *      - -1 on error
 */
qosa_int32_t qosa_vfs_mkpath(const char *path, qosa_int32_t mode);

/**
 * @brief create directory with all parents of a file
 *
 * @param[in] const char * path
 *            path of file
 *
 * @param[in] qosa_int32_t mode
 *            create mode
 *
 * @return qosa_int32_t
 *      - 0 on succes
 *      - -1 on error
 */
qosa_int32_t qosa_vfs_mkfilepath(const char *path, qosa_int32_t mode);

/**
 * @brief helper to get file size
 *        It calls \p qosa_vfs_stat to get file status, and return file size only.
 *
 * @param[in] const char * path
 *            file path
 *
 * @return qosa_ssize_t
 *      - file size on success
 *      - -1 on error
 */
qosa_ssize_t qosa_vfs_file_size(const char *path);

/**
 * @brief helper to read file
 *        It calls \p qosa_vfs_open, \p qosa_vfs_read and \p qosa_vfs_close to read bytes from file.
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] void * buf
 *            buf for read
 *
 * @param[in] qosa_size_t count
 *            byte count
 *
 *
 * @return qosa_ssize_t
 *      - the number of bytes read on success
 *      - -1 on error
 */
qosa_ssize_t qosa_vfs_file_read(const char *path, void *buf, qosa_size_t count);

/**
 * @brief helper to write file
 *        It calls \p qosa_vfs_open, \p qosa_vfs_write and \p qosa_vfs_close to write bytes to file.
 *
 * @param[in] const char * path
 *            file path
 *
 * @param[in] const void * buf
 *            buf for write
 *
 * @param[in] qosa_size_t count
 *            byte count
 *
 * @return qosa_ssize_t
 *      - the number of bytes written on success
 *      - -1 on error
 */
qosa_ssize_t qosa_vfs_file_write(const char *path, const void *buf, qosa_size_t count);

/**
 * @brief delete files and subdirectories under a directory
 *
 * @param[in] const char *path
 *            directory path
 *
 * @return qosa_int32_t
 *      - the number of bytes read on success
 *      - -1 on error
 */
qosa_int32_t qosa_vfs_rmchildren(const char *path);

/**
 * @brief delete directory and children under the directory
 *
 * @param[in] const char *path
 *            directory path
 *
 * @return qosa_int32_t
 *      - the number of bytes read on success
 *      - -1 on error
 */
qosa_int32_t qosa_vfs_rmdir_recursive(const char *path);

/**
 * @brief umount all mounted file system
 *
 * It should only be called in FDL now. And in application, the
 * implementation is empty.
 */
void qosa_vfs_umount_all(void);

/**
 * @brief fs handle of a path
 *        It is for debug only, get the file system implementation handler.
 *
 * @param[in] const char * path
 *            file path
 *
 * @return void *
 *         - the file system implementation handler
 */
void *qosa_vfs_fs_handle(const char *path);

/**
 * @brief fs handle of mount point
 *        It can be used to determine whether a mount point is mounted.
 *
 * @param[in] const char * path
 *            mount point
 *
 * @return void *
 *      - the file system implementation handler
 */
void *qosa_vfs_mount_handle(const char *path);

/**
 * @brief recursive total file size of a directory
 *
 * @param[in] const char * path
 *            path directory name
 *
 * @return qosa_int64_t
 *      - recursive total file size of a directory
 *      - -1 on error, invalid parameter or out of memory
 */
qosa_int64_t qosa_vfs_dir_total_size(const char *path);

/**
 * @brief Usage is consistent with the sync function in posix, used to forcibly flush data in the file system buffer to disk.
 *        Use man 3 sync to see specific usage
 *
 * @param[in] qosa_int32_t fd
 *            File handle to be synchronized
 */
qosa_int32_t qosa_vfs_sync(qosa_int32_t fd);

#endif /* __VFS_H__ */
