/*****************************************************************/ /**
* @file qosa_can_eigen.h
* @brief
* @author Lambert.Zhao@quectel.com
* @date 2025-10-21
*
* @copyright Copyright (c) 2023 Quectel Wireless Solution, Co., Ltd.
* All Rights Reserved. Quectel Wireless Solution Proprietary and Confidential.
*
* @par EDIT HISTORY FOR MODULE
* <table>
* <tr><th>Date <th>Version <th>Author <th>Description"
* <tr><td>2025-10-21 <td>1.0 <td>Lambert.Zhao <td> Init
* </table>
**********************************************************************/  
#ifndef __QOSA_CAN_EIGEN_H__
#define __QOSA_CAN_EIGEN_H__

#include "qosa_system_config.h"
#ifdef CONFIG_QOSA_EIGEN718_PLATFORM_FUNC
#include "qosa_def.h"
#include "qosa_sys.h"

/*===========================================================================
 * Macro Definition
 ===========================================================================*/
//#define QOSA_I2C_ERRCODE_BASE (QOSA_COMPONENT_API_I2C << 16)
/*========================================================================
*  Enumeration Definition
*========================================================================*/
typedef enum 
{
    QOSA_CAN_DRIVER_OK = 0, 		  
    QOSA_CAN_DRIVER_ERROR = -1,
    QOSA_CAN_DRIVER_ERROR_BUSY = -2, 
    QOSA_CAN_DRIVER_ERROR_TIMEOUT = -3, 
    QOSA_CAN_DRIVER_ERROR_UNSUPPORTED = -4, 
    QOSA_CAN_DRIVER_ERROR_PARAMETER = -5, 
    QOSA_CAN_DRIVER_ERROR_SPECIFIC = -6, 
} qosa_can_error_e;

typedef enum 
{
    QOSA_CAN_DEV_NUM0, 		  
    QOSA_CAN_DEV_NUM1,
    QOSA_CAN_DEV_NUM2, 
    QOSA_CAN_DEV_MAX
} qosa_can_dev_num_e;

typedef enum
{
	  QOSA_CAN_POWER_OFF,                        ///< Power off: no operation possible
	  QOSA_CAN_POWER_LOW,                        ///< Low Power mode: retain state, detect and signal wake-up events
	  QOSA_CAN_POWER_FULL                        ///< Power on: full operation at maximum performance
} qosa_can_power_state_e;

typedef enum
{
    QOSA_CAN_BITRATE_NOMINAL,              ///< Select nominal (flexible data-rate arbitration) bitrate
    QOSA_CAN_BITRATE_FD_DATA               ///< Select flexible data-rate data bitrate
} qosa_can_bitrate_select_e;

/****** CAN Mode codes *****/
typedef enum 
{
    QOSA_CAN_MODE_INITIALIZATION,          ///< Initialization mode. Used to setup communication parameters for the reception objects and global filtering, while peripheral is not active on the bus.
    QOSA_CAN_MODE_NORMAL,                  ///< Normal operation mode. Used when peripheral is in active mode to receive, transmit, and acknowledge messages on the bus. Depending on the current unit state, it can generate error or overload messages.
    QOSA_CAN_MODE_RESTRICTED,              ///< Restricted operation mode. Used for monitoring the bus communication non-instrusively without transmitting.
    QOSA_CAN_MODE_MONITOR,                 ///< Bus monitoring mode
    QOSA_CAN_MODE_LOOPBACK_INTERNAL,       ///< Loopback internal mode. No transmission visible on CAN bus.
    QOSA_CAN_MODE_LOOPBACK_EXTERNAL        ///< Loopback external mode. Transmission is visible on CAN bus.
} qosa_can_mode_e;

