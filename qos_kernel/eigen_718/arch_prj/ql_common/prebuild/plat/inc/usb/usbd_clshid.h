#ifndef __USBD_CLSHID_H__
#define __USBD_CLSHID_H__

#include "stdint.h"
#include "string.h"

#include "usbd_desc_def.h"



//hid class descriptor
struct tag_usb_hid_cls_desc {
    uint8_t	bLength;
    uint8_t	bDescriptorType;
    uint8_t	bcdHIDLow;
    uint8_t	bcdHIDHigh;
    
    uint8_t	bCountryCode;
    uint8_t	bNumDescriptors; 
    uint8_t      bSubDescType;
    uint8_t wSubDescLen_Low; 
    uint8_t wSubDescLen_High; 
    
} __attribute__ ((packed));    


typedef struct tag_usb_hid_cls_desc usb_hid_cls_desc_st;

struct tag_usbd_hid_desc 
{
    usb_config_desc_st                 cfg_desc;
    
    usb_intf_assoc_desc_st           intf_asso_desc;
    
    usb_interface_descriptor_st     intf_ctrl_desc;
    usb_hid_cls_desc_st         hid_cls_desc;

    usb_endpoint_descriptor_st     hid_intr_iep_desc;
    usb_endpoint_descriptor_st     dc_intr_oep_desc;
}__attribute__ ((packed));

typedef struct tag_usbd_hid_desc usbd_hid_desc_st;


struct tag_usbd_hid_ccinst_desc_inhrnt 
{
    usb_interface_descriptor_st     intf_ctrl_desc;
    usb_hid_cls_desc_st         hid_cls_desc;

    usb_endpoint_descriptor_st     hid_itriep_desc;
    usb_endpoint_descriptor_st     hid_itroep_desc;

    
}__attribute__ ((packed));



#endif

