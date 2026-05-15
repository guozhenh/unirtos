/*****************************************************************/ /**
* @file qcm_lwm2m_platform.h
* @brief LWM2M platform abstraction layer header file
* @author larson.li@quectel.com
* @date 2024-02-21
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2024-02-21 <td>1.0 <td>Larson.Li <td> Initial version
* </table>
**********************************************************************/
#ifndef __QCM_LWM2M_PLATFORM_H__
#define __QCM_LWM2M_PLATFORM_H__

#include "qosa_def.h"
#include "qosa_sys.h"

#include "qosa_virtual_file.h"

/**
 * @brief System memory space type
 */
typedef enum
{
    QCM_LWM2M_MEMORY_FREE_TYPE = 0, /*!< Remaining available space size */
    QCM_LWM2M_MEMORY_TOTAL_TYPE,    /*!< Total space size */
} qcm_lwm2m_memory_e;

/**
 * @brief Check if the file exists by name
 */
qosa_bool_t qcm_lwm2m_file_exist(const char *file_name);

/**
 * @brief Open file
 */
int qcm_lwm2m_file_open(const char *file_name, int mode);

/**
 * @brief Specify file pointer offset
 */
int qcm_lwm2m_file_seek(int fd, int offset, int origin);

/**
 * @brief Save corresponding file content through fd
 */
int qcm_lwm2m_file_fwrite(void *buffer, qosa_size_t size, qosa_size_t num, int fd);

/**
 * @brief Read file length content through fd
 */
int qcm_lwm2m_file_fread(void *buffer, qosa_size_t size, qosa_size_t num, int fd);

/**
 * @brief Get file size through handle
 *
 * @param[in] int fd
 *          - File handle to operate
 *
 * @return int
 *       - Return file size on success
 *       - Return negative value on failure
 */
int qcm_lwm2m_file_fsize(int fd);

/**
 * @brief LWM2M file operation, close file
 *
 * @param[in] int fd
 *         - File handle to close
 *
 * @return int
 *       - Same as posix fclose
 */
int qcm_lwm2m_file_fclose(int fd);

/**
 * @brief LWM2M file operation, delete file
 *
 * @param[in] const char *file_name
 *         - File name to delete
 *
 * @return int
 *       - Same as posix remove
 */
int qcm_lwm2m_file_remove(const char *file_name);

/**
 * @brief The purpose of this function is to get the current timezone offset value and daylight saving time offset value.
 *
 * @param[out] int * daylight_val
 *          - Return corresponding daylight saving time offset value
 *
 * @return int
 *       - Return corresponding timezone offset value
 */
qosa_int32_t qcm_lwm2m_get_time_zone(qosa_int32_t *daylight_val);

/**
 * @brief Get system memory information
 *
 * @param[in] qcm_lwm2m_memory_e type
 *          - Memory space information type to get
 *
 * @return int
 *       - Greater than 0 indicates successful acquisition
 *       - Return 0 if acquisition fails
 */
qosa_uint32_t lwm2m_get_memory_size(qcm_lwm2m_memory_e type);

/**
 * @brief Get module system software version
 *
 * @param[out] char * sw_version
 *           - Return module software version number
 *
 * @param[in] int len
 *          - Corresponding sw_version space size
 *
 */
void qcm_lwm2m_get_device_sw_version(char *sw_version, qosa_int32_t len);

/**
 * @brief Get module product unique identifier serial number, generally defaults to IMEI number
 *
 * @param[out] char * serial
 *           - Return module product unique code
 *
 * @param[in] qosa_int32_t len
 *           - Corresponding string serial space length
 */
qcm_lwm2m_error_code_e qcm_lwm2m_get_device_serial_no(char *serial, qosa_int32_t len);

/**
 * @brief Return product manufacturer representation
 *
 * @param[in] char * man_id
 *          - Return module product manufacturer
 *
 * @param[in] qosa_int32_t len
 *           - Corresponding string man_id space length
 */
void qcm_lwm2m_get_device_manufacturer_id(char *man_id, qosa_int32_t len);

/**
 * @brief Return module product hardware version number
 *
 * @param[out] char * hw_ver
 *           - Corresponding product hardware version number
 *
 * @param[in] int len
 *          - Corresponding hw_ver space size
 */
void qcm_lwm2m_get_device_hw_ver(char *hw_ver, qosa_int32_t len);

/**
 * @brief Return module firmware version number
 *
 * @param[out] char * firmware
 *           - Corresponding module firmware version number
 *
 * @param[in] int len
 *          - Corresponding firmware space size
 *
 */
void qcm_lwm2m_get_device_firmware_ver(char *firmware, qosa_int32_t len);

/**
 * @brief Return module model type
 *
 * @param[out] char * model_id
 *           - Return module model type
 *
 * @param[in] int len
 *          - Corresponding model_id string length
 */
void qcm_lwm2m_get_device_model_id(char *model_id, qosa_int32_t len);

/**
 * @brief Get sim_id and pdp_id through sim_cid
 *
 * @param[in] qosa_int32_t sim_cid
 *          - Parameter sim and profile combination value
 *
 * @param[out] qosa_int32_t *sim_id
 *          - Parameter Get sim sequence number
 *
 * @param[out] qosa_int32_t *profile_idx
 *          - Parameter Get PDP context id sequence number
 *
 * @return qcm_lwm2m_error_code_e
 *       -  0 indicates successful acquisition
 *       - Return -1 if acquisition fails
 */
