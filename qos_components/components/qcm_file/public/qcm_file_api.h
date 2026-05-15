/*****************************************************************/ /**
* @file qcm_file_api.h
* @brief UniRTOS File System API Interface Header File
* @details
* This file defines the file operation API interface for the UniRTOS platform, providing a unified
* file system access interface that supports file operations on multiple storage media
* (UFS, SFS, SD, FOTA, DATA, etc.).
*
* Main features include:
* - File open, close, read and write operations
* - Directory create, delete, and traversal operations
* - File attribute getting and setting
* - File system information query
* - Unified management of multiple storage media
*
* @author larson.li@quectel.com
* @date 2023-06-20
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2023-06-20 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/

#ifndef __QCM_FILE_API_H__
#define __QCM_FILE_API_H__

#include "qcm_proj_config.h"
#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_buffer_block.h"
#include "qosa_virtual_file.h"
#include "qcm_file_cfg.h"

/**
 * @brief Byte alignment reserved size
 * @details Used for byte alignment during file system operations to ensure data access efficiency
 */
#define QCM_FILE_ALIGEN_RESERVE_SIZE 3 * 1024

/**
 * @brief Maximum file path length
 * @details Includes maximum filename length + maximum folder path length + 1(path separator)
 */
#define QCM_FILE_MAX_PATH_LEN        (CONFIG_QOSA_FILE_MAX_NAME_LEN + CONFIG_QOSA_FILE_MAX_FOLDER_LEN + 1)

/**
 * @brief Maximum filename length with prefix
 * @details Includes file prefix length + maximum file path length
 */
#define QCM_FILE_FILENAME_MAX_LEN    (CONFIG_QOSA_FILE_PREFIX_LEN + QCM_FILE_MAX_PATH_LEN)

/**
 * @brief UFS file system prefix
 * @details
 * - Note: The macro UNIR_FILE_PREFIX_LEN in unir_file_cfg.h defines the maximum length of prefix names
 * - If the prefix name exceeds the maximum length defined by UNIR_FILE_PREFIX_LEN, this macro needs to be modified
 * - UFS: Represents UFS file system prefix
 */
#define QCM_FILE_PRXFIX_UFS          "UFS:"   // UFS file system prefix
#define QCM_FILE_PRXFIX_UFS_LEN      4        // UFS prefix length

#define QCM_FILE_PRXFIX_FOTA         "FOTA:"  // FOTA file system prefix
#define QCM_FILE_PRXFIX_FOTA_LEN     5        // FOTA prefix length

#ifdef CONFIG_QCM_FILE_SFS_FUNC
#define QCM_FILE_PRXFIX_SFS     "SFS:"  // SFS secure file system prefix (requires CONFIG_QCM_FILE_SFS_FUNC configuration)
#define QCM_FILE_PRXFIX_SFS_LEN 4       // SFS prefix length
#else
#define QCM_FILE_PRXFIX_SFS     NULL    // Empty when SFS function is not enabled
#define QCM_FILE_PRXFIX_SFS_LEN 0       // SFS prefix length is 0
#endif

#define QCM_FILE_PRXFIX_SD_1ST     "SD:"  // First SD card file system prefix
#define QCM_FILE_PRXFIX_SD_1ST_LEN 3      // SD prefix length

#if defined(CONFIG_QOSA_ASR1903_PLATFORM_FUNC) || defined(CONFIG_QOSA_ASR160X_PLATFORM_FUNC) || defined(CONFIG_QOSA_ASR1805_PLATFORM_FUNC)
/**
 * @brief LFS file system too many files open error code
 * @details Defines the error code returned in lfs_vfs when too many files are opened,
 *          only applicable to ASR1903, ASR160X, and ASR1805 platforms
 */
#define OSA_LFS_FILE_IS_OPENED_TOO_MUCH (-23)
#endif /* CONFIG_QOSA_ASR1903_PLATFORM_FUNC || CONFIG_QOSA_ASR160X_PLATFORM_FUNC || CONFIG_QOSA_ASR1805_PLATFORM_FUNC */

#define QCM_FILE_PRXFIX_DATA      "DATA:"  // DATA file system prefix
#define QCM_FILE_PRXFIX_DATA_LEN  5        // DATA prefix length

#define QCM_FILE_PRXFIX_EXT       "EXTNOR:"
#define QCM_FILE_PRXFIX_EXT_LEN   7

#define QCM_FILE_SEEK_SET         QOSA_VFS_SEEK_SET  // Seek from the beginning of the file
#define QCM_FILE_SEEK_CUR         QOSA_VFS_SEEK_CUR  // Seek from the current position
#define QCM_FILE_SEEK_END         QOSA_VFS_SEEK_END  // Seek from the end of the file

#define QCM_FILE_DT_REG           QOSA_VFS_DT_REG    // File attribute is regular file
#define QCM_FILE_DT_DIR           QOSA_VFS_DT_DIR    // File attribute is regular directory

