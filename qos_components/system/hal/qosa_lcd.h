/*****************************************************************/ /**
* @file qosa_lcd.h
* @brief Lcd operation interface
* @author bronson.zhan@quectel.com
* @date 2025-10-13
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-10-13 <td>1.0 <td>Bronson.Zhan <td> Init
* </table>
**********************************************************************/
#ifndef __QOSA_LCD_H__
#define __QOSA_LCD_H__

#include "qosa_def.h"
#include "qosa_sys.h"

#include "qosa_gpio.h"
#include "qosa_spi.h"
#include "qosa_camera.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/

#define QOSA_LCD_ERRCODE_BASE (QOSA_COMPONENT_API_LCD << 16)

/*===========================================================================
 * Enum
===========================================================================*/
/**
 * @enum qosa_lcd_error_e
 * @brief lcd error code.
 */
typedef enum
{
    QOSA_LCD_SUCCESS = 0,

    QOSA_LCD_EXECUTE_ERR = 1 | QOSA_LCD_ERRCODE_BASE, /*!< LCD execution error */
    QOSA_LCD_MEM_ADDR_NULL_ERR,                       /*!< Memory address is null */
    QOSA_LCD_INVALID_PARAM_ERR,                       /*!< Invalid parameter */
    QOSA_LCD_MEM_LOW_ERR,                             /*!< Insufficient memory */
    QOSA_LCD_INIT_REPEAT_ERR,                         /*!< LCD repeated initialization */
    QOSA_LCD_INIT_ERR,                                /*!< LCD initialization error */
    QOSA_LCD_NO_INIT_ERR,                             /*!< LCD didn't initialization */
    QOSA_LCD_OPTION_NOT_SUPPORT_ERR,                  /*!< LCD configuration is not supported */
    QOSA_LCD_OPTION_NOT_FIND_ERR,                     /*!< LCD configuration not find */
    QOSA_LCD_INPUT_FORMAT_NOT_SUPPORT_ERROR,          /*!< Input format is not supported */
    QOSA_LCD_OUTPUT_FORMAT_NOT_SUPPORT_ERROR,         /*!< Output format is not supported */
    QOSA_LCD_SEM_CREATE_ERROR,                        /*!< Trying create semaphore error */
    QOSA_LCD_SEM_WAIT_ERROR,                          /*!< Trying wait semaphore error */
    QOSA_LCD_MUTEX_NOT_CREATE_ERROR,                  /*!< Trying find mutex error */
} qosa_lcd_error_e;

/**
 * @enum qosa_lcd_channel_e
 * @brief lcd channel enum.
 */
typedef enum
{
    QOSA_LCD_SPECIAL_PORT = 0, /*!< LCD special controler */
    QOSA_LCD_SPI_PORT_0,       /*!< Standard SPI port 0 */
    QOSA_LCD_SPI_PORT_1,       /*!< Standard SPI port 1 */
    QOSA_LCD_SPI_PORT_2,       /*!< Standard SPI port 2 */
    QOSA_LCD_PORT_MAX,
} qosa_lcd_channel_e;

/**
  * @enum  qosa_lcd_input_format_opt_e
  * @brief The original format of the data.
  */
typedef enum
{
    QOSA_LCD_INPUT_FORMAT_RGB565 = 0,    /*!< RGB565 format input(default) */
    QOSA_LCD_INPUT_FORMAT_RGB444,        /*!< RGB444 format input */
    QOSA_LCD_INPUT_FORMAT_RGB555,        /*!< RGB555 format input */
    QOSA_LCD_INPUT_FORMAT_RGB666,        /*!< RGB666 format input */
    QOSA_LCD_INPUT_FORMAT_RGB888,        /*!< RGB888 format input */
    QOSA_LCD_INPUT_FORMAT_GREY,          /*!< ONLY Y format input */
    QOSA_LCD_INPUT_FORMAT_YUV422,        /*!< YUV422 format input */
    QOSA_LCD_INPUT_FORMAT_YUV420,        /*!< YUV420 format input */
    QOSA_LCD_INPUT_FORMAT_YUV400,        /*!< YUV400 format input */
    QOSA_LCD_INPUT_FORMAT_YUV420_3PLANE, /*!< YUV420 3-plane format input */
    QOSA_LCD_INPUT_FORMAT_MAX,
} qosa_lcd_input_format_opt_e;