qcm_lwm2m_error_code_e qcm_lwm2m_platform_get_sim_and_profile(qosa_int32_t sim_cid, qosa_int32_t *sim_id, qosa_int32_t *profile_idx);

/**
 * @brief Get current network system
 *
 * @param[in] void
 *          - Parameter none
 *
 * @return qosa_int32_t
 *       - Greater than 0 indicates successful acquisition
 *       - Return -1 if acquisition fails
 */
qosa_int32_t qcm_lwm2m_get_current_bearer_type(qosa_int32_t sim_id, qosa_uint8_t *tddfdd);

/**
 * @brief Get module IMEI
 *
 * @param[out] char *imei
 *          - Parameter Store acquired IMEI number
 *
 * @param[in] qosa_int32_t len
 *          - Parameter Cache IMEI number limit length
 *
 * @return qcm_lwm2m_error_code_e
 *       -  0 indicates successful acquisition
 *       - Return -1 if acquisition fails
 */
qcm_lwm2m_error_code_e qcm_lwm2m_platform_get_imei(char *imei, qosa_int32_t len, qosa_int32_t sim_id);

/**
* @brief Get module IMSI
*
* @param[out] char *imsi
*          - Parameter Store acquired IMSI number
*
* @param[in] qosa_int32_t len
*          - Parameter Cache IMSI number limit length
*
* @return qcm_lwm2m_error_code_e
*       -  0 indicates successful acquisition
*       - Return -1 if acquisition fails
*/
qcm_lwm2m_error_code_e qcm_lwm2m_platform_get_imsi(qosa_int32_t sim_id, char *imsi, qosa_int32_t len);

/**
 * @brief Query software tool version number
 *
 * @param[out] char * tool_ver
 *           - Corresponding software tool version number
 *
 * @param[in] int len
 *          - Corresponding tool_ver space size
 */
void qcm_lwm2m_get_soft_tool_ver(char *tool_ver, qosa_int32_t len);

/**
* @brief Get SIM phone number
*
* @param[in] qosa_int32_t sim_id
*          - Parameter sim sequence number
*
* @param[out] char *phonenumber
*          - Parameter Store acquired phonenumber number
*
* @param[in] qosa_int32_t len
*          - Parameter Cache phonenumber number limit length
*
* @return qcm_lwm2m_error_code_e
*       -  0 indicates successful acquisition
*       - Return -1 if acquisition fails
*/
qcm_lwm2m_error_code_e qcm_lwm2m_platform_get_phonenumber(qosa_int32_t sim_id, char *phonenumber, qosa_int32_t len);

/**
* @brief Get uplink and downlink traffic
*
* @param[in] qosa_int32_t sim_id
*          - Parameter sim sequence number
*
* @param[out] qosa_uint64_t *tx_cnt
*          - Parameter Get uplink traffic
*
* @param[in] qosa_uint64_t *rx_cnt
*          - Parameter Get downlink traffic
*
* @return qcm_lwm2m_error_code_e
*       -  0 indicates successful acquisition
*       - Other acquisition failures
*/
qcm_lwm2m_error_code_e qcm_lwm2m_platform_get_gdcnt_count(qosa_int32_t sim_id, qosa_uint64_t *tx_cnt, qosa_uint64_t *rx_cnt);

/**
* @brief Get current SIM card APN
*
* @param[in] qosa_uint16_t pdp_cid
*          -  Parameter PDP context id
*
* @param[out] char *apn
*          -  Parameter Return APN
*
* @param[in] qosa_int32_t size
*          - Parameter APN cache length
*
* @return qcm_lwm2m_error_code_e
*       -  0 indicates successful acquisition
*       - Other acquisition failures
*/
qcm_lwm2m_error_code_e qcm_lwm2m_platform_get_apn(qosa_uint8_t sim_id, qosa_uint8_t pdp_cid, char *apn, qosa_int32_t size);

/**
 * @brief Module restart
 */
qcm_lwm2m_error_code_e qcm_lwm2m_platform_reboot_system(void);

/**
* @brief Calculate simple integer power - used for reconnection/retransmission delay calculation
*/
qosa_int32_t qcm_lwm2m_platform_pow(qosa_int32_t base, qosa_int32_t exponent);

/**
 * @brief Get default pdp cid
 *
 * @param[out] qosa_uint8_t *pdp_cid
 *          - Parameter Store acquired pdp cid number
 *
 * @return qcm_lwm2m_error_code_e
 *       -  0 indicates successful acquisition
 *       - Return -1 if acquisition fails
 */
qcm_lwm2m_error_code_e qcm_lwm2m_platform_get_pdp_cid(qosa_uint8_t *pdp_cid);

/**
 * @brief Set DNS TTL
 *
 * @param[in] qosa_uint32_t ttl
 *          - Parameter DNS TTL time value
 * @note AEP Default maximum value. Reduce the number of DNS request resolutions
 */
void qcm_lwm2m_dns_set_ttl(qosa_uint32_t ttl);

/**
 * @brief Device information initialization - pass to external configuration
 */
qcm_lwm2m_device_info_t *qcm_lwm2m_device_init(void);

#endif /* __QCM_LWM2M_PLATFORM_H__ */
