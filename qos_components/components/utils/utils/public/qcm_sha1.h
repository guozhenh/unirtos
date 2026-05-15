/**
 *@file qcm_hmac_sha1.h
 * @author lawrence.liu (lawrence.liu@quectel.com)
 *  @brief
 * @version 0.1
 * @date 2023-11-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _QOSA_SHA1_H
#define _QOSA_SHA1_H

#include "qosa_sys.h"
#include "qosa_def.h"

/*****************************************************************
* Function: qcm_hmac_sha1
*
* Description:
* HMACSHA1 encryption algorithm
* The hash function mixes the key with the message data and uses the hash function to hash the mixed result.
*
* Parameters:
* key            [in]  secret key
* key_length     [in]  secret key length
* data           [in]  message data
* data_length    [in]  message data length
* digest         [out] hash result

* Return:
* void
*
*****************************************************************/
void qcm_hmac_sha1(qosa_uint8_t *key, int key_length, qosa_uint8_t *data, int data_length, qosa_uint8_t *digest);

#endif
