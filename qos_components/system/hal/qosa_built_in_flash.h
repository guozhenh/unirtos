/*****************************************************************/ /**
* @file qosa_built_in_flash.h
* @brief Built-in Flash operation interface
* @author larson.li@quectel.com
* @date 2025-05-23
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-05-23 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_BUILT_IN_FLASH_H__
#define __QOSA_BUILT_IN_FLASH_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/*===========================================================================
 * Enum
===========================================================================*/
/**
 * @enum qosa_built_nor_error_e
 * @brief SPI NOR error codes
 */
typedef enum
{
    QOSA_BUILT_NOR_SUCESS = 0,                                             /*!< Indicates built-in flash operation successful */
    QOSA_BUILT_NOR_EXECUTE_ERR = 1 | (QOSA_COMPONENT_BSP_BUILT_NOR << 16), /*!< Indicates built-in flash operation execution failed */
    QOSA_BUILT_NOR_INVALID_PARAM_ERR,                                      /*!< Indicates built-in flash operation parameter invalid */
    QOSA_BUILT_NOR_READ_ERR,                                               /*!< Indicates built-in flash operation read failed */
    QOSA_BUILT_NOR_WRITE_ERR,                                              /*!< Indicates built-in flash operation write failed */
    QOSA_BUILT_NOR_ERASE_ERR,                                              /*!< Indicates built-in flash operation erase failed */
} qosa_built_nor_error_e;

/*===========================================================================
 * Function
===========================================================================*/
/**
 * @brief Validates the erase/write memory region.
 *
 * @param[in] SectorAddress
 *            Start address of the erase/write region.
 *
 * @param[in] Size
 *            Size of the erase/write operation.
 *
 * @return true  Operation succeeded.
 * @return false Operation failed.
 */
qosa_bool_t qosa_flash_limit_allow_check(qosa_uint32_t SectorAddress, qosa_uint32_t size);

/**
 * @brief This function is used to read data from built-in flash.
 *
 * @param[in] qosa_uint32_t addr
 *           - Starting address to read data
 *
 * @param[out] qosa_uint8_t *data
 *           - Pointer to store the data to be read
 *
 * @param[in] qosa_uint32_t len
 *           - Length of data to read
 *
 * @return qosa_built_nor_error_e
 *       - If operation is successful, return QOSA_BUILT_NOR_SUCESS
 *       - Return corresponding error code if failed
 */
qosa_built_nor_error_e qosa_builtin_flash_read(qosa_uint32_t addr, qosa_uint8_t *data, qosa_uint32_t len);

/**
 * @brief This function is used to write data to built-in flash.
 *
 *
 * @param[in] qosa_uint8_t *data
 *           - Pointer to the data to be written
 *
 * @param[in] qosa_uint32_t addr
 *           - Starting address to write data
 *
 * @param[in] qosa_uint32_t len
 *           - Length of data to write
 *
 * @return qosa_built_nor_error_e
 *       - If operation is successful, return QOSA_BUILT_NOR_SUCESS
 *       - Return corresponding error code if failed
 */
qosa_built_nor_error_e qosa_builtin_flash_write(qosa_uint32_t addr, qosa_uint8_t *data, qosa_uint32_t len);

/**
 * @brief Used to erase built-in flash at specified address
 *
 * @param[in] qosa_uint32_t addr
 *          - Address of memory to erase
 *
 * @param[in] qosa_uint32_t len
 *          - Size of memory to erase
 *
 * @return qosa_built_nor_error_e
 *       - If operation is successful, return QOSA_BUILT_NOR_SUCESS
 *       - Return corresponding error code if failed
 */
qosa_built_nor_error_e qosa_builtin_flash_erase(qosa_uint32_t addr, qosa_uint32_t len);

#endif /* __QOSA_BUILT_IN_FLASH_H__ */
