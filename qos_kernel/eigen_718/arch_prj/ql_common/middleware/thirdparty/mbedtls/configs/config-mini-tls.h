/**
 * \file config-mini-tls.h
 *
 * \brief For tailoring, only the most basic encryption functions are retained
 */


#ifndef MBEDTLS_CONFIG_H
#define MBEDTLS_CONFIG_H

/* System support */
#define MBEDTLS_HAVE_ASM
#define MBEDTLS_HAVE_TIME

#include "qosa_platform_cfg.h"
#define MBEDTLS_OS_FREERTOS

#if defined(MBEDTLS_OS_FREERTOS)
#define MBEDTLS_PLATFORM_CALLOC_MACRO calloc //mbedtls_calloc //
#define MBEDTLS_PLATFORM_FREE_MACRO	free //mbedtls_free //
#endif

#define MBEDTLS_PLATFORM_C
#define MBEDTLS_PLATFORM_MEMORY
#define MBEDTLS_MD5_C

#define MBEDTLS_PKCS1_V15                   // 用于 RSA 加密和签名

// 核心加密算法
#define MBEDTLS_AES_C                       // AES 对称加密算法
#define MBEDTLS_CIPHER_C                    // ͨ通用密码算法框架（支持CBC等模式）
#define MBEDTLS_CIPHER_MODE_CBC             // CBC 分组密码模式,加密模式
#define MBEDTLS_SHA256_C                    // SHA-256 哈希算法（用于HMAC等）
#define MBEDTLS_SHA512_C                    // SHA-512 哈希算法

// 公钥加密算法
#define MBEDTLS_RSA_C                       // RSA 公钥算法
#define MBEDTLS_PK_C                        // 公钥算法框架,RSA 也会用到
#define MBEDTLS_PK_PARSE_C                  // 公钥解析（证书用）

// 裁剪掉 ECDSA
//#define MBEDTLS_ECDSA_C                     // ECDSA 椭圆曲线数字签名,10K大小
//#define MBEDTLS_ECP_C                       // 椭圆曲线基础运算
//#define MBEDTLS_ECP_DP_SECP256R1_ENABLED    // P-256 椭圆曲线
//#define MBEDTLS_ECP_DP_SECP384R1_ENABLED    // P-384 椭圆曲线


#define MBEDTLS_MD_C                        // CTR-DRBG 随机数生成器

#define MBEDTLS_ASN1_PARSE_C                // ASN.1 解析
#define MBEDTLS_ASN1_WRITE_C                // ASN.1 生成

#define MBEDTLS_BIGNUM_C                    // 大数运算

#define MBEDTLS_OID_C                       // OID 解析,MBEDTLS_RSA_C used

#define MBEDTLS_AES_ROM_TABLES              // AES 算法的查表法优化

//#define MBEDTLS_BASE64_C                  // Base64 编解码,1K大小

//#define MBEDTLS_PEM_PARSE_C               // PEM 解析
//#define MBEDTLS_X509_USE_C                // X.509 证书使用
//#define MBEDTLS_X509_CRT_PARSE_C          // X.509 证书解析

#include "mbedtls/check_config.h"
#endif /* MBEDTLS_CONFIG_H */