/**
  * @enum  qosa_lcd_output_format_opt_e
  * @brief The target format of the data.
  */
typedef enum
{
    QOSA_LCD_OUTPUT_FORMAT_RGB565 = 0, /*!< RGB565 format output(default) */
    QOSA_LCD_OUTPUT_FORMAT_RGB444,     /*!< RGB444 format output */
    QOSA_LCD_OUTPUT_FORMAT_RGB666,     /*!< RGB666 format output */
    QOSA_LCD_OUTPUT_FORMAT_RGB888,     /*!< RGB888 format output */
    QOSA_LCD_OUTPUT_FORMAT_MAX,
} qosa_lcd_output_format_opt_e;

/**
  * @enum  qosa_lcd_spi_mode_e
  * @brief lcd SPI mode.
  */
typedef enum
{
    QOSA_LCD_BUS_MODE_3_WIRE_I_SPI = 0,     /*!< 3-wire I SPI Interface (default) */
    QOSA_LCD_BUS_MODE_3_WIRE_II_SPI,        /*!< 3-wire II SPI Interface */
    QOSA_LCD_BUS_MODE_4_WIRE_I_SPI,         /*!< 4-wire I SPI Interface */
    QOSA_LCD_BUS_MODE_4_WIRE_II_SPI,        /*!< 4-wire II SPI Interface */
    QOSA_LCD_BUS_MODE_4_WIRE_II_2_DATA_SPI, /*!< 4-wire II 2 Data SPI Interface */
    QOSA_LCD_BUS_MODE_8080,                 /*!< 8080(mcu) Interface */
    QOSA_LCD_BUS_MODE_MIPI,                 /*!< MIPI Interface */
} qosa_lcd_bus_mode_e;

/**
 * @enum  qosa_lcd_ioctl_cmd_e
 * @brief lcd ioctl cmd definition.
 */
typedef enum
{
    QOSA_LCD_IOCTL_NONE,
    QOSA_LCD_IOCTL_GET_STD_CHANNEL, /*!< Get STD version Lcd default channel number
                                                        - This configuration can be the value of @ref qosa_lcd_channel_e */
    QOSA_LCD_IOCTL_SET_DRV_CFG,     /*!< Set Lcd driver configuration
                                                        - This configuration can be the value of @ref qosa_lcd_drv_cfg_t */
    QOSA_LCD_IOCTL_GET_DRV_CFG,     /*!< Get Lcd driver configuration
                                                        - This configuration can be the value of @ref qosa_lcd_drv_cfg_t */
    QOSA_LCD_IOCTL_SET_SLEEP,       /*!< Control LCD controler enter/exit sleep mode
                                                        - This configuration can be the value of @ref qosa_bool_t */
    QOSA_LCD_IOCTL_HARDWARE_PRECIEW_ENABLE, /*!< Enable hardware preview mode
                                                        - This configuration can be the value of @ref qosa_lcd_set_hardware_preciew_cfg_t */
} qosa_lcd_ioctl_cmd_e;

/**
 * @enum qosa_lcd_set_hardware_preciew_e
 * @brief LCD hardware preview enable/disable status enumeration.
 */
typedef enum
{
    QOSA_LCD_HARDWARE_PRECIEW_DISENABLE = 0,  /*!< Disable LCD hardware preview function */
    QOSA_LCD_HARDWARE_PRECIEW_ENABLE = 1,     /*!< Enable LCD hardware preview function */
} qosa_lcd_set_hardware_preciew_e;
/*===========================================================================
 *  Struct
===========================================================================*/