/****** CAN Filter Operation codes *****/
typedef enum
{
    QOSA_CAN_FILTER_ID_EXACT_ADD,          ///< Add    exact id filter
    QOSA_CAN_FILTER_ID_EXACT_REMOVE,       ///< Remove exact id filter
    QOSA_CAN_FILTER_ID_RANGE_ADD,          ///< Add    range id filter
    QOSA_CAN_FILTER_ID_RANGE_REMOVE,       ///< Remove range id filter
    QOSA_CAN_FILTER_ID_MASKABLE_ADD,       ///< Add    maskable id filter
    QOSA_CAN_FILTER_ID_MASKABLE_REMOVE,    ///< Remove maskable id filter
    QOSA_CAN_FILTER_SINGLE_MODE_ADD,       ///< Add    vendor specific filter
    QOSA_CAN_FILTER_SINGLE_MODE_REMOVE,    ///< Remove vendor specific filter
    QOSA_CAN_FILTER_DUAL_MODE_ADD,         ///< Add    vendor specific filter
    QOSA_CAN_FILTER_DUAL_MODE_REMOVE       ///< Remove vendor specific filter
} qosa_can_filter_operation_e;

/****** CAN Object Configuration codes *****/
typedef enum
{
    QOSA_CAN_OBJ_INACTIVE,                 ///< CAN object inactive
    QOSA_CAN_OBJ_TX,                       ///< CAN transmit object
    QOSA_CAN_OBJ_RX,                       ///< CAN receive object
    QOSA_CAN_OBJ_RX_RTR_TX_DATA,           ///< CAN object that on RTR reception automatically transmits Data Frame
    QOSA_CAN_OBJ_TX_RTR_RX_DATA            ///< CAN object that transmits RTR and automatically receives Data Frame
} qosa_can_obj_config_e;

/****** CAN control codes *****/
typedef enum 
{
    QOSA_CAN_CMD_GET_CAP,           
    QOSA_CAN_CMD_SET_POWER,                   
    QOSA_CAN_CMD_GET_CLOCK,   
    QOSA_CAN_CMD_SET_BITRATE,   
    QOSA_CAN_CMD_SET_MODE,   
    QOSA_CAN_CMD_GET_OBJ_CAP,   
    QOSA_CAN_CMD_SET_OBJ_FILTER,   
    QOSA_CAN_CMD_SET_OBJ_CONFIG,   
    QOSA_CAN_CMD_GET_STATUS, 
    QOSA_CAN_CMD_GET_RX_MESS_COUNT,
    QOSA_CAN_CMD_CONTROL,
    QOSA_CAN_CMD_MAX  
} qosa_can_cmd_control_e;

/*========================================================================
*  Structure Definition
*========================================================================*/
typedef struct
{
    qosa_can_bitrate_select_e select;
    qosa_uint32_t bitrate;
    qosa_uint32_t bit_segments;
} qosa_can_bitrate_t;

typedef struct
{
    qosa_uint32_t obj_idx;
    qosa_can_filter_operation_e operation;
    qosa_uint32_t id;
    qosa_uint32_t arg;
} qosa_can_obj_filter_t;

typedef struct
{
    qosa_uint32_t obj_idx;
    qosa_can_obj_config_e configure;
} qosa_can_obj_config_t;

typedef struct
{
	  qosa_uint32_t control;
	  qosa_uint32_t arg;
} qosa_can_control_t;

typedef struct
{
	  qosa_uint32_t unit_state       : 4;        ///< Unit bus state
	  qosa_uint32_t last_error_code  : 4;        ///< Last error code
	  qosa_uint32_t tx_error_count   : 8;        ///< Transmitter error count
	  qosa_uint32_t rx_error_count   : 8;        ///< Receiver error count
	  qosa_uint32_t reserved         : 8;
} qosa_can_status_t;

typedef struct
{
	  qosa_uint32_t num_objects            : 8;  ///< Number of \ref can_objects available
	  qosa_uint32_t reentrant_operation    : 1;  ///< Support for reentrant calls to \ref ARM_CAN_MessageSend, \ref ARM_CAN_MessageRead, \ref ARM_CAN_ObjectConfigure and abort message sending used by \ref ARM_CAN_Control
	  qosa_uint32_t fd_mode                : 1;  ///< Support for CAN with flexible data-rate mode (CAN_FD) (set by \ref ARM_CAN_Control)
	  qosa_uint32_t restricted_mode        : 1;  ///< Support for restricted operation mode (set by \ref ARM_CAN_SetMode)
	  qosa_uint32_t monitor_mode           : 1;  ///< Support for bus monitoring mode (set by \ref ARM_CAN_SetMode)
	  qosa_uint32_t internal_loopback      : 1;  ///< Support for internal loopback mode (set by \ref ARM_CAN_SetMode)
	  qosa_uint32_t external_loopback      : 1;  ///< Support for external loopback mode (set by \ref ARM_CAN_SetMode)
	  qosa_uint32_t reserved               : 18; ///< Reserved (must be zero)
} qosa_can_capabilities_t;

