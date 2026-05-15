/** @file         qurl_tls.h
 *  @brief        Brief Description
 *  @details      Detailed Description
 *  @author       lzm
 *  @date         2023-11-09 20:19:51
 *  @version      v1.0
 *  @copyright    Copyright By lizhuming, All Rights Reserved
 *
 **********************************************************
 *  @LOG Change Log: -----
 **********************************************************
*/

#ifndef QURL_TLS_H
#define QURL_TLS_H

#include "qurl_config.h"
#include "qurl_code.h"
#include "qurl_def.h"
#include "qurl_def_in.h"

/**
 * @brief Initialize tls_cfg and assign it the default configuration.
 * 
 * @param tls_cfg_ptr 
 */
void qurl_tls_cfg_init(qurl_tls_cfg_t *tls_cfg_ptr);

/**
 * @brief Initialize the TLS function
 * 
 * @return qurl_ecode_t 
 */
qurl_ecode_t qurl_tls_init(void);

/**
 * @brief Deinitialize the TLS function
 * 
 * @return qurl_ecode_t 
 */
qurl_ecode_t qurl_tls_deinit(void);

/**
 * @brief TLS version number
 * 
 * @param buf[out] TLS version, in string format
 * @param size[in] The available buffer size of buf
 * @return long version number occupies the size of the buffer
 */
long qurl_tls_version(char *buf, long size);

/**
 * @brief Create a new TLS service
 * 
 * @param tls_cfg_ptr[out] The TLS configuration to be provided
 * @return qurl_tls_ctrl_t* 
 */
qurl_tls_ctrl_t *qurl_tls_new(qurl_tls_cfg_t *tls_cfg_ptr);

/**
 * @brief Release a TLS service
 * 
 * @param tls_ctrl_ptr[in] The TLS handle to be released
 */
void qurl_tls_free(qurl_tls_ctrl_t *tls_ctrl_ptr);

/**
 * @brief Configure TLS information
 * 
 * @param tls_ctrl_ptr[in] TLS handle
 * @param opt_tag[in] option
 * @param opt_val[in] value
 */
qurl_ecode_t qurl_tls_set_option(qurl_tls_ctrl_t *tls_ctrl_ptr, int opt_tag, void *opt_val);

/**
 * @brief The length of data in the receive buffer, which is the data that has been decrypted but not yet read.
 * 
 * @param tls_ctrl_ptr[in] The TLS handle to be released
 * @return qurl_bit_t 
 */
qurl_bit_t qurl_tls_data_pending(qurl_tls_ctrl_t *tls_ctrl_ptr);

/**
 * @brief Blocking TLS connection
 * 
 * @param tls_ctrl_ptr[in] The TLS handle to be released
 * @return qurl_ecode_t 
 */
qurl_ecode_t qurl_tls_connect(qurl_tls_ctrl_t *tls_ctrl_ptr);

/**
 * @brief Non-blocking TLS connection establishment
 * Can be re-entered repeatedly until the TLS connection is completed or an exception occurs
 * 
 * @param tls_ctrl_ptr[in] The TLS handle to be released
 * @param done_ptr[out] Whether the connection is completed
 * @return qurl_ecode_t 
 */
qurl_ecode_t qurl_tls_connect_nonblocking(qurl_tls_ctrl_t *tls_ctrl_ptr, qurl_bit_t *done_ptr);

/**
 * @brief Disconnect TLS connection
 * 
 * @param tls_ctrl_ptr[in] The TLS handle to be released
 * @return qurl_ecode_t 
 */
qurl_ecode_t qurl_tls_close(qurl_tls_ctrl_t *tls_ctrl_ptr);

/**
 * @brief Read data
 * 
 * @param tls_ctrl_ptr[in] The TLS handle to be released
 * @param buf[out] Caller buffer for receiving data
 * @param buf_size[in] The available space size of buf
 * @param ecode_ptr[out] error code
 * @return long buf length of received data
 */
long qurl_tls_read(qurl_tls_ctrl_t *tls_ctrl_ptr, unsigned char *buf, long buf_size, qurl_ecode_t *ecode_ptr);

/**
 * @brief Send data
 * 
 * @param tls_ctrl_ptr[in] The TLS handle to be released
 * @param buf[in] Caller's transmit buffer
 * @param buf_size[in] The length to be sent
 * @param ecode_ptr[out] error code
 * @return long The actual length successfully sent
 */
long qurl_tls_write(qurl_tls_ctrl_t *tls_ctrl_ptr, unsigned char *buf, long buf_size, qurl_ecode_t *ecode_ptr);

#endif /* Head define end*/
