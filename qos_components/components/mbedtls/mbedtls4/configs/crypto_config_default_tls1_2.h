/**
 * \file crypto_config_default_tls1_2.h
 *
 * \brief \brief Minimal crypto configuration for
 * TLS NSA Suite B Profile (RFC 6460).
 */
/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

/**
 * Minimal crypto configuration for TLS NSA Suite B Profile (RFC 6460)
 *
 * Distinguishing features:
 * - no RSA or classic DH, fully based on ECC
 * - optimized for low RAM usage
 *
 * Possible improvements:
 * - if 128-bit security is enough, disable secp384r1 and SHA-512
 *
 * To be used in conjunction with configs/config-suite-b.h. */

#ifndef PSA_CRYPTO_CONFIG_H
#define PSA_CRYPTO_CONFIG_H
#include "qosa_def.h"

#define PSA_WANT_ALG_ECDH                        1
#define PSA_WANT_ALG_ECDSA                       1
#define PSA_WANT_ALG_GCM                         1
#define PSA_WANT_ALG_SHA_256                     1
#define PSA_WANT_ALG_SHA_384                     1
#define PSA_WANT_ALG_SHA_512                     1
#define PSA_WANT_ECC_SECP_R1_256                 1
#define PSA_WANT_ECC_SECP_R1_384                 1
#define PSA_WANT_ALG_TLS12_PRF                   1

#define PSA_WANT_KEY_TYPE_AES                    1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC     1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT    1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE  1

#define MBEDTLS_PSA_CRYPTO_C

/* System support */
#define MBEDTLS_HAVE_ASM
#define MBEDTLS_HAVE_TIME

#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_ASN1_WRITE_C
#define MBEDTLS_CTR_DRBG_C
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C
// #define MBEDTLS_PSA_BUILTIN_GET_ENTROPY
#define MBEDTLS_PSA_DRIVER_GET_ENTROPY

/* For test certificates */
#define MBEDTLS_BASE64_C
#define MBEDTLS_PEM_PARSE_C

/* Save RAM at the expense of ROM */
#define MBEDTLS_AES_ROM_TABLES

/* Save RAM by adjusting to our exact needs */
#define MBEDTLS_MPI_MAX_SIZE    48 // 384-bit EC curve = 48 bytes

/* Save RAM at the expense of speed, see ecp.h */
#define MBEDTLS_ECP_WINDOW_SIZE        2
#define MBEDTLS_ECP_FIXED_POINT_OPTIM  0

/* Significant speed benefit at the expense of some ROM */
#define MBEDTLS_ECP_NIST_OPTIM


// #define MBEDTLS_AES_C
// #define MBEDTLS_CIPHER_MODE_CBC
// #define MBEDTLS_PKCS1_V15
#define MBEDTLS_BIGNUM_C
// #define MBEDTLS_CIPHER_C
// #define MBEDTLS_DES_C
#define MBEDTLS_MD_C
// #define MBEDTLS_MD5_C
// #define MBEDTLS_RSA_C
// #define MBEDTLS_SHA256_C
#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_CALLOC_MACRO   qosa_calloc
#define MBEDTLS_PLATFORM_FREE_MACRO	    qosa_free
#define mbedtls_malloc                  qosa_malloc

#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_HAVE_TIME_DATE
#define MBEDTLS_TIMING_C
#define MBEDTLS_SSL_SERVER_NAME_INDICATION
// #define MBEDTLS_SHA512_C
#define MBEDTLS_ECP_C
#define MBEDTLS_ECDSA_C
#define MBEDTLS_ECP_DP_SECP384R1_ENABLED

#define MBEDTLS_SSL_RENEGOTIATION
#define MBEDTLS_ARC4_C
#define MBEDTLS_SSL_SESSION_TICKETS
#define MBEDTLS_CAN_ECDH
#define PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_BASIC
#define PSA_WANT_ALG_RSA_PKCS1V15_CRYPT
#define PSA_WANT_ALG_RSA_PKCS1V15_SIGN
#define MBEDTLS_ECDH_C
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED

// #define MBEDTLS_GCM_C
#define MBEDTLS_ECP_DP_SECP256R1_ENABLED
#define MBEDTLS_ECP_DP_SECP192R1_ENABLED

#endif /* PSA_CRYPTO_CONFIG_H */