typedef struct
{
	  qosa_uint32_t tx               : 1;        ///< Object supports transmission
	  qosa_uint32_t rx               : 1;        ///< Object supports reception
	  qosa_uint32_t rx_rtr_tx_data   : 1;        ///< Object supports RTR reception and automatic Data Frame transmission
	  qosa_uint32_t tx_rtr_rx_data   : 1;        ///< Object supports RTR transmission and automatic Data Frame reception
	  qosa_uint32_t multiple_filters : 1;        ///< Object allows assignment of multiple filters to it
	  qosa_uint32_t exact_filtering  : 1;        ///< Object supports exact identifier filtering
	  qosa_uint32_t range_filtering  : 1;        ///< Object supports range identifier filtering
	  qosa_uint32_t mask_filtering   : 1;        ///< Object supports mask identifier filtering
	  qosa_uint32_t message_depth    : 8;        ///< Number of messages buffers (FIFO) for that object
	  qosa_uint32_t reserved         : 16;       ///< Reserved (must be zero)
	  qosa_uint32_t obj_idx;
} qosa_can_obj_capabilities_t;

/**
\brief CAN Message Information
*/
typedef struct
{
	  qosa_uint32_t id;                          ///< CAN identifier with frame format specifier (bit 31)
	  qosa_uint32_t rtr              : 1;        ///< Remote transmission request frame
	  qosa_uint32_t edl              : 1;        ///< Flexible data-rate format extended data length
	  qosa_uint32_t brs              : 1;        ///< Flexible data-rate format with bitrate switch
	  qosa_uint32_t esi              : 1;        ///< Flexible data-rate format error state indicator
	  qosa_uint32_t dlc              : 4;        ///< Data length code
	  qosa_uint32_t reserved         : 24;
} qosa_can_msg_info_t;

typedef struct
{
    qosa_uint32_t obj_idx;
    qosa_uint32_t count;
} qosa_can_mess_count_t;


typedef void (*qosa_can_signal_unit_event_t    )   (qosa_uint32_t event);                   ///< Pointer to \ref ARM_CAN_SignalUnitEvent   : Signal CAN Unit Event.
typedef void (*qosa_can_signal_object_event_t) (qosa_uint32_t obj_idx, qosa_uint32_t event); ///< Pointer to \ref ARM_CAN_SignalObjectEvent : Signal CAN Object Event.

typedef struct
{
	  qosa_can_dev_num_e dev;
	  qosa_can_obj_config_t can_obj_config;
	  qosa_can_capabilities_t can_cap;
	  qosa_can_obj_capabilities_t can_obj_cap;
	  qosa_can_bitrate_t bitrate;
	  qosa_can_obj_filter_t can_obj_filter;
	  qosa_can_msg_info_t msg_info;
	  qosa_can_mess_count_t mess_count;
	  qosa_can_signal_unit_event_t    cb_unit_event;                                
	  qosa_can_signal_object_event_t  cb_object_event;
} qosa_can_device_ctrl_t;

/*========================================================================
*  Func Definition
*========================================================================*/
/**
* @brief Get CAN driver support features
*
* @param dev[in] CAN bus number
* @param temp[out] CAN driver supported functions
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
qosa_int32_t qosa_can_get_capabilities(qosa_can_dev_num_e dev, qosa_can_capabilities_t* temp);

/**
* @brief CAN bus initialization
*
* @param dev[in] CAN bus number
* @param cb_unit_event[in] unit event callback function
* @param cb_object_event[in] object event callback function
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
int32_t qosa_can_init(qosa_can_dev_num_e dev, qosa_can_signal_unit_event_t cb_unit_event, qosa_can_signal_object_event_t cb_object_event);

/**
* @brief CAN bus uninit
*
* @param dev[in] CAN bus number
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
int32_t qosa_can_uninit(qosa_can_dev_num_e dev);

/**
* @brief CAN bus power control
*
* @param dev[in] CAN bus number
* @param state[in] configured power status
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
int32_t qosa_can_set_power(qosa_can_dev_num_e dev, qosa_can_power_state_e state);

/**
* @brief Get the CAN bus clock frequency
*
* @param dev[in] CAN bus number
*
* @return CAN bus clock frequency
*/
uint32_t qosa_can_get_clk(qosa_can_dev_num_e dev);

