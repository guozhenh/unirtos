/*================================================================
  Copyright (c) 2021, Quectel Wireless Solutions Co., Ltd. All rights reserved.
  Quectel Wireless Solutions Proprietary and Confidential.
=================================================================*/

#ifndef __QL_THREAD_MSG_H_
#define __QL_THREAD_MSG_H_


#include "cmsis_os2.h"
#include "cms_def.h"
#include "osasys.h"
#include "ql_api_common.h"
#define QUEC_MSG_LEN_NONE   0

 #ifdef QL_SUPPORT_USIM_HOTPLUG
typedef struct {
  unsigned char sim_det_enable;    //0--disable,1--enable
  unsigned char sim_insert_level;   //0--low level,1--high level
  unsigned char sim_report_enable;  // sim report enable /disable
}quec_usim_det_stat_s;

extern quec_usim_det_stat_s   g_usim_det_stat;
#endif

typedef enum
{
    QUEC_NONE_MSG=-1,
#ifdef QL_SUPPORT_USIM_HOTPLUG
    GPIO_MANAGE_HOTPLUG_MSG,
#endif
#ifdef QL_USB_SUPPORT
    QUEC_USB_SEND_REMOTE_WKUP,
#endif

	MSG_IND_MAX
}quec_msg_control_type_e;


typedef struct
{
    quec_msg_control_type_e type;
	uint8 data_len;
	uint8 *pdata;  				//msg_handle task process actual data
	uint8 extra_data;
}quec_msg_process_s;

extern int ql_send_msg_to_handle_task_isr(quec_msg_control_type_e msg_type, uint8_t data);
extern int ql_send_msg_to_handle_task(quec_msg_control_type_e msg_type,char *pdata,uint32 len);
extern int ql_msg_handle_task_init(void);
 int ql_gpio_manage_msg_send(UINT8 msgId, UINT8 level);



#endif



