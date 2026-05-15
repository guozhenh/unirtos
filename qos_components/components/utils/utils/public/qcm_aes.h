/**
 *@file qcm_aes.h
 * @author lawrence.liu (lawrence.liu@quectel.com)
 *  @brief
 * @version 0.1
 * @date 2023-11-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _QOSA_AES_H
#define _QOSA_AES_H

#include "qosa_sys.h"
#include "qosa_def.h"

/*****************************************************************
* Function: qcm_aes_string_encryption
*
* Description:
* AES encryption algorithm
* This function is to hash and encrypt the user input data to get an encrypted result.
*
* Parameters:
* key            [in]  secret key
* key_length     [in]  secret key length
* inputdata      [in]  use inut data
* outputdata     [in]  hash result

* Return:
* void
*
*****************************************************************/
void qcm_aes_string_encryption(qosa_uint8_t key[16], int key_length, qosa_uint8_t inputdata[16], qosa_uint8_t outputdata[16]);

/*****************************************************************
* Function: qcm_aes_string_decryption
*
* Description:
* AES decryption algorithm
* This function decrypts the hash encrypted data to get the original data.
*
* Parameters:
* key            [in]  secret key
* key_length     [in]  secret key length
* inputdata      [in]  use inut data
* outputdata     [in]  hash result

* Return:
* void
*
*****************************************************************/
void qcm_aes_string_decryption(qosa_uint8_t key[16], int key_length, qosa_uint8_t inputdata[16], qosa_uint8_t outputdata[16]);

/**
 * @brief Encrypts input data using the AES algorithm in CBC (Cipher Block Chaining) mode,
 *        and automatically applies PKCS#7 padding.
 *
 * @param key         Pointer to the AES encryption key.
 * @param keybits     The length of the key in bits. Valid values are typically 128, 192, or 256.
 * @param iv          Pointer to the 16-byte Initialization Vector (IV).
 * @param inputdata   Pointer to the raw input data to be encrypted.
 * @param outputdata  Pointer to the output buffer.
 * @param inlen       Length of the input data.
 * @param outlen      Length of the output data.
 *
 */
void qcm_aes_string_cbc_encryption(
    qosa_uint8_t *key,
    qosa_uint32_t keybits,
    qosa_uint8_t  iv[16],
    qosa_uint8_t *inputdata,
    qosa_uint8_t *outputdata,
    int           inlen,
    int          *outlen
);

#endif