/**
* @brief Set the CAN bus bit rate
*
* @param dev[in] CAN bus number
* @param bitrate_ptr[in] configured bitrate parameter
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
int32_t qosa_can_set_bitrate(qosa_can_dev_num_e dev, qosa_can_bitrate_t* bitrate_ptr);

/**
* @brief Set the CAN bus operation mode
*
* @param dev[in] CAN bus number
* @param mode[in] configured mode
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
int32_t qosa_can_set_mode(qosa_can_dev_num_e dev, qosa_can_mode_e mode);

/**
* @brief Obtain the functions supported by the specified object
*
* @param dev[in] CAN bus number
* @param temp[out] the functions supported by the specified object
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
qosa_int32_t qosa_can_get_obj_capabilities(qosa_can_dev_num_e dev, qosa_can_obj_capabilities_t* temp);

/**
* @brief Add/Remove the message filter of the specified object
*
* @param dev[in] CAN bus number
* @param can_obj_filter[in] The message filter of the specified object
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
int32_t qosa_can_set_obj_filter(qosa_can_dev_num_e dev, qosa_can_obj_filter_t* can_obj_filter);

/**
* @brief Configure the specified object
*
* @param dev[in] CAN bus number
* @param can_obj_config[in] configure the parameters of the specified object
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
int32_t qosa_can_set_obj_config(qosa_can_dev_num_e dev, qosa_can_obj_config_t* can_obj_config);

/**
* @brief uses CAN bus to send data
*
* @param dev CAN[in] bus number
* @param obj_idx[in] object index
* @param msg_info[in] message information structure
* @param data[in] sends the data buffer
* @param size[in] the length of the data sent
*
* @return returns the actual sending length. If it fails, a negative number is returned
*/
int32_t qosa_can_write(qosa_can_dev_num_e dev, uint32_t obj_idx, qosa_can_msg_info_t *msg_info, const uint8_t *data, uint8_t size);

/**
* @brief uses CAN bus to receive data
*
* @param dev[in] CAN bus number
* @param obj_idx[in] object index
* @param msg_info[in] message information structure
* @param data[out] receiving data buffer
* @param size[in] the length of the data receive
*
* @return returns the actual received length. If it fails, it returns a negative number
*/
int32_t qosa_can_read(qosa_can_dev_num_e dev, uint32_t obj_idx, qosa_can_msg_info_t *msg_info, uint8_t *data, uint8_t size);

/**
* @brief Controls the CAN bus
*
* @param dev[in] CAN bus number
* @param can_control[in] CAN bus control parameters
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
int32_t qosa_can_control(qosa_can_dev_num_e dev, qosa_can_control_t* can_control);

/**
* @brief Get the CAN bus status
*
* @param dev[in] CAN bus number
* @param temp[out] read CAN bus status
*
* @return qosa_can_error_e
		- If the operation is successful, return QOSA_CAN_DRIVER_OK
		- Return corresponding error code if failed
*/
qosa_int32_t qosa_can_get_status(qosa_can_dev_num_e dev, qosa_can_status_t* temp);

/**
* @brief Get the count of received messages
*
* @param dev[in] CAN bus number
* @param obj_idx[in] object index
*
* @return count of received messages
*/
int32_t qosa_can_get_rx_message_count(qosa_can_dev_num_e dev, uint32_t obj_idx);

#endif /* CONFIG_QOSA_EIGEN718_PLATFORM_FUNC */
#endif /* __QOSA_CAN_EIGEN_H__ */