message(STATUS "cmake config ${CMAKE_CURRENT_SOURCE_DIR}")

set(target mbedtls)
add_components_libraries($<TARGET_FILE:${target}>)
add_library(${target} STATIC)
set_target_properties(${target} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${out_unir_lib_dir})
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_SYS_DEBUG_DIR})
target_include_directories(${target} PRIVATE ${SOURCE_SYS_OS_DIR})
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(${target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/library)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/configs)

target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tf-psa-crypto/configs)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tf-psa-crypto/configs/ext)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tf-psa-crypto/core)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tf-psa-crypto/drivers/builtin/src)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tf-psa-crypto/drivers/builtin/include)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tf-psa-crypto/include)
target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/tf-psa-crypto/include/tf-psa-crypto)

target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR}/components/include)

target_add_sources(${target} PRIVATE
    tf-psa-crypto/drivers/builtin/src/aes.c
    tf-psa-crypto/drivers/builtin/src/aesce.c
    tf-psa-crypto/drivers/builtin/src/aesni.c
    tf-psa-crypto/drivers/builtin/src/aria.c
    tf-psa-crypto/drivers/builtin/src/asn1parse.c
    tf-psa-crypto/drivers/builtin/src/asn1write.c
    tf-psa-crypto/drivers/builtin/src/base64.c
    tf-psa-crypto/drivers/builtin/src/bignum.c
    tf-psa-crypto/drivers/builtin/src/bignum_core.c
    tf-psa-crypto/drivers/builtin/src/bignum_mod.c
    tf-psa-crypto/drivers/builtin/src/bignum_mod_raw.c
    tf-psa-crypto/drivers/builtin/src/block_cipher.c
    tf-psa-crypto/drivers/builtin/src/camellia.c
    tf-psa-crypto/drivers/builtin/src/ccm.c
    tf-psa-crypto/drivers/builtin/src/chacha20.c
    tf-psa-crypto/drivers/builtin/src/chachapoly.c
    tf-psa-crypto/drivers/builtin/src/cipher.c
    tf-psa-crypto/drivers/builtin/src/cipher_wrap.c
    tf-psa-crypto/drivers/builtin/src/cmac.c
    tf-psa-crypto/drivers/builtin/src/constant_time.c
    tf-psa-crypto/drivers/builtin/src/ctr_drbg.c
    tf-psa-crypto/drivers/builtin/src/ecdh.c
    tf-psa-crypto/drivers/builtin/src/ecdsa.c
    tf-psa-crypto/drivers/builtin/src/ecjpake.c
    tf-psa-crypto/drivers/builtin/src/ecp.c
    tf-psa-crypto/drivers/builtin/src/ecp_curves.c
    tf-psa-crypto/drivers/builtin/src/ecp_curves_new.c
    tf-psa-crypto/drivers/builtin/src/entropy.c
    tf-psa-crypto/drivers/builtin/src/entropy_poll.c
    tf-psa-crypto/drivers/builtin/src/gcm.c
    tf-psa-crypto/drivers/builtin/src/hmac_drbg.c
    tf-psa-crypto/drivers/builtin/src/lmots.c
    tf-psa-crypto/drivers/builtin/src/lms.c
    tf-psa-crypto/drivers/builtin/src/md.c
    tf-psa-crypto/drivers/builtin/src/md5.c
    tf-psa-crypto/drivers/builtin/src/memory_buffer_alloc.c
    tf-psa-crypto/drivers/builtin/src/nist_kw.c
    tf-psa-crypto/drivers/builtin/src/oid.c
    tf-psa-crypto/drivers/builtin/src/pem.c
    tf-psa-crypto/drivers/builtin/src/pk.c
    tf-psa-crypto/drivers/builtin/src/pk_ecc.c
    tf-psa-crypto/drivers/builtin/src/pk_rsa.c
    tf-psa-crypto/drivers/builtin/src/pk_wrap.c
    tf-psa-crypto/drivers/builtin/src/pkcs5.c
    tf-psa-crypto/drivers/builtin/src/pkparse.c
    tf-psa-crypto/drivers/builtin/src/pkwrite.c
    tf-psa-crypto/drivers/builtin/src/platform.c
    tf-psa-crypto/drivers/builtin/src/platform_util.c
    tf-psa-crypto/drivers/builtin/src/poly1305.c
    tf-psa-crypto/drivers/builtin/src/psa_crypto_aead.c
    tf-psa-crypto/drivers/builtin/src/psa_crypto_cipher.c
    tf-psa-crypto/drivers/builtin/src/psa_crypto_ecp.c
    tf-psa-crypto/drivers/builtin/src/psa_crypto_ffdh.c
    tf-psa-crypto/drivers/builtin/src/psa_crypto_hash.c
    tf-psa-crypto/drivers/builtin/src/psa_crypto_mac.c
    tf-psa-crypto/drivers/builtin/src/psa_crypto_pake.c
    tf-psa-crypto/drivers/builtin/src/psa_crypto_rsa.c
    tf-psa-crypto/drivers/builtin/src/psa_util.c
    tf-psa-crypto/drivers/builtin/src/ripemd160.c
    tf-psa-crypto/drivers/builtin/src/rsa.c
    tf-psa-crypto/drivers/builtin/src/rsa_alt_helpers.c
    tf-psa-crypto/drivers/builtin/src/sha1.c
    tf-psa-crypto/drivers/builtin/src/sha3.c
    tf-psa-crypto/drivers/builtin/src/sha256.c
    tf-psa-crypto/drivers/builtin/src/sha512.c
    tf-psa-crypto/drivers/builtin/src/threading.c
    tf-psa-crypto/core/psa_crypto.c
    tf-psa-crypto/core/psa_crypto_client.c
    tf-psa-crypto/core/psa_crypto_driver_wrappers_no_static.c
    tf-psa-crypto/core/psa_crypto_slot_management.c
    tf-psa-crypto/core/psa_crypto_storage.c
    tf-psa-crypto/core/psa_its_file.c
    tf-psa-crypto/core/tf_psa_crypto_config.c
    tf-psa-crypto/core/tf_psa_crypto_version.c
    library/debug.c
    library/error.c
    library/mbedtls_config.c
    library/mps_reader.c
    library/mps_trace.c
    library/pkcs7.c
    library/x509.c
    library/x509_create.c
    library/x509_crl.c
    library/x509_crt.c
    library/x509_csr.c
    library/x509_oid.c
    library/x509write.c
    library/x509write_crt.c
    library/x509write_csr.c
    library/net_sockets.c
    library/ssl_cache.c
    library/ssl_ciphersuites.c
    library/ssl_client.c
    library/ssl_cookie.c
    library/ssl_debug_helpers_generated.c
    library/ssl_msg.c
    library/ssl_ticket.c
    library/ssl_tls.c
    library/ssl_tls12_client.c
    library/ssl_tls12_server.c
    library/ssl_tls13_keys.c
    library/ssl_tls13_server.c
    library/ssl_tls13_client.c
    library/ssl_tls13_generic.c
    library/timing.c
    library/version.c
    library/version_features.c
)

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${target} system_os)