/**
 * @struct  qosa_lcd_operations_t
 * @brief lcd operation callback.
 */
typedef struct
{
    void (*init)(qosa_lcd_channel_e lcd_no);                             /*!< Function will running after LCD init  */
    void (*deInit)(qosa_lcd_channel_e lcd_no);                           /*!< Function will running before LCD deinit */
    void (*enterSleep)(qosa_lcd_channel_e lcd_no, qosa_bool_t is_sleep); /*!< Function will running before try display LCD on/off */
    void (*setDisplayWindow)(
        qosa_lcd_channel_e lcd_no,
        uint16_t           left,
        uint16_t           top,
        uint16_t           right,
        uint16_t           bottom
    );                                                  /*!< Function will running before try write package to LCD */
    qosa_uint32_t (*readId)(qosa_lcd_channel_e lcd_no); /*!< Function will running when wants to read LCD ID */
} qosa_lcd_operations_t;

/**
 * @struct  qosa_lcd_reset_func_t
 * @brief lcd's Reset function configuration.
 */
typedef struct
{
    qosa_gpio_num_e reset_gpio;   /*!<  GPIO pin number for the Reset pin */

    qosa_uint32_t low_level_time; /*!<  The time for the Reset pin to remain at a low level */
} qosa_lcd_reset_func_t;

/**
 * @struct  qosa_lcd_spi_func_t
 * @brief lcd's Reset function configuration.
 */
typedef struct
{
    qosa_spi_frame_format_e  frame_format;  /*!< SPI clock polarity and phase configuration */
    qosa_spi_bit_order_e     bit_order;     /*!< SPI data transmission bit order */
    qosa_spi_transmit_mode_e transmit_mode; /*!< SPI transmission mode */
    qosa_spi_nss_mode_e      nss_mode;      /*!< SPI chip select signal control method */

    qosa_gpio_num_e nss_gpio;               /*!< Standard SPI CS pin (If nss_mode is hardware control, this parameter is invalid) */
    qosa_gpio_num_e ds_gpio;                /*!< When used standard SPI, need control the LCD DS pin by software */
} qosa_lcd_spi_func_t;

/**
 * @struct  qosa_lcd_drv_cfg_t
 * @brief lcd driver option.
 */
typedef struct
{
    qosa_lcd_channel_e lcd_channel;                 /*!< Lcd channel config */

    qosa_uint8_t *lcd_name;                         /*!<  Lcd name */
    qosa_uint32_t lcd_device_id;                    /*!<  Lcd device ID */

    qosa_uint32_t lcd_width;                        /*!<  Lcd width config */
    qosa_uint32_t lcd_height;                       /*!<  Lcd height config */

    qosa_lcd_bus_mode_e          bus_mode;          /*!<  Lcd operation bus mode */
    qosa_lcd_input_format_opt_e  lcd_input_format;  /*!<  Lcd input data format */
    qosa_lcd_output_format_opt_e lcd_output_format; /*!<  Lcd output data format */

    qosa_lcd_operations_t *operation;               /*!< Lcd operation callback */
    qosa_uint32_t          lcd_write_frequence;     /*!<  Lcd controler write rate */
    qosa_uint32_t          lcd_read_frequence;      /*!<  Lcd controler read rate */
    qosa_lcd_reset_func_t  lcd_reset_func;          /*!<  Lcd reset function */
    qosa_lcd_spi_func_t    lcd_spi_func;            /*!<  Lcd standard SPI function */
} qosa_lcd_drv_cfg_t;

/**
 * @struct  qosa_lcd_set_hardware_preciew_cfg_t
 * @brief lcd hardware preview configuration.
 */