#define QCM_FILE_O_RDONLY         QOSA_VFS_O_RDONLY  // Open file in read-only mode
#define QCM_FILE_O_WRONLY         QOSA_VFS_O_WRONLY  // Open file in write-only mode
#define QCM_FILE_O_RDWR           QOSA_VFS_O_RDWR    // Open file in read-write mode (mutually exclusive with RDONLY/WRONLY)
#define QCM_FILE_O_CREAT          QOSA_VFS_O_CREAT   // Create file if it doesn't exist
#define QCM_FILE_O_TRUNC          QOSA_VFS_O_TRUNC   // Truncate file if it exists
#define QCM_FILE_O_ACCESSPERMS    (QOSA_VFS_S_IRWXU | QOSA_VFS_S_IRWXG | QOSA_VFS_S_IRWXO)  // Default file permissions 0777

/** Base offset code for FILE API function module errors */
#define QCM_ERRCODE_FILE_API_BASE (QOSA_COMPONENT_FILE_API << 16)

/**
 * @enum qcm_file_error_e
 * @brief File API operation error codes
 * @details Defines all possible error codes in file system operations, divided into the following categories:
 *          - Basic file operation errors (400-438)
 *          - ZIP file operation errors (439-442)
 *          - File creation limit errors (450)
 *          - Directory operation errors (600-608)
 */
