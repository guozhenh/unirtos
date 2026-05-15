/*****************************************************************/ /**
* @file qosa_spi.h
* @brief SPI operation interface
* @author bronson.zhan@quectel.com
* @date 2025-05-13
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-05-13 <td>1.0 <td>Bronson.Zhan <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_SPI_H__
#define __QOSA_SPI_H__

#include "qosa_def.h"
#include "qosa_sys.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
#define QOSA_SPI_ERRCODE_BASE (QOSA_COMPONENT_API_SPI << 16)

/*===========================================================================
 * Enum
===========================================================================*/
/**
  * @enum  qosa_spi_errcode_e
  * @brief SPI error code definition
  */
typedef enum
{
    QOSA_SPI_SUCCESS = 0,
    QOSA_SPI_EXECUTE_ERR = 1 | QOSA_SPI_ERRCODE_BASE, /*!< Execution error */
    QOSA_SPI_MEM_ADDR_NULL_ERR,                       /*!< Memory address is null */
    QOSA_SPI_INVALID_PARAM_ERR,                       /*!< Invalid parameter */
    QOSA_SPI_WRITE_READ_ERR,                          /*!< Read/Write failed */
} qosa_spi_errcode_e;

/**
  * @enum  qosa_spi_clk_e
  * @brief SPI transmission rate configuration
  */
typedef enum
{
    /* @tips The frequency configured here is the typical value supported by SPI, actually specific values can also be configured, need to calculate actual frequency by yourself (refer to QOSA_SPI_IOCTL_SET_CLOCK_FREQUENCY configuration description) */

    QOSA_SPI_CLK_INVALID = -1,       /*!< Invalid clock frequency configuration */

    QOSA_SPI_CLK_812_5KHZ = 812500,  /*!< Clock frequency: 812.5K */
    QOSA_SPI_CLK_1_625MHZ = 1625000, /*!< Clock frequency: 1.625M */
    QOSA_SPI_CLK_3_25MHZ = 3250000,  /*!< Clock frequency: 3.125M */
    QOSA_SPI_CLK_6_5MHZ = 6500000,   /*!< Clock frequency: 6.5M */
    QOSA_SPI_CLK_13MHZ = 13000000,   /*!< Clock frequency: 13M */
    QOSA_SPI_CLK_26MHZ = 26000000,   /*!< Clock frequency: 26M */
    QOSA_SPI_CLK_52MHZ = 52000000,   /*!< Clock frequency: 52M */
} qosa_spi_clk_e;

/**
  * @enum  qosa_spi_transfer_mode_e
  * @brief SPI transmission mode
  */
typedef enum
{
    QOSA_SPI_TRANSMIT_POLLING = 0, /*!< FIFO read/write */
    QOSA_SPI_TRANSMIT_DMA,         /*!< DMA read/write */
} qosa_spi_transmit_mode_e;

/**
  * @enum  qosa_spi_port_e
  * @brief SPI port selection
  */
typedef enum
{
    QOSA_SPI_PORT_NONE = -1,
    QOSA_SPI_PORT0, /*!< SPI0 */
    QOSA_SPI_PORT1, /*!< SPI1 */
    QOSA_SPI_PORT2, /*!< SPI2 */
    QOSA_SPI_PORT_MAX,
} qosa_spi_port_e;

/**
  * @enum  qosa_spi_mode_e
  * @brief SPI master/slave mode setting
  */
typedef enum
{
    QOSA_SPI_MODE_MASTER = 0, /*!< Master mode (default) */
    QOSA_SPI_MODE_SLAVE,      /*!< Slave mode */
} qosa_spi_mode_e;

/**
  * @enum  qosa_spi_frame_format_e
  * @brief SPI clock polarity and phase configuration
  */
typedef enum
{
    QOSA_SPI_CLK_CPOL0_CPHA0 = 0, /*!< Clock Polarity 0 / Clock Phase 0 (default) */
    QOSA_SPI_CLK_CPOL0_CPHA1,     /*!< Clock Polarity 0 / Clock Phase 1 */
    QOSA_SPI_CLK_CPOL1_CPHA0,     /*!< Clock Polarity 1 / Clock Phase 0 */
    QOSA_SPI_CLK_CPOL1_CPHA1,     /*!< Clock Polarity 1 / Clock Phase 1 */
} qosa_spi_frame_format_e;

/**
  * @enum  qosa_spi_nss_mode_e
  * @brief SPI chip select signal control method
  */
typedef enum
{
    QOSA_SPI_NSS_MASTER_HARDWARE = 0, /*!< Master mode hardware control CS pin (default) */
    QOSA_SPI_NSS_MASTER_SOFTWARE,     /*!< Master mode software control CS pin (bottom layer does not control, need to control GPIO by yourself) */
    QOSA_SPI_NSS_SLAVE_HARDWARE,      /*!< Slave mode hardware control CS pin */
} qosa_spi_nss_mode_e;

/**
  * @enum  qosa_spi_bit_order_e
  * @brief SPI data transmission bit order
  */
typedef enum
{
    QOSA_SPI_MSB_FIRST = 0, /*!< SPI receive/transmit data uses MSB as starting bit (default) */
    QOSA_SPI_LSB_FIRST,     /*!< SPI receive/transmit data uses LSB as starting bit */
} qosa_spi_bit_order_e;
/**
  * @enum  qosa_spi_data_width_e
  * @brief SPI data width configuration
  */
typedef enum
{
    QOSA_SPI_WIDTH_1_BYTES = 0, /*!< SPI transmit data bit width is 1bytes(8bits) (default) */
    QOSA_SPI_WIDTH_2_BYTES,     /*!< SPI transmit data bit width is 2bytes(16bits) */
} qosa_spi_data_width_e;

/**
 * @enum  qosa_spi_ioctl_cmd_e
 * @brief SPI ioctl command definition
 */
typedef enum
{
    QOSA_SPI_IOCTL_NONE,
    QOSA_SPI_IOCTL_SET_MODE,               /*!< Set SPI mode
                                                - This configuration can be the value of @ref qosa_spi_mode_e */
    QOSA_SPI_IOCTL_SET_CLK_POLARITY_PHASE, /*!< Set SPI clock polarity and phase
                                                - This configuration can be the value of @ref qosa_spi_frame_format_e */
    QOSA_SPI_IOCTL_SET_NSS_MODE,           /*!< Set NSS signal mode
                                                - This configuration can be the value of @ref qosa_spi_nss_mode_e */
    QOSA_SPI_IOCTL_SET_CLOCK_FREQUENCY,    /*!< Set SPI clock frequency(qosa_uint32_t)
                                                - This configuration will calculate the supported clock frequency based on the value entered by the customer and configure it
                                                  EIGEN platform:
                                                        Master clock frequency: 26M/612M(@tips maximum can support 52M needs frequency division)
                                                        Division factor = (cpsr(2*2n) (range[2,254])*scr(n) (range[1,256])) (division factor range [2, 254*256]) */
    QOSA_SPI_IOCTL_SET_BIT_ORDER,          /*!< Set data transmission bit order
                                                - This configuration can be the value of @ref qosa_spi_bit_order_e */
    QOSA_SPI_IOCTL_SET_DATA_WIDTH,         /*!< Set data transmission bit width
                                                - This configuration can be the value of @ref qosa_spi_data_width_e */
} qosa_spi_ioctl_cmd_e;

/**
 * @brief SPI channel initialization
 *
 * @param[in] qosa_spi_port_e port
 *          - SPI channel selection
 *
 * @param[in] qosa_spi_transmit_mode_e mode
 *          - SPI transmission mode selection
 *
 * @param[in] qosa_spi_clk_e clk
 *           - SPI transmission rate clock configuration
 *
 * @return qosa_spi_errcode_e
 *          - QOSA_SPI_SUCCESS: Success
 *          - other: Error code
 */
qosa_spi_errcode_e qosa_spi_init(qosa_spi_port_e port, qosa_spi_transmit_mode_e mode, qosa_spi_clk_e clk);

/**
 * @brief SPI channel deinitialization
 *
 * @param[in] qosa_spi_port_e port
 *          - SPI channel selection
 *
 * @return qosa_spi_errcode_e
 *          - QOSA_SPI_SUCCESS: Success
 *          - other: Error code
 */
qosa_spi_errcode_e qosa_spi_deinit(qosa_spi_port_e port);

/**
 * @brief SPI send data
 *
 * @param[in] qosa_spi_port_e port
 *          - SPI channel selection
 *
 * @param[in] void *txData
 *          - Address of data to be sent (internal determines type based on bit width, bit width = 1(uint_8) or 2(uint_16))
 *
 * @param[in] qosa_uint32_t length
 *           - Length of data to be sent/read
 *
 * @return qosa_spi_errcode_e
 *          - QOSA_SPI_SUCCESS: Success
 *          - other: Error code
 */
qosa_spi_errcode_e qosa_spi_write(qosa_spi_port_e port, void *txData, qosa_uint32_t length);

/**
 * @brief SPI receive data
 *
 * @param[in] qosa_spi_port_e port
 *          - SPI channel selection
 *
 * @param[out] void *rxData
 *          - Address of data to be read (internal determines type based on bit width, bit width = 1(uint_8) or 2(uint_16))
 *
 * @param[in] qosa_uint32_t length
 *           - Length of data to be read
 *
 * @return qosa_spi_errcode_e
 *          - QOSA_SPI_SUCCESS: Success
 *          - other: Error code
 */
qosa_spi_errcode_e qosa_spi_read(qosa_spi_port_e port, void *rxData, qosa_uint32_t length);

/**
 * @brief SPI trigger send and receive data transmission
 *
 * @param[in] qosa_spi_port_e port
 *          - SPI channel selection
 *
 * @param[out] void *rxData
 *          - Address of data to be read (internal determines type based on bit width, bit width = 1(uint_8) or 2(uint_16))
 *
 * @param[in] void *txData
 *          - Address of data to be sent (internal determines type based on bit width, bit width = 1(uint_8) or 2(uint_16))
 *
 * @param[in] qosa_uint32_t length
 *           - Length of data to be sent/read
 *
 * @return qosa_spi_errcode_e
 *          - QOSA_SPI_SUCCESS: Success
 *          - other: Error code
 */
qosa_spi_errcode_e qosa_spi_write_read(qosa_spi_port_e port, void *rxData, void *txData, qosa_uint32_t length);

/**
 * @brief Register SPI event handler
 *
 * @param[in] qosa_spi_port_e port
 *          - SPI channel selection
 *
 * @param[in] qosa_spi_ioctl_cmd_e cmd
 *          - Corresponding ioctl command
 *
 * @param[in/out] void * arg
 *           - Corresponding ioctl command parameters
 *
 * @return qosa_spi_errcode_e
 *          - QOSA_SPI_SUCCESS: Success
 *          - other: Error code
 */
qosa_spi_errcode_e qosa_spi_ioctl(qosa_spi_port_e port, qosa_spi_ioctl_cmd_e cmd, void *arg);

#endif /* __QOSA_SPI_H__ */
