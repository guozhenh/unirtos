#ifndef _UNIRTOS_ATCMD_LICENSE_H_
#define _UNIRTOS_ATCMD_LICENSE_H_

#include "qosa_def.h"
#include "qosa_at_cmd.h"

// Constants
#define TOTAL_DATA_LENGTH        2048
#define CPU_ID_LENGTH            15
#define DIGEST_LENGTH            128
#define DEV_LENGTH               48
#define RETURN_STRING_LENGTH     256
#define APPID_MAX_LENGTH         2000

/**
 * @brief Execute the AT+QSECLICINFO command
 *
 * @param[in] cmd Pointer to the AT command structure
 */
void qosa_exec_qlicenseinfo_cmd(qosa_at_cmd_t *cmd);

/**
 * @brief Execute the AT+QSECLICEXE command
 *
 * @param[in] cmd Pointer to the AT command structure
 */
void qosa_exec_qlicense_cmd(qosa_at_cmd_t *cmd);

#endif /* _UNIRTOS_ATCMD_LICENSE_H_ */