typedef enum
{
    QCM_FILE_OK = 0, /*!< Operation successful */

    /* Basic file operation error codes (400-438) */
    QCM_FILE_INVALID_INPUT_VALUE = QCM_ERRCODE_FILE_API_BASE | 400,     /*!< Invalid input parameter value */
    QCM_FILE_LARGER_THAN_FILESIZE = QCM_ERRCODE_FILE_API_BASE | 401,    /*!< Requested operation data size exceeds file size */
    QCM_FILE_READ_ZERO = QCM_ERRCODE_FILE_API_BASE | 402,               /*!< Read 0 bytes of data */
    QCM_FILE_DRIVE_FULL = QCM_ERRCODE_FILE_API_BASE | 403,              /*!< Storage drive is full */
    QCM_FILE_MOV_ERROR = QCM_ERRCODE_FILE_API_BASE | 404,               /*!< File move operation failed */
    QCM_FILE_NOT_FOUND = QCM_ERRCODE_FILE_API_BASE | 405,               /*!< File not found */
    QCM_FILE_INVALID_FILE_NAME = QCM_ERRCODE_FILE_API_BASE | 406,       /*!< Invalid file name */
    QCM_FILE_FILE_ALREADY_EXISTED = QCM_ERRCODE_FILE_API_BASE | 407,    /*!< File already exists */
    QCM_FILE_FAILED_TO_CREATE_FILE = QCM_ERRCODE_FILE_API_BASE | 408,   /*!< File creation failed */
    QCM_FILE_FAILED_TO_WRITE_FILE = QCM_ERRCODE_FILE_API_BASE | 409,    /*!< File write failed */
    QCM_FILE_FAILED_TO_OPEN_FILE = QCM_ERRCODE_FILE_API_BASE | 410,     /*!< File open failed */
    QCM_FILE_FAILED_TO_READ_FILE = QCM_ERRCODE_FILE_API_BASE | 411,     /*!< File read failed */
    QCM_FILE_EXCEED_MAX_LENGTH = QCM_ERRCODE_FILE_API_BASE | 412,       /*!< Exceeded maximum length limit */
    QCM_FILE_REACH_MAX_OPENFILE_NUM = QCM_ERRCODE_FILE_API_BASE | 413,  /*!< Reached maximum open file number limit */
    QCM_FILE_IS_READONLY = QCM_ERRCODE_FILE_API_BASE | 414,             /*!< File is read-only */
    QCM_FILE_GET_SIZE_FAIL = QCM_ERRCODE_FILE_API_BASE | 415,           /*!< Failed to get file size */
    QCM_FILE_INVALID_FILE_DESCRIPTOR = QCM_ERRCODE_FILE_API_BASE | 416, /*!< Invalid file descriptor */
    QCM_FILE_LIST_FILE_FAIL = QCM_ERRCODE_FILE_API_BASE | 417,          /*!< Failed to get file list */
    QCM_FILE_DELETE_FILE_FAIL = QCM_ERRCODE_FILE_API_BASE | 418,        /*!< Failed to delete file */
    QCM_FILE_GET_DISK_INFO_FAIL = QCM_ERRCODE_FILE_API_BASE | 419,      /*!< Failed to get disk information */
    QCM_FILE_NO_SPACE = QCM_ERRCODE_FILE_API_BASE | 420,                /*!< Insufficient disk space */
    QCM_FILE_TIME_OUT = QCM_ERRCODE_FILE_API_BASE | 421,                /*!< Operation timeout */
    QCM_FILE_NOT_FOUND_2 = QCM_ERRCODE_FILE_API_BASE | 422,             /*!< File not found (backup error code) */
    QCM_FILE_TOO_LARGE = QCM_ERRCODE_FILE_API_BASE | 423,               /*!< File is too large */
    QCM_FILE_ALREADY_EXIST = QCM_ERRCODE_FILE_API_BASE | 424,           /*!< File already exists (backup error code) */
    QCM_FILE_INVALID_PARAMETER = QCM_ERRCODE_FILE_API_BASE | 425,       /*!< Invalid input parameter */
    QCM_FILE_ALREADY_OPERATION = QCM_ERRCODE_FILE_API_BASE | 426,       /*!< File is being operated on */
    QCM_FILE_ERROR_GENERAL = QCM_ERRCODE_FILE_API_BASE | 427,           /*!< General error */
    QCM_FILE_RENAME_ERROR = QCM_ERRCODE_FILE_API_BASE | 428,            /*!< File rename error */
    QCM_FILE_QUIT_DATE_MODE = QCM_ERRCODE_FILE_API_BASE | 429,          /*!< Exit data mode error */
    QCM_FILE_CLOSE_FAIL = QCM_ERRCODE_FILE_API_BASE | 430,              /*!< File close failed */
    QCM_FILE_SEEK_FAIL = QCM_ERRCODE_FILE_API_BASE | 431,               /*!< File pointer positioning failed */
    QCM_FILE_TELL_FAIL = QCM_ERRCODE_FILE_API_BASE | 432,               /*!< Failed to get file pointer position */
    QCM_FILE_FAILED_TO_GET_STAT = QCM_ERRCODE_FILE_API_BASE | 433,      /*!< Failed to get file status */
    QCM_FILE_NOT_EXIST = QCM_ERRCODE_FILE_API_BASE | 434,               /*!< File does not exist */
    QCM_FILE_RENAME_FAIL = QCM_ERRCODE_FILE_API_BASE | 435,             /*!< File rename failed */
    QCM_FILE_TRUNCATE_FAIL = QCM_ERRCODE_FILE_API_BASE | 436,           /*!< File truncate failed */
    QCM_FILE_NOT_OPEN = QCM_ERRCODE_FILE_API_BASE | 437,                /*!< File not opened */
    QCM_FILE_DEVICE_READONLY = QCM_ERRCODE_FILE_API_BASE | 438,         /*!< Device is in read-only mode */

    /* ZIP file operation error codes (439-442) */
    QCM_FILE_ZIP_INVALID_FILE_NAME = QCM_ERRCODE_FILE_API_BASE | 439, /*!< Invalid ZIP file name */
    QCM_FILE_ZIP_FILE_NOT_FOUND = QCM_ERRCODE_FILE_API_BASE | 440,    /*!< ZIP file not found */
    QCM_FILE_ZIP_DEL_ERROR = QCM_ERRCODE_FILE_API_BASE | 441,         /*!< ZIP file deletion error */
    QCM_FILE_ZIP_UNZIP_ERROR = QCM_ERRCODE_FILE_API_BASE | 442,       /*!< ZIP file extraction error */

    /* File creation limit error codes (450) */
    QCM_FILE_CREATE_EXCEED = QCM_ERRCODE_FILE_API_BASE | 450, /*!< Number of created files exceeds limit */

    /* Directory operation error codes (600-608) */
    QCM_FILE_DIR_MAKE_FAIL = QCM_ERRCODE_FILE_API_BASE | 600,     /*!< Directory creation failed */
    QCM_FILE_DIR_OPEN_FAIL = QCM_ERRCODE_FILE_API_BASE | 601,     /*!< Directory open failed */
    QCM_FILE_DIR_CLOSE_FAIL = QCM_ERRCODE_FILE_API_BASE | 602,    /*!< Directory close failed */
    QCM_FILE_DIR_READ_FAIL = QCM_ERRCODE_FILE_API_BASE | 603,     /*!< Directory read failed */
    QCM_FILE_DIR_TELL_FAIL = QCM_ERRCODE_FILE_API_BASE | 604,     /*!< Failed to get directory pointer position */
    QCM_FILE_DIR_SEEK_FAIL = QCM_ERRCODE_FILE_API_BASE | 605,     /*!< Directory pointer positioning failed */
    QCM_FILE_DIR_REMOVE_FAIL = QCM_ERRCODE_FILE_API_BASE | 606,   /*!< Directory deletion failed */
    QCM_FILE_DIR_ALREADY_EXIST = QCM_ERRCODE_FILE_API_BASE | 607, /*!< Directory already exists */
    QCM_FILE_DIR_NOT_EXIST = QCM_ERRCODE_FILE_API_BASE | 608,     /*!< Directory does not exist */

    QCM_FILE_ERROR_MAX,                                           /*!< Maximum error code value, used for boundary checking */
} qcm_file_error_e;

typedef enum
{
    QCM_FILE_UFS = 0, /*!< ufs */
    QCM_FILE_SFS,     /*!< sfs */
    QCM_FILE_SD_1ST,  /*!< sd */
#if 0
    QCM_FILE_SD_2ND,   /*!< sd partition2 */
    QCM_FILE_EMMC_1ST, /*!< emmc */
    QCM_FILE_EMMC_2ND, /*!< emmc partition2*/
    QCM_FILE_NAND,     /*!< nand */
    QCM_FILE_NOR,      /*!< nor */
    QCM_FILE_RAM,      /*!< ram  */
    QCM_FILE_EFS,      /*!< efs */
    QCM_FILE_EXNSFFS,  /*!< exnsffs */
    QCM_FILE_NVM,      /*!< nvm */
#endif
    QCM_FILE_FOTA, /*!< fota */
    QCM_FILE_DATA, /*!< user data */
    QCM_EXT_FLASH, /*!< external flash fs */
    QCM_FILE_MAX,
} qcm_file_type_e;

