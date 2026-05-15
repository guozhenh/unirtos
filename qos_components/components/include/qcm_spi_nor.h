/*****************************************************************/ /**
* @file qcm_spi_nor.h
* @brief
* @author larson.li@quectel.com
* @date 2025-05-21
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-05-21 <td>1.0 <td>Larson.Li <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_SPI_NOR_H__
#define __QOSA_SPI_NOR_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "qosa_spi.h"

/**
 * @brief SPI NOR error codes
 */
typedef enum
{
    QCM_SPI_NOR_SUCESS = 0,                                           /*!< Indicates SPI NOR operation successful */
    QCM_SPI_NOR_EXECUTE_ERR = 1 | (QOSA_COMPONENT_BSP_SPI_NOR << 16), /*!< Indicates SPI_NOR operation execution failed */
    QCM_SPI_NOR_INVALID_PARAM_ERR,                                    /*!< Indicates SPI NOR operation parameter invalid */
    QCM_SPI_NOR_INIT_ERR,                                             /*!< Indicates SPI NOR operation port invalid */
    QCM_SPI_NOR_ERASE_ERR,                                            /*!< Indicates SPI NOR operation erase failed */
    QCM_SPI_NOR_READ_ERR,                                             /*!< Indicates SPI NOR operation read failed */
    QCM_SPI_NOR_WRITE_ERR,                                            /*!< Indicates SPI NOR operation write failed */
    QCM_SPI_NOR_TIMEOUT_ERR,                                          /*!< Indicates SPI NOR operation timeout */
    QCM_SPI_NOR_NOT_INITED_ERR,                                       /*!< Indicates SPI NOR not initialized */
} qcm_spi_nor_error_e;

#define QCM_SPI_NOR_WriteEnable      0x06
#define QCM_SPI_NOR_WriteDisable     0x04
#define QCM_SPI_NOR_ReadStatusReg    0x05
#define QCM_SPI_NOR_WriteStatusReg   0x01
#define QCM_SPI_NOR_ReadData         0x03
#define QCM_SPI_NOR_FastReadData     0x0B
#define QCM_SPI_NOR_FastReadDual     0x3B
#define QCM_SPI_NOR_PageProgram      0x02
#define QCM_SPI_NOR_BlockErase       0xD8
#define QCM_SPI_NOR_SectorErase      0x20
#define QCM_SPI_NOR_ChipErase        0xC7
#define QCM_SPI_NOR_PowerDown        0xB9
#define QCM_SPI_NOR_ReleasePowerDown 0xAB
#define QCM_SPI_NOR_DeviceID         0xAB
#define QCM_SPI_NOR_ManufactDeviceID 0x90
#define QCM_SPI_NOR_JedecDeviceID    0x9F

#define QCM_SPI_NOR_SECTOR_SIZE      0x1000
#define QCM_SPI_NOR_BLOCK_SIZE       0x10000
#define QCM_SPI_NOR_PARGE_SIZE       0x100
/**
 * @brief This function is used to get the Manufacturer ID of the SPI nor device.
 *
 * @param[in] qcm_spi_port_e port
 *           - SPI port enumeration, specifying the SPI port to use
 *
 * @param[out] qosa_uint8_t * id
 *           - Pointer to the buffer where the Manufacturer ID will be stored
 *
 * @return qcm_spi_nor_error_e
 *       - If the operation is successful, return QOSA_SPI_NOR_SUCESS
 *       - If failed, return the corresponding error code
 */
qcm_spi_nor_error_e qcm_spi_nor_read_id(qosa_spi_port_e port, qosa_uint8_t *id);

/**
 * @brief This function is used to initialize the SPI nor device.
 *
 * @param[in] qosa_spi_port_e port
 *           - SPI port enumeration, specifying the SPI port to use
 *
 * @param[in] qosa_spi_clk_e clk
 *           - SPI clock enumeration, specifying the SPI clock to use
 *
 * @return qcm_spi_nor_error_e
 *       - If the operation is successful, return QOSA_SPI_NOR_SUCESS
 *       - If failed, return the corresponding error code
 */
qcm_spi_nor_error_e qcm_spi_nor_init(qosa_spi_port_e port, qosa_spi_clk_e clk);

/**
 * @brief This function is used to read data from the SPI NOR device.
 *
 * @param[in] qosa_spi_port_e port
 *           - SPI port enumeration, specifying the SPI port to use
 *
 * @param[out] qosa_uint8_t *data
 *           - Pointer to the buffer where the data to be read will be stored
 *
 * @param[in] qosa_uint32_t addr
 *           - Starting address of the data to be read
 *
 * @param[in] qosa_uint32_t len
 *           - Length of the data to be read
 *
 * @return qcm_spi_nor_error_e
 *       - If the operation is successful, return QOSA_SPI_NOR_SUCESS
 *       - If failed, return the corresponding error code
 */
qcm_spi_nor_error_e qcm_spi_nor_read(qosa_spi_port_e port, qosa_uint8_t *data, qosa_uint32_t addr, qosa_uint32_t len);

/**
 * @brief This function is used to write data to the SPI NOR device.
 *
 * @param[in] qosa_spi_port_e port
 *           - SPI port enumeration, specifying the SPI port to use
 *
 * @param[in] qosa_uint8_t *data
 *           - Pointer to the data to be written
 *
 * @param[in] qosa_uint32_t addr
 *           - Starting address of the data to be written
 *
 * @param[in] qosa_uint32_t len
 *           - Length of the data to be written
 *
 * @return qcm_spi_nor_error_e
 *       - If the operation is successful, return QOSA_SPI_NOR_SUCESS
 *       - If failed, return the corresponding error code
 */
qcm_spi_nor_error_e qcm_spi_nor_write(qosa_spi_port_e port, qosa_uint8_t *data, qosa_uint32_t addr, qosa_uint32_t len);

/**
 * @brief Used for chip erase of SPI NOR flash
 *
 * @param[in] qosa_spi_port_e port
 *           - SPI port enumeration, specifying the SPI port to use
 *
 * @return qcm_spi_nor_error_e
 *       - If the operation is successful, return QOSA_SPI_NOR_SUCESS
 *       - If failed, return the corresponding error code
 */
qcm_spi_nor_error_e qcm_spi_nor_erase_chip(qosa_spi_port_e port);

/**
 * @brief Used for erasing at a specified address of SPI NOR flash
 *
 * @param[in] qosa_spi_port_e port
 *           - SPI port enumeration, specifying the SPI port to use
 *
 * @param[in] qosa_uint32_t addr
 *          - Address of the memory to be erased
 *
 * @param[in] qosa_uint32_t size
 *          - Size of the memory to be erased
 *
 * @return qcm_spi_nor_error_e
 *       - If the operation is successful, return QOSA_SPI_NOR_SUCESS
 *       - If failed, return the corresponding error code
 */
qcm_spi_nor_error_e qcm_spi_nor_erase(qosa_spi_port_e port, qosa_uint32_t addr, qosa_uint32_t size);

/**
 * @brief Used for reading and writing the status register of SPI NOR flash
 *
 * @param[in] qosa_spi_port_e port
 *           - SPI port enumeration, specifying the SPI port to use
 *
 * @param[in] qosa_uint8_t *cmd
 *          - Pointer to the command, used to specify read or write operation
 *
 * @param[out] qosa_uint8_t *data
 *          - Pointer to data, used to store the read data or send data
 *
 * @param[in] qosa_uint8_t len
 *          - Data length, indicating the number of bytes to read or write
 *
 * @return qcm_spi_nor_error_e
 *       - If the operation is successful, return QOSA_SPI_NOR_SUCESS
 *       - If failed, return the corresponding error code
 */
qcm_spi_nor_error_e qcm_spi_nor_status_reg_write_read(qosa_spi_port_e port, qosa_uint8_t *cmd, qosa_uint8_t *data, qosa_uint8_t len);

/**
 * @brief Mount the external flash LFS file system
 *
 * @param[in] qosa_uint8_t format_opt
 *           - 0: Not Format when mount the file system ( no recommand . If mount failed and without format, the file system fails to mount consistently , module will be bricked )
 *           - 1: Format when mount the file system failed ( recommanded )
 *           - 2: Format forcibly when mount the file system 
 * 
 * @return qosa_int32_t
 *          - 0: Success
 */
qosa_int32_t qcm_ext_flash_lfs_register(qosa_uint8_t format_opt);

/**
 * @brief Set the external flash Cs pin
 *
 * @param[in] qosa_uint32_t pin_num
 *           - Cs pin number
 *
 * @param[in] qosa_uint8_t func
 *          - Cs pin function
 *
 * @return qosa_int32_t
 *          - 0: Success
 */
qosa_int32_t qcm_ext_flash_set_cs(qosa_uint32_t pin_num, qosa_uint8_t func);
#endif /* __QOSA_SPI_NOR_H__ */
