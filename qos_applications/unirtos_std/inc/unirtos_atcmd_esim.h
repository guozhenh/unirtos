#ifndef __UNIRTOS_ATCMD_ESIM_H__
#define __UNIRTOS_ATCMD_ESIM_H__

#include "qosa_def.h"
#include "qosa_sys.h"
#include "esim_interface.h"
#include "qosa_at_cmd.h"

#define UNIRTOS_ESIM_RESPONSE_SIZE  4096
#define ESIM_MAIN_THREAD_STACK_SIZE 10240
#define ESIM_MAIN_THREAD_PRIORITY   QOSA_PRIORITY_NORMAL
#define ESIM_MAIN_THREAD_NAME_RSP   "ESIM_RSP_THREAD"
#define ESIM_MAIN_THREAD_NAME_AT    "ESIM_AT_PROCESS_THREAD"
#define ESIM_URC_MAX_LEN            4096
#define ESIM_LPA_DIRECT_RSP_MODE    0
#define ESIM_LPA_INDIRECT_RSP_MODE  1
#define ESIM_MSG_WAIT_1000_MS       1000

typedef enum
{
    LPACMD_INVALID = 0,
    LPACMD_GET_PROFILES,
    LPACMD_ENABLE_PROFILE,
    LPACMD_DISABLE_PROFILE,
    LPACMD_DELETE_PROFILE,
    LPACMD_GET_EID,
    LPACMD_SET_PROFILE_NICKNAME,
    LPACMD_DOWNLOAD_PROFILE,
    LPACMD_TRANS_PROFILE,
    LPACMD_SET_APN,
    LPACMD_GET_VERSION,
    LPACMD_GET_COMPILE_TIME,

    LPACMD_NOTIFICATIONS_LIST,
    LPACMD_NOTIFICATION_DELETE,
    LPACMD_NOTIFICATION_REPORT,
    LPACMD_NOTIFICATION_GET,

    IPACMD_IPA_POLL_SET,
    IPACMD_EIM_LIST_GET,
    IPACMD_EIM_LIST_ADD,
    IPACMD_EIM_INFO_SELECT,
    IPACMD_IPA_TIMER_GET,
    IPACMD_IPA_FALLBACK,
    IPACMD_IPA_FALLBACK_RETURN,
    IPACMD_EUICC_MEMORY_RESET,
    IPACMD_IPA_POLL_DELAY,
} ql_esim_arg1_e;

#define QL_ESIM_LISTEN_NULL 0x00
#define QL_ESIM_LISTEN_RESP 0x01

typedef enum
{
    ESIM_NOTIFICATION_TYPE_ALL = 0,
    ESIM_NOTIFICATION_TYPE_ENABLE,
    ESIM_NOTIFICATION_TYPE_DISABLE,
    ESIM_NOTIFICATION_TYPE_DELETE,
    ESIM_NOTIFICATION_TYPE_INSTALL
} ql_esim_ntf_type_e;

typedef struct
{
    int  mode;
    char ac_code[ESIM_AC_CC_CODE_MAX_LEN];
    char cc_code[ESIM_AC_CC_CODE_MAX_LEN];
} lpa_rsp_arg_t;

typedef struct
{
    qosa_bool_t   state;
    qosa_uint8_t  seq;
    qosa_uint8_t *data;
    qosa_uint32_t data_len;
} lpa_trans_arg_t;

typedef union
{
    lpa_rsp_arg_t   download;
    lpa_trans_arg_t trans;
} esim_at_argv_u_t;

typedef struct
{
    qosa_at_dev_type_e dev_port;
    esim_at_argv_u_t   argv;
} qesim_async_at_arg_t;

void unir_exec_esim_cmd(qosa_at_cmd_t *cmd);

void ql_esim_service_start(void);

#endif  // __UNIRTOS_ATCMD_ESIM_H__