// clang-format off

/*!< add file system type below  */

#define qcm_file_type_list_default                                                                                              \
{                                                                                                                               \
    {QCM_FILE_UFS,      QCM_FILE_PRXFIX_UFS,    QCM_FILE_PRXFIX_UFS_LEN,    QCM_USER_DIR_ROOT,       QCM_USER_DIR_ROOT_LEN       },\
    {QCM_FILE_SFS,      QCM_FILE_PRXFIX_SFS,    QCM_FILE_PRXFIX_SFS_LEN,    QCM_SFS_FILE_ROOT_DIR,   QCM_SFS_FILE_ROOT_DIR_LEN   },\
    {QCM_FILE_SD_1ST,   QCM_FILE_PRXFIX_SD_1ST, QCM_FILE_PRXFIX_SD_1ST_LEN, QCM_SD1_ROOT_DIR,        QCM_SD1_ROOT_DIR_LEN        },\
    {QCM_FILE_FOTA,     QCM_FILE_PRXFIX_FOTA,   QCM_FILE_PRXFIX_FOTA_LEN,   QCM_OPEN_FOTA_ROOT_DIR,  QCM_OPEN_FOTA_ROOT_DIR_LEN  },\
    {QCM_FILE_DATA,     QCM_FILE_PRXFIX_DATA,   QCM_FILE_PRXFIX_DATA_LEN,   QCM_DATA_FILE_ROOT_DIR,  QCM_DATA_FILE_ROOT_DIR_LEN  },\
    {QCM_EXT_FLASH,     QCM_FILE_PRXFIX_EXT,    QCM_FILE_PRXFIX_EXT_LEN,    QCM_EXT_FILE_ROOT_DIR,   QCM_EXT_FILE_ROOT_DIR_LEN   },\
    {QCM_FILE_MAX,      QOSA_NULL,               0,                          QOSA_NULL,                0                           },\
};

// clang-format on

/**
 * @struct qcm_file_stat_vfs_t
 * @brief statvfs data structure, same function as struct statvfs in POSIX interface
 * @details Used to get file system statistics, including file system capacity, usage, etc.
 */
typedef struct
{
    unsigned long f_bsize;           /*!< File system block size in bytes */
    unsigned long f_frsize;          /*!< File system fragment size in bytes */
    unsigned long f_blocks;          /*!< Total number of blocks in file system, in units of f_frsize */
    unsigned long f_bfree;           /*!< Number of free blocks in file system */
    unsigned long f_bavail;          /*!< Number of free blocks available to non-superuser */
    unsigned long f_files;           /*!< Total number of file nodes in file system */
    unsigned long f_ffree;           /*!< Number of free file nodes in file system */
    unsigned long f_favail;          /*!< Number of available file nodes in file system */
    unsigned long f_fsid;            /*!< File system ID identifier */
    unsigned long f_flag;            /*!< Mount flags */
    unsigned long f_namemax;         /*!< Maximum filename length limit */
    char          f_getfree_FatFlag; /*!< FAT file system free cluster counting method flag */
} qcm_file_stat_vfs_t;

/**
 * @struct qcm_file_type_list_t
 * @brief Registered QL FILE type conversion table mapping UFS to directory path correspondence
 * @details Used for mapping configuration between file type prefixes and real paths
 */
typedef struct
{
    qcm_file_type_e file_type;     /*!< Corresponding file type enumeration value */
    char           *type_name;     /*!< File type prefix string, e.g.: "UFS:" */
    qosa_uint8_t    type_name_len; /*!< File type prefix string length */
    char           *path_name;     /*!< Corresponding real directory path */
    qosa_uint8_t    path_name_len; /*!< Real directory path length */
} qcm_file_type_list_t;

/**
 * @struct qcm_file_open_list_t
 * @brief Used to store already opened file information
 * @details Maintains linked list structure of opened files for file management and status tracking
 */
typedef struct
{
    qosa_q_link_type_t list;                                 /*!< Linked list node pointer, for maintaining opened file list */
    char               file_name[QCM_FILE_FILENAME_MAX_LEN]; /*!< Complete filename (including prefix) */
    int                file_open_mode;                       /*!< File open mode flags */
    int                fd;                                   /*!< File descriptor handle */
    qosa_bool_t        is_sfs;                               /*!< Flag indicating if it's SFS secure file system file */
    int                user_mode;                            /*!< User mode identifier for permission control */
} qcm_file_open_list_t;

/**
 * @enum qcm_file_get_size_type_e
 * @brief Enumeration for file system space size acquisition types
 */
