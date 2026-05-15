#ifndef __USBD_MACRO_DEF_H__
#define __USBD_MACRO_DEF_H__
#ifndef INVALID_EP_NUM
#define INVALID_EP_NUM 0xff
#endif

#ifndef INVALID_INTF_NUM
#define INVALID_INTF_NUM 0xff
#endif

#ifndef INVALID_FUNC_NUM
//#define INVALID_FUNC_NUM 0xff
#endif


#define USBC_CTRL_EP_CONTROL                           0
#define USBC_CTRL_EP_ISOC                           1
#define USBC_CTRL_EP_BULK                           2
#define USBC_CTRL_EP_INTR                           3


#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define STANDARD_INTERFACE_DESC_SIZE             0x09
#define STANDARD_ENDPOINT_DESC_SIZE             0x07

#ifndef LOBYTE
#define LOBYTE(x)  ((uint8_t)(x & 0x00FF))
#endif

#ifndef HIBYTE
#define HIBYTE(x)  ((uint8_t)((x & 0xFF00) >>8))
#endif

#ifndef USB_RET_TYPE_ALREADY
#define USB_RET_TYPE_ALREADY
typedef enum {
  usbd_ret_no_err   = 0,
  usbd_ret_busy,
  usbd_ret_fail,
}usbd_ret_type;
#endif

#define MAX_SUPPORT_INTERFACE 16

//function resource table, max element cnt, internal used, do not change
//Function resouce max num, only some elements in the table will be maped, map cnt no more than MAX_MAP_FILLED_FUNCTION
#define MAX_SUPPORT_FUNCTION_RESOURCE 9 

//function maped table, allowed max cnt for map dev to function, internal used, do not change
#define MAX_MAP_FILLED_FUNCTION 5 

#endif

