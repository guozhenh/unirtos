/**
 *@file qcm_sha256.h
 * @author lawrence.liu (lawrence.liu@quectel.com)
 *  @brief
 * @version 0.1
 * @date 2023-11-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef _QOSA_SHA256_H_
#define _QOSA_SHA256_H_

#if defined(__cplusplus)
extern "C"
{
#endif

#include "qosa_sys.h"
#include "qosa_def.h"

#define CORE_SHA256_DIGEST_LENGTH        (32)
#define CORE_SHA256_BLOCK_LENGTH         (64)
#define CORE_SHA256_SHORT_BLOCK_LENGTH   (CORE_SHA256_BLOCK_LENGTH - 8)
#define CORE_SHA256_DIGEST_STRING_LENGTH (CORE_SHA256_DIGEST_LENGTH * 2 + 1)

    /**
 * \brief          SHA-256 context structure
 */
    typedef struct
    {
        qosa_uint32_t total[2];   /*!< number of bytes processed  */
        qosa_uint32_t state[8];   /*!< intermediate digest state  */
        qosa_uint8_t  buffer[64]; /*!< data block being processed */
        qosa_uint8_t  is224;      /*!< 0 => SHA-256, else SHA-224 */
    } qcm_core_sha256_context_t;

    /**
 * \brief          Initialize SHA-256 context
 *
 * \param ctx      SHA-256 context to be initialized
 */
    void qcm_sha256_init(qcm_core_sha256_context_t *ctx);

    /**
 * \brief          Clear SHA-256 context
 *
 * \param ctx      SHA-256 context to be cleared
 */
    void qcm_sha256_free(qcm_core_sha256_context_t *ctx);

    /**
 * \brief          SHA-256 context setup
 *
 * \param ctx      context to be initialized
 */
    void qcm_sha256_starts(qcm_core_sha256_context_t *ctx);

    /**
 * \brief          SHA-256 process buffer
 *
 * \param ctx      SHA-256 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
    void qcm_sha256_update(qcm_core_sha256_context_t *ctx, const qosa_uint8_t *input, qosa_uint32_t ilen);

    /**
 * \brief          SHA-256 final digest
 *
 * \param ctx      SHA-256 context
 * \param output   SHA-256 checksum result
 */
    void qcm_sha256_finish(qcm_core_sha256_context_t *ctx, qosa_uint8_t output[32]);

    /* Internal use */
    void qcm_sha256_process(qcm_core_sha256_context_t *ctx, const qosa_uint8_t data[64]);

    /**
 * \brief          Output = SHA-256( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SHA-256 checksum result
 */
    void qcm_core_sha256(const qosa_uint8_t *input, qosa_uint32_t ilen, qosa_uint8_t output[32]);

    void qcm_hmac_sha256(const qosa_uint8_t *msg, qosa_uint32_t msg_len, const qosa_uint8_t *key, qosa_uint32_t key_len, qosa_uint8_t output[32]);

/* unir API name change */  // [warning]
#define qcm_sha256(input, ilen, output) qcm_core_sha256(input, ilen, output);

#if defined(__cplusplus)
}
#endif

#endif