typedef enum
{
    QCM_FS_GET_SIZE_FREE = 0, /*!< Get remaining space size */
    QCM_FS_GET_SIZE_TOTAL,    /*!< Get total space size */
} qcm_file_get_size_type_e;

typedef int Q_FILE;  /*!< File handle type, returned by qcm_file_fopen function */

typedef int Q_STATE; /*!< State return type, used for function execution status return */

/**
 * @struct QCM_FILE_DIR_T
 * @brief Directory operation handle structure
 */
typedef struct
{
    qosa_uint16_t fs_index;  /*!< Internal file system index identifier */
    qosa_uint16_t _reserved; /*!< Reserved field for byte alignment or future expansion */
} QCM_FILE_DIR_T;

/**
 * @struct qcm_file_dirent_t
 * @brief Directory entry information structure, used to store directory traversal results
 */
typedef struct
{
    int          d_ino;       /*!< inode number, file system implementation can customize usage */
    qosa_uint8_t d_type;      /*!< File type identifier (regular file, directory, etc.) */
    char         d_name[256]; /*!< Filename (without path) */
} qcm_file_dirent_t;

/**
 * @brief Corresponds to posix fopen function
 *
 * @param[in] const char * file_name
 *           - File name to open
 *
 * @param[in] const char * mode
 *           - File attribute information for opening
 *
 * @return Q_FILE
 *        - Returns file open handle
 */
Q_FILE qcm_file_fopen(const char *file_name, const char *mode);

/**
 * @brief Open file, corresponds to file operation open function
 *
 * @param[in] const char * file_name
 *           - File name to open
 *
 * @param[in] int mode
 *           - File attribute information for opening
 *
 * @param[in] qosa_bool_t open_platform
 *           - Reserved interface, whether it is OS operation file path
 *
 * @return int
 *        - Returns corresponding file handle
 */
int qcm_file_open_ex(const char *file_name, int mode, qosa_bool_t open_platform);

/**
 * @brief Close opened file handle
 *
 * @param[in] Q_FILE fd
 *          - Handle to close
 *
 * @return int
 *        - Returns corresponding file close execution result
 */
int qcm_file_fclose(Q_FILE fd);

/**
 * @brief Delete file with specified file name
 *
 * @param[in] const char * file_name
 *          - File name to delete
 *
 * @return int
 *        - Returns QCM_FILE_OK indicates execution successful
 *        - Returns other values indicate execution failed
 */
int qcm_file_remove(const char *file_name);

/**
 * @brief Read corresponding file through opened file fd
 *
 * @param[in] void * buffer
 *          - Data storage space for reading
 *
 * @param[in] qosa_size_t size
 *          - File block size to read
 *
 * @param[in] qosa_size_t num
 *          - Number of files to read
 *
 * @param[in] Q_FILE fd
 *          - File fd to operate
 *
 * @return int
 *        - Greater than 0 indicates size of file already read
 *        - Returns value less than 0 indicates execution failed
 */
int qcm_file_fread(void *buffer, qosa_size_t size, qosa_size_t num, Q_FILE fd);

/**
 * @brief Write file content of specified size through already opened FD handle
 *
 * @param[in] void * buffer
 *          - Data storage space to write
 *
 * @param[in] qosa_size_t size
 *          - Block size of data to write
 *
 * @param[in] qosa_size_t num
 *          - Number of files to write
 *
 * @param[in] Q_FILE fd
 *          - File fd to operate
 *
 * @return int
 *        - Greater than 0 indicates size of file already written
 *        - Returns value less than 0 indicates execution failed
 */
int qcm_file_fwrite(void *buffer, qosa_size_t size, qosa_size_t num, Q_FILE fd);

/**
 * @brief Used to set file offset
 *
 * @param[in] Q_FILE fd
 *           - File descriptor.
 *
 * @param[in] long offset
 *           - Offset, represents offset relative to whence
 *
 * @param[in] int whence
 *           - Starting offset position, can be one of the following three values:
 *             FILE_SEEK_SET：Offset relative to beginning of file.
 *             FILE_SEEK_CUR：Offset relative to current position.
 *             FILE_SEEK_END：Offset relative to end of file.
 *
 * @return int
 *        - Success: Returns new file offset.
 *        - Failure: -1
 */
int qcm_file_fseek(Q_FILE fd, long offset, int whence);

/**
 * @brief Used to return current file pointer position
 *
 * @param[in] Q_FILE fd
 *           - File descriptor.
 *
 * @return int
 *        - Greater than 0 indicates current file pointer position
 *        - Less than 0 indicates execution failed
 */
int qcm_file_ftell(Q_FILE fd);

/**
 * @brief Used to get file status information
 *
 * Function can get basic file information, such as file type, file permissions, owner, file size, etc.
 * This information is stored in stat structure, and specific file information can be obtained by accessing structure members
 *
 * @param[in] Q_FILE fd
 *          - File descriptor, used to specify file to get status information.
 *
 * @param[in] struct qosa_vfs_stat_t * st
 *          - Pointer to stat structure, used to store file status information.
 *
 * @return int
 *        - 0: Successfully obtained file status information.
 *         -1: Failed to get file status information.
 */
