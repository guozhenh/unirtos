/*****************************************************************/ /**
* @file qosa_fota.h
* @brief FOTA operation interface
* @author larson.li@quectel.com
* @date 2024-06-13
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-06-13 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_FOTA_H__
#define __QOSA_FOTA_H__

#include "qosa_def.h"
#include "qosa_sys.h"

typedef enum
{
    QOSA_FOTA_OK = 0,
    QOSA_FOTA_OPEN_ERROR = -1,          /*!< FOTA file open error */
    QOSA_FOTA_WRITE_ERROR = -2,         /*!< FOTA data write error */
    QOSA_FOTA_READ_ERROR = -3,          /*!< FOTA data read error */
    QOSA_FOTA_ERASE_ERROR = -4,         /*!< FOTA data read error */
    QOSA_FOTA_COMPARE_ERROR = -5,       /*!< FOTA data read error */
    QOSA_FOTA_SEEK_ERROR = -6,          /*!< FOTA seek error */
    QOSA_FOTA_UPDATE_ERROR = -7,        /*!< Upgrade failed */
    QOSA_FOTA_PARAM_ERROR = -8,         /*!< Input parameter error */
    QOSA_FOTA_IMAGE_VERIFY_ERROR = -9,  /*!< Image verification failed */
    QOSA_FOTA_CODE_GENERAL_ERROR = -10, /*!< FOTA upgrade internal error */
    QOSA_FOTA_NOT_EXIST_ERROR = -11,    /*!< File does not exist */
    QOSA_FOTA_OUT_OF_MEMORY = -12,      /*!< Insufficient memory */
    QOSA_FOTA_PARTITION_ERROR = -13,    /*!< Partition error */
    QOSA_FOTA_CLOSE_ERROR = -14,        /*!< Close failed */
    QOSA_FOTA_RENAME_ERROR = -15,       /*!< Rename failed */
    QOSA_FOTA_BUF_NOT_ENOUGH = -16,     /*!< Insufficient buffer */
    QOSA_FOTA_FLASH_READ_ERROR = -17,   /*!< FLASH data read error */
    QOSA_FOTA_FLASH_WRITE_ERROR = -18,  /*!< FLASH data write error */
    QOSA_FOTA_FLASH_ERASE_ERROR = -19,  /*!< FLASH data erase error */
    QOSA_FOTA_VERIFY_NOT_ENABLE = -20,  /*!< Fota verify not enable */
    QOSA_FOTA_NOT_SUPPORT_ERROR = -21,  /*!< Fota NOT support */
} qosa_fota_errno_e;
typedef enum
{
    QOSA_FOTA_VERIFY_NOT_SUPPORT = -1,
    QOSA_FOTA_VERIFY_DISABLE = 0,
    QOSA_FOTA_VERIFY_ENABLE = 1, 
} qosa_fota_verify_control_e;

typedef enum
{
    // Currently, only the cmd for setting the fota mode has been added, and additional ones can be added as needed in the future
    QOSA_FOTA_OPT_CMD_SET_FOTA_MODE = 0,  // Set Upgrade Mode
}qosa_fota_opt_cmd_e;

typedef enum
{
    // DFOTA uses the original factory differential upgrade, while fullfota uses the full upgrade newly added by Quec
    QOSA_FOTA_DFOTA_MODE = 0, // Default DFOTA
    QOSA_FOTA_FULL_MODE, // Full FOTA
}qosa_fota_mode_e;

/**
 * @brief FOTA operation handle for platform side
 */
typedef void *qosa_fota_t;

/**
 * @brief Initialize and apply for internal FOTA resource information
 *
 * @param[in] char * fota_name
 *          - User location and name for saving FOTA firmware package
 *
 * @param[in] qosa_bool_t del_old_file
 *          - Whether to delete historical files, note that if upgrading through existing UFS files, it should be FALSE not to delete
 *
 * @return osa_fota_manage_t *
 *       - Return corresponding FOTA management handle pointer if successful
 *       - Return OSA_NULL if failed
 */
qosa_fota_t *qosa_fota_init(char *fota_name, qosa_bool_t del_old_file);

/**
 * @brief Release and destroy FOTA upgrade related resources
 *
 * @param[in] qosa_fota_t * fota
 *          - FOTA address pointer corresponding to init initialization
 *
 */
void qosa_fota_deinit(qosa_fota_t *fota);

/**
 * @brief Get FOTA signature verification settings (enable/disable)
 *
 * @return qosa_fota_verify_control_e
 *       - Signature verification has been disabled. Return to QOSA_FOTA_VERIFY_DISABLE
 *       - Signature verification has been enabled. Return QOSA_FOTA_VERIFY_ENABLE
 */
qosa_fota_verify_control_e qosa_get_fota_verify_config(void);

/**
 * @brief Enable/Disable FOTA signature verification
 *
 * @param[in] qosa_fota_verify_control_e value
 *          - Set to TRUE to enable FOTA signature verification, and set to FALSE to disable FOTA signature verification.
 *
 * @return qosa_fota_errno_e
 *       - Return OSA_FOTA_OK if successful
 *       - Return other values if failed
 */
qosa_fota_errno_e qosa_set_fota_verify_config(qosa_fota_verify_control_e value);

/**
 * @brief Verify whether the downloaded image information is correct
 *
 * @param[in] qosa_fota_t * fota
 *          - FOTA address pointer corresponding to init initialization
 *
 * @return qosa_fota_errno_e
 *       - Return OSA_FOTA_OK if successful
 *       - Return other values if failed
 */
qosa_fota_errno_e qosa_fota_verify_image(qosa_fota_t *fota);

/**
 * @brief Get partition space size to check if it meets data writing requirements
 *
 * @param[in] qosa_fota_t * fota
 *          - FOTA address pointer corresponding to init initialization
 *
 * @return qosa_int64_t
 *       - Return actual space size
 *       - Return -1 if failed
 */
qosa_int64_t qosa_fota_get_partition_space(qosa_fota_t *fota);

/**
 * Get FOTA upgrade status after restart
 */
qosa_fota_errno_e qosa_fota_get_update_result(void);

/**
 * @brief FOTA data packet writing
 *
 * @param[in] qosa_fota_t * fota
 *          - FOTA address pointer corresponding to init initialization
 *
 * @param[in] qosa_uint8_t * payload
 *          - Corresponding upgrade data content
 *
 * @param[in] qosa_size_t payload_len
 *          - Corresponding payload data packet length
 *
 * @return qosa_fota_errno_e
 *       - Return OSA_FOTA_OK if successful
 *       - Return other values if failed
 */
qosa_fota_errno_e qosa_fota_write_packet_data(qosa_fota_t *fota, qosa_uint8_t *payload, qosa_size_t payload_len);

/**
 * @brief Set the number of URC reports reported by the underlying bootloader during upgrade
 *
 * If the platform does not need it, it can be not adapted
 *
 * @param[in] qosa_uint8_t update_urc_num
 *          - Set the number of update reports
 *
 * @return qosa_fota_errno_e
 *       - Return OSA_FOTA_OK if successful
 *       - Return other values if failed
 */
qosa_fota_errno_e qosa_fota_set_update_urc_num(qosa_fota_t *fota, qosa_uint8_t update_urc_num);

/**
 * @brief Get the current file storage space size
 *
 * @param[in] qosa_fota_t * fota
 *          - FOTA address pointer corresponding to init initialization
 *
 * @return qosa_int64_t
 *       - Return current file size
 */
qosa_int64_t qosa_fota_get_current_file_size(qosa_fota_t *fota);

/**
 * @brief Return FOTA firmware package write name
 *
 * When adapting, can return empty *length = 0
 *
 * @param[out] char * packname
 *           - Return corresponding firmware package name
 *
 * @param[in/out] int * length
 *           - Return firmware package data length
 *
 * @return qosa_fota_errno_e
 *       - Return OSA_FOTA_OK if successful
 *       - Return other values if failed
 */
qosa_fota_errno_e qosa_fota_get_default_packname(char *packname, int *length);

/**
 * @brief Set fota pack file name
 *
 * @param[in] char * packetname
 *          - Set default pack file name, note that the name here is the real folder path
 *
 * @param[in] int length
 *          - Corresponding packet name length
 *
 * @return qosa_fota_errno_e
 *       - Return OSA_FOTA_OK if successful
 *       - Return other values if failed
 */
qosa_fota_errno_e qosa_fota_set_packname(char *packetname, int length);

/**
 * @brief Delete fota file, please ensure the file is closed, if the file is opened, it will fail
 *
 */
void qosa_fota_file_reset(void);

/**
 * @brief Set FOTA upgrade flag
 *
 * @note If the platform needs to set flag separately, adapt it, otherwise it is empty, if more parameters are needed later, it can be modified to a structure
 */
void qosa_fota_flag_set(void);

/**
 * @brief Get whether it has been checked if the data contains special packet headers
 *
 * @return int
 *       - Return firmware package check flag, download firmware type mode can only be distinguished by firmware package data parsing
 *
 * @note Upper layer handles URC reporting
 */
qosa_bool_t qosa_fota_get_updater_info(void);

/**
 * @brief Start updater upgrade
 *
 * @note Currently only used by eigen platform, other platforms may not adapt, directly return QOSA_FOTA_OK
 */
int qosa_fota_updater_start(void);

/**
 * @brief Set fota option
 *
 * @param[in] qosa_fota_opt_cmd_e cmd
 *          - Set fota option cmd
 *
 * @param[in] void *param
 *          - corresponds to the parameter passed to cmd 
 *
 * @return qosa_fota_errno_e
 *       - Return OSA_FOTA_OK if successful
 *       - Return other values if failed
 */
qosa_fota_errno_e qosa_fota_set_opt(qosa_fota_opt_cmd_e cmd, void *param);
#endif /* __QOSA_FOTA_H__ */