typedef struct
{
    qosa_uint32_t camera_width;                        /*!<  camera width config */
    qosa_uint32_t camera_height;                       /*!<  camera height config */
    qosa_camera_output_format_opt_e camera_format;      /*!<  camera output format,The hardware preview LCD can only receive YUV422 data. */
    qosa_lcd_set_hardware_preciew_e hardware_preciew_set;/*!<  Enable hardware preview */
} qosa_lcd_set_hardware_preciew_cfg_t;

/*===========================================================================
 * Function
===========================================================================*/

/**
 * @brief Initialize LCD controller
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_init(qosa_lcd_channel_e lcd_no);

/**
 * @brief Deinitialize LCD controller
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_deinit(qosa_lcd_channel_e lcd_no);

/**
 * @brief Clear the LCD screen
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @param[in] qosa_uint16_t color
 *          - Colors that need to be filled
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_clear_screen(qosa_lcd_channel_e lcd_no, qosa_uint16_t color);

/**
 * @brief Turn on the LCD display
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_display_on(qosa_lcd_channel_e lcd_no);

/**
 * @brief Turn off the LCD display
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_display_off(qosa_lcd_channel_e lcd_no);

/**
 * @brief Send configuration commands to the LCD
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @param[in] qosa_uint8_t cmd
 *          - Commands that need to be sent
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_write_cmd(qosa_lcd_channel_e lcd_no, qosa_uint8_t cmd);

/**
 * @brief Send data to the LCD
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @param[in] qosa_uint8_t cmd_data
 *          - Cmd data that need to be sent
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_write_cmd_data(qosa_lcd_channel_e lcd_no, qosa_uint8_t cmd_data);

/**
 * @brief Flush all cached cmd and cmd data
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_flush_cmd(qosa_lcd_channel_e lcd_no);

/**
 * @brief LCD controler read lcd configuration
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD controler channel selection
 *
 * @param[in] qosa_uint8_t cmd
 *          - LCD configuration command that need to read
 *
 * @param[out] qosa_uint8_t *data
 *          - Address of data to be read
 *
 * @param[in] qosa_uint32_t length
 *           - Length of data to be read
 *
 * @param[in] qosa_uint32_t dummyCycleLen
 *           - The dummy cycles which the driver waits after cmd transmission
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_read_cmd_data(qosa_lcd_channel_e lcd_no, qosa_uint8_t cmd, qosa_uint8_t *data, qosa_uint32_t length, qosa_uint32_t dummyCycleLen);

/**
 * @brief Send picture data to the designated location of LCD
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @param[in] qosa_uint8_t* data
 *          - Data address that need to be sent
 *
 * @param[in] qosa_uint16_t start_x
 *          - Starting X-coordinate
 *
 * @param[in] qosa_uint16_t start_y
 *          - Starting Y-coordinate
 *
 * @param[in] qosa_uint16_t end_x
 *          - Ending Y-coordinate
 *
 * @param[in] qosa_uint16_t end_y
 *          - Ending Y-coordinate
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e
qosa_lcd_write(qosa_lcd_channel_e lcd_no, qosa_uint8_t *buffer, qosa_uint16_t start_x, qosa_uint16_t start_y, qosa_uint16_t end_x, qosa_uint16_t end_y);

/**
 * @brief Register LCD event handler
 *
 * @param[in] qosa_lcd_channel_e lcd_no
 *          - LCD channel number selection
 *
 * @param[in] qosa_lcd_ioctl_cmd_e cmd
 *          - Corresponding ioctl command
 *
 * @param[in/out] void * arg
 *           - Corresponding ioctl command parameters
 *
 * @return qosa_lcd_error_e
 *          - QOSA_LCD_SUCCESS: Success
 *          - other: Error code
 */
qosa_lcd_error_e qosa_lcd_ioctl(qosa_lcd_channel_e lcd_no, qosa_lcd_ioctl_cmd_e cmd, void *arg);

#endif /* __QOSA_LCD_H__ */