int qcm_file_fstat(Q_FILE fd, struct qosa_vfs_stat_t *st);

/**
 * @brief Used in operating system or file system. It is typically used to get file metadata,
 *   such as file size, creation time, modification time, etc.
 *
 * @param[in] const char * file_name
 *          - File path, used to specify file to get metadata.
 *
 * @param[in] struct qosa_vfs_stat_t * st
 *          - Pointer to stat structure, used to store file metadata information.
 *
 * @return int
 *       - 0: Successfully obtained file metadata.
 *        -1: Failed to get file metadata.
 */
int qcm_file_stat_func(const char *file_name, struct qosa_vfs_stat_t *st);

/**
 * @brief Get file size information
 *
 * @param[in] int fd
 *          - File descriptor, file handle to operate
 *
 * @return int
 *        - Returns current size
 *        - Returns -1 if error occurs
 */
int qcm_file_fsize(int fd);

/**
 * @brief Check if current file exists
 *
 * @param[in] const char * file_name
 *          - File name to check eg: UFS:1.txt
 *
 * @return int
 *         - QCM_FILE_OK indicates file already exists
 *         - QCM_FILE_NOT_EXIST indicates file does not exist
 */
int qcm_file_is_exist(const char *file_name);

/**
 * @brief Move file position pointer to beginning of file.
 *
 * @param[in] Q_FILE fd
 *          - File descriptor, file handle to operate
 *
 * @return int
 *         - Returns 0 indicates execution successful
 *         - Returns other values indicate execution failed
 */
int qcm_file_frewind(Q_FILE fd);

/**
 * @brief This function truncates file to specified length
 *  If current file length is less than length, file will be extended, if current
 *  file length is greater than length, file will be truncated.
 *
 * @param[in] Q_FILE fd
 *          - File descriptor, file handle to operate
 *
 * @param[in] long length
 *          - Specified length to adjust file
 *
 * @return int
 *         - 0 indicates execution successful
 *         - -1 indicates execution failed
 */
int qcm_file_ftruncate(Q_FILE fd, long length);

/**
 * @brief Used to create folder directory
 *
 * @param[in] const char * path
 *            - Path information for folder to create
 *
 * @param[in] qosa_uint32_t mode
 *            - Permission information for folder to create, default is 0
 *
 * @return int
 *        - 0 indicates execution successful
 *        - Negative number returns execution failed
 */
int qcm_file_mkdir(const char *path, qosa_uint32_t mode);

/**
 * @brief Open corresponding folder
 *
 * @param[in] const char * path
 *           - Folder path to operate
 *
 * @return QCM_FILE_DIR_T *
 *        - Returns corresponding file directory handle on success
 *        - Returns OSA_NULL on failure
 */
QCM_FILE_DIR_T *qcm_file_opendir(const char *path);

/**
 * @brief Close folder
 *
 * @param[in] QCM_FILE_DIR_T * pdir
 *          - Handle opened by corresponding opendir
 *
 * @return int
 *        - Returns FILE_OK indicates execution successful
 */
int qcm_file_closedir(QCM_FILE_DIR_T *pdir);

/**
 * @brief Read folder
 *
 * @param[in] QCM_FILE_DIR_T * pdir
 *          - Handle opened by corresponding opendir
 *
 * @return qcm_file_dirent_t *
 *       - Returns directory information if read successful
 *       - Returns OSA_NULL if read failed
 */
qcm_file_dirent_t *qcm_file_readdir(QCM_FILE_DIR_T *pdir);

/**
 * @brief Get read position of directory stream
 *
 * @param[in] QCM_FILE_DIR_T * pdir
 *          - Handle opened by corresponding opendir
 *
 * @return int
 *        - This return value represents offset from beginning of directory file
 *        - Returns -1 if error occurs
 */
int qcm_file_telldir(QCM_FILE_DIR_T *pdir);

/**
 * @brief Set next read position of directory
 *
 * @param[in] QCM_FILE_DIR_T * pdir
 *          - Handle opened by corresponding opendir
 *
 * @param[in] int offset
 *          - Represents offset from beginning of directory file
 *
 * @return int
 *        - This return value represents offset from beginning of directory file
 *        - Returns -1 if error occurs
 */
int qcm_file_seekdir(QCM_FILE_DIR_T *pdir, int offset);

/**
 * @brief Reset read position of directory to beginning
 *
 * @param[in] QCM_FILE_DIR_T * pdir
 *          - Handle opened by corresponding opendir
 *
 * @return int
 *        - This return value represents offset from beginning of directory file
 *        - Returns -1 if error occurs
 */
int qcm_file_rewinddir(QCM_FILE_DIR_T *pdir);

/**
 * @brief Delete folder
 *
 * @param[in] const char * path
 *          - Directory of folder to delete
 *
 * @return int
 *        - Returns FILE_OK indicates execution successful
 *        - Returns other values indicate failure
 */
int qcm_file_rmdir(const char *path);

/**
 * @brief Recursively delete folder, including subfiles and subfolders under the folder
 *
 * @param[in] char * path
 *          - Folder path to operate
 *
 * @return int
 *        - Returns FILE_OK indicates execution successful
 *        - Returns other values indicate failure
 */
