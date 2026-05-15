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
target_include_directories(${target} PRIVATE ${SOURCE_TOP_DIR}/components/include)

target_add_sources(${target} PRIVATE
                    library/aes.c
					library/aesni.c
					library/arc4.c
					library/aria.c
					library/asn1parse.c
					library/asn1write.c
					library/base64.c
					library/bignum.c
					library/blowfish.c
					library/camellia.c
					library/ccm.c
					library/certs.c
					library/chacha20.c
					library/chachapoly.c
					library/cipher.c
					library/cipher_wrap.c
					library/cmac.c
					library/constant_time.c
					library/ctr_drbg.c
					library/debug.c
					library/des.c
					library/dhm.c
					library/ecdh.c
					library/ecdsa.c
					library/ecjpake.c
					library/ecp.c
					library/ecp_curves.c
					library/entropy.c
					library/entropy_poll.c
					library/error.c
					library/gcm.c
					library/havege.c
					library/hkdf.c
					library/hmac_drbg.c
					library/md.c
					library/md2.c
					library/md4.c
					library/md5.c
					library/memory_buffer_alloc.c
					library/mps_reader.c
					library/mps_trace.c
					library/net_sockets.c
					library/nist_kw.c
					library/oid.c
					library/padlock.c
					library/pem.c
					library/pk.c
					library/pk_wrap.c
					library/pkcs11.c
					library/pkcs12.c
					library/pkcs5.c
					library/pkparse.c
					library/pkwrite.c
					library/platform.c
					library/platform_util.c
					library/poly1305.c
					library/psa_crypto_aead.c
					library/psa_crypto_cipher.c
					library/psa_crypto_driver_wrappers.c
					library/psa_crypto_ecp.c
					library/psa_crypto_hash.c
					library/psa_crypto_mac.c
					library/psa_crypto_rsa.c
					library/psa_crypto.c
					library/psa_crypto_se.c
					library/psa_crypto_slot_management.c
					library/psa_crypto_storage.c
					library/psa_its_file.c
					library/ripemd160.c
					library/rsa.c
					library/rsa_internal.c
					library/sha1.c
					library/sha256.c
					library/sha512.c
					library/ssl_cache.c
					library/ssl_ciphersuites.c
					library/ssl_cli.c
					library/ssl_cookie.c
					library/ssl_msg.c
					library/ssl_srv.c
					library/ssl_ticket.c
					library/ssl_tls.c
					library/threading.c
					library/timing.c
					library/version.c
					library/version_features.c
					library/x509.c
					library/x509_create.c
					library/x509_crl.c
					library/x509_crt.c
					library/x509_csr.c
					library/x509write_crt.c
					library/x509write_csr.c
					library/xtea.c
)

target_link_libraries_if(CONFIG_QOSA_LINK_SYSTEM_LIBRARIES_FUNC THEN ${target} system_os)