int qcm_file_rmdir_ex(char *path);

/**
 * @brief Change file name
 *
 * @param[in] const char * old_name
 *          - Historical file name to modify
 *
 * @param[in] const char * new_name
 *          - Change to new file name
 *
 * @return int
 *       - Returns FILE_OK indicates execution successful
 *       - Returns other values indicate execution failed
 */
int qcm_file_rename(const char *old_name, const char *new_name);

/**
 * @brief Write a specified character to file stream
 *
 * @param[in] int chr
 *          - Character to operate
 *
 * @param[in] int fd
 *          - fd handle to write, handle opened by file_open
 *
 * @return int
 *        - Returns FILE_OK indicates execution successful
 *        - Returns other values indicate execution failed
 */
int qcm_file_fputc(int chr, int fd);

/**
 * @brief Write a specified string to file stream
 *
 * @param[in] const char *chr
 *          - String to operate
 *
 * @param[in] int fd
 *          - fd handle to write, handle opened by file_open
 *
 * @return int
 *        - Returns FILE_OK indicates execution successful
 *        - Returns other values indicate execution failed
 */
int qcm_file_fputs(const char *chr, int fd);

/**
 * @brief Get a character from file stream
 *
 * @param[in] int fd
 *          - File handle to operate, must be handle opened by file_open
 *
 * @return int
 *        - Returns value greater than or equal to 0 indicates get successful
 *        - Returns negative value indicates execution failed
 */
int qcm_file_fgetc(int fd);

/**
 * @brief Get string of specified character length from file stream
 *
 * @param[in] char * str
 *          - Storage pointer address for string to get
 *
 * @param[in] int n
 *          - Length information of string to get
 *
 * @param[in] int fd
 *          - File handle to operate
 *
 * @return char *
 *         - Returns OSA_NULL indicates execution failed
 *         - Returns other values indicate execution successful
 */
char *qcm_file_fgets(char *str, int n, int fd);

/**
 * @brief Write buffer data back to disk, same function as posix fsync function
 */
int qcm_file_fsync(int fd);

/**
 * @brief Format string input, same as posix fscanf function, see man fscanf
 */
int qcm_file_fscanf(int fd, const char *format, ...);

/**
 * @brief Format output data to file, same as posix fprintf function
 */
int qcm_file_fprintf(int fd, const char *format, ...);

/**
 * @brief Check if current file is already opened by file name
 *
 * @param[in] char * file_name
 *          - File name to check
 *
 * @return int
 *       - Returns corresponding file socket handle if already opened,
 *         returns -1 if other errors occur
 */
int file_check_is_opened(char *file_name);

/**
 * @brief Get count of opened file handles
 *
 * @return int
 *        - Returns actual number of opened file handles
 */
int qcm_file_open_get_cnt(void);

/**
 * @brief Get first node information in currently opened file linked list
 *
 * @return qcm_file_open_list_t *
 *        - Returns corresponding queried node information
 */
qcm_file_open_list_t *qcm_file_get_first_file_info(void);

/**
 * @brief Get next node information in current node
 *
 * @param[in] qcm_file_open_list_t * file_info
 *         - Node pointer to query
 *
 * @return qcm_file_open_list_t *
 *        - Returns corresponding queried node information
 */
qcm_file_open_list_t *qcm_file_get_next_file_info(qcm_file_open_list_t *file_info);

/**
 * @brief Function processes input file path, removes special characters and redundant parts from path, and returns processed path.
 *
 * @param[in] char * input
 *           - Full file path name to check
 *
 * @return char *
 *        - Success: Processed path string pointer. Returned pointer needs to be released with free.
 *        - Failure: Returns OSA_NULL
 *
 * @note
 *     If returned pointer is not OSA_NULL, it needs to be released with qcm_free.
 */
char *qcm_file_dedotdotify(char *input);

/**
 * @brief Check if passed file name is valid
 *
 * @param[in] char * file_name
 *          - File name to check
 *            file_name content is UFS:xxx type
 *
 * @return int
 *         - Returns OSA_OK if valid,
 *           otherwise returns other values
 */
int qcm_file_is_name_valid(char *file_name);

/**
 * @brief Check if input path information is correct, and convert path information to real path
 *
 * @param[in] char * input_path
 *            - Path information to check, UFS: type file
 *
 * @param[out] char * real_path
 *           - Real path after checking and conversion, corresponding to real /user directory
*
* @param[in/out] int * path_size
 *
 *           - Input the cache size of the path and output the actual path length
 *
 * @param[out] int * file_type
 *           - Check file type information corresponding to current path
 *
 * @return Q_STATE
 *         - Returns QCM_FILE_OK indicates data check successful
 *         - Returns negative value indicates path illegal
 */
Q_STATE qcm_file_check_path(char *input_path, char *real_path, int *path_size, int *file_type);

/**
 * @brief Get remaining space size of file path
 *
 * @param[in] const char * path
 *           - Path name to get eg:UFS:
 *
 * @return qosa_int64_t
 *        - Get actual remaining space size
 */
qosa_int64_t qcm_file_free_size(const char *path);

/**
 * @brief Get space size of file path
 *
 * @param[in] const char * path
 *           - Path name to get eg:UFS:
 *
 * @return qosa_int64_t
 *        - Get actual space size
 */
qosa_int64_t qcm_file_total_size(const char *path);

/**
 * @brief Get corresponding file type to operate by colon separation
 *
 * For example, UFS:1.txt corresponds to UFS type
 *
 * @param[in] char * param_str
 *           - To get file type
 *
 * @return qcm_file_type_e
 *        - Returns type information to get
 */
qcm_file_type_e qcm_file_get_type(char *param_str);

/**
 * @brief Get corresponding opened file mode information through already opened file fd
 *
 * @param[in] int fd
 *          - File fd handle to query
 *
 * @return int
 *        - Returns actual mode information
 *        - Returns -1 indicates query failed
 */
int qcm_file_get_mode_by_fd(int fd);

/**
 * @brief Check if current file is opened by file handle
 *
 * @param[in] int fd
 *         - File handle information to find
 *
 * @return int
 *        - Returns QCM_FILE_OK indicates file is already opened,
 *         returns other values indicates file is not opened
 */
int qcm_file_is_opened_by_fd(int fd);

/**
 * @brief Get remaining space size of file path based on fd handle
 *
 * @param[in] int fd
 *           - fd handle to get
 *
 * @return qosa_int64_t
 *        - Get actual remaining space size
 */
qosa_int64_t qcm_file_free_size_by_fd(int fd);

/**
 * @brief Get corresponding file type through already opened file fd
 *
 * @param[in] int fd
 *          - File fd information to query
 *
 * @return qcm_file_type_e
 *        - Returns actual queried file type
 *        - Returns QCM_FILE_MAX indicates not found
 */
qcm_file_type_e qcm_file_type_get_by_fd(int fd);

/**
 * @brief Convert opening types like rwb in FOPEN parameters to read-write types of open interface
 *
 * @param[in] char * mode
 *          - Mode type input by FOPEN interface
 *
 * @return int
 *        - Returns converted mode type
 */
int qcm_file_get_mode(char *mode);

/**
 * @brief Get corresponding file name through already opened file handle
 *
 * @param[in] int fd
 *          - File handle information to query
 *
 * @param[out] char * name
 *          - Returns corresponding file name
 *          - Caller must provide a buffer of at least QCM_FILE_FILENAME_MAX_LEN bytes
 *
 * @return int
 *        - Returns QCM_FILE_OK indicates execution successful,
 *          returns other values indicates execution failed
 */
int qcm_file_get_file_name_by_fd(int fd, char *name);

/**
 * @brief Get corresponding space size through file name
 *
 * @param[in] qcm_file_get_size_type_e type
 *          - Type corresponding to get size
 *
 * @param[in] const char * path
 *         - Corresponding file name to get eg UFS:
 *
 * @return qosa_int64_t
 *        - Returns actual queried size
 */
qosa_int64_t qcm_file_get_size(qcm_file_get_size_type_e type, const char *path);

/**
 * @brief Get type_name name of corresponding type
 *
 * @param[in] qcm_file_type_e type_e
 *          - Type type to query
 *
 * @return char *
 *        - Returns corresponding type name, note cannot use free function to release
 */
char *qcm_file_get_type_name(qcm_file_type_e type_e);

/**
 * @brief Get type_name name length of corresponding type
 *
 * @param[in] qcm_file_type_e type_e
 *          - Type type to query
 *
 * @return qosa_size_t
 *        - Returns corresponding type name length
 */
qosa_size_t qcm_file_get_type_name_len(qcm_file_type_e type_e);

/**
 * @brief Determine whether it is sfs operation based on converted real path or corresponding opened file fd
 *
 * @param[in] char * real_path
 *          - Pass converted real path information
 *
 * @param[in] int fd
 *          - Corresponding opened file fd handle
 *
 * @return qosa_bool_t
 *       - Returns OSA_TRUE if it is sfs file operation
 *       - Returns OSA_FALSE if not
 */
qosa_bool_t qcm_file_check_support_sfs(char *real_path, int fd);

/**
 * @brief Address conversion, convert original path to formatted path address
 *
 * @param[in] const char * path_name
 *          - Original path address, such as /fota/1.zip
 *
 * @param[in] char * type_name
 *          - Convert to address with type, such as FOTA:1.zip
 *
 * @param[in] qosa_size_t * type_name_length
 *          - Corresponding converted file name length
 *
 * @return int
 *       - Returns 0 if successful
 *       - Returns -1 if failed
 */
int qcm_file_path_type_covert(const char *path_name, char *type_name, qosa_size_t *type_name_length);

/**
 * @brief Get folder directory level, default is only level 1
 *
 * @param[in] const char * path
 *          - File to check
 *
 * @return int
 *       - Returns actual directory level if successful
 *       - Returns -1 if failed
 */
int qcm_file_get_dir_level(const char *path);

#endif /* __QCM_FILE_API_H__ */
