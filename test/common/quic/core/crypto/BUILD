licenses(["notice"])  # Apache 2


load(
   "//bazel:envoy_build_system.bzl",
   "envoy_cc_binary",
   "envoy_cc_library",
   "envoy_cc_test",
   "envoy_cc_test_library",
   "envoy_package",
)

envoy_package()


envoy_cc_test(
    name = "curve25519_key_exchange_test",
    srcs = ["curve25519_key_exchange_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "p256_key_exchange_test",
    srcs = ["p256_key_exchange_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_crypto_client_config_test",
    srcs = ["quic_crypto_client_config_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core:quic_server_id_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_endian_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_random",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "quic_crypto_server_config_test",
    srcs = ["quic_crypto_server_config_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_clock",
        "//test/common/quic/test_tools:quic_crypto_server_config_peer",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "transport_parameters_test",
    srcs = ["transport_parameters_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//third_party/openssl:crypto",
    ],
)

envoy_cc_test(
    name = "proof_test_google3",
    srcs = ["proof_test_google3.cc"],
    data = [
        "//gfe/quic/core/crypto/testdata",
    ],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:proof_source",
        "//base",
        "//concurrent/futures",
        "//file/base",
        "//file/base:path",
        "//gfe/gfe2/base:data_registry",
        "//gfe/gfe2/base:epoll_eventmanager",
        "//gfe/gfe2/mentat:mentat_client_lib",
        "//gfe/gfe2/mentat/test_tools:http_server_utils",
        "//gfe/gfe2/mentat/test_tools:mentat_client_test_peer_lib",
        "//gfe/gfe2/mentat/test_tools:standalone_oracle_utils_lib",
        "//gfe/gfe2/test_tools:failure_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_ip_address_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_test_utils",
        "//net/base:ipaddress",
        "//net/http2/server/lib/public:server",
        "//net/httpsconnection:sslconnectionbase",
        "//security/mentat/lib/internal:tls_oracle_signing_impl",
        "//testing/lib/stats/io:expvar_test_helper",
        "//testing/production_stub/public:testvalue",
        "//third_party/openssl:crypto",
        "//thread/future",
    ],
)

envoy_cc_test(
    name = "crypto_handshake_message_test",
    srcs = ["crypto_handshake_message_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//base",
        "//source/common/quic/platform/api:quic_endian_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "common_cert_set_test",
    srcs = ["common_cert_set_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//base",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "cert_compressor_test",
    srcs = ["cert_compressor_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//base",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "quic_compressed_certs_cache_test",
    srcs = ["quic_compressed_certs_cache_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//base",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "crypto_utils_test",
    srcs = [
        "crypto_utils_test.cc",
    ],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "quic_random_test",
    srcs = ["quic_random_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_random_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "null_encrypter_test",
    srcs = ["null_encrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "null_decrypter_test",
    srcs = ["null_decrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "aes_128_gcm_12_encrypter_test",
    srcs = ["aes_128_gcm_12_encrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "aes_128_gcm_12_decrypter_test",
    srcs = ["aes_128_gcm_12_decrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "aes_128_gcm_encrypter_test",
    srcs = ["aes_128_gcm_encrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "aes_128_gcm_decrypter_test",
    srcs = ["aes_128_gcm_decrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "aes_256_gcm_encrypter_test",
    srcs = ["aes_256_gcm_encrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "aes_256_gcm_decrypter_test",
    srcs = ["aes_256_gcm_decrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "chacha20_poly1305_encrypter_test",
    srcs = ["chacha20_poly1305_encrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "chacha20_poly1305_decrypter_test",
    srcs = ["chacha20_poly1305_decrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "chacha20_poly1305_tls_encrypter_test",
    srcs = ["chacha20_poly1305_tls_encrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "chacha20_poly1305_tls_decrypter_test",
    srcs = ["chacha20_poly1305_tls_decrypter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "crypto_framer_test",
    srcs = ["crypto_framer_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "quic_tls_adapter_test",
    srcs = ["quic_tls_adapter_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//third_party/openssl:crypto",
    ],
)

envoy_cc_test(
    name = "crypto_server_test",
    srcs = ["crypto_server_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:proof_source",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//base",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_socket_address_coder_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_endian_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:failing_proof_source",
        "//test/common/quic/test_tools:mock_clock",
        "//test/common/quic/test_tools:mock_random",
        "//test/common/quic/test_tools:quic_crypto_server_config_peer",
        "//test/common/quic/test_tools:quic_test_utils",
        "//third_party/openssl:crypto",
    ],
)

envoy_cc_test(
    name = "crypto_secret_boxer_test",
    srcs = ["crypto_secret_boxer_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//base",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "channel_id_test",
    srcs = ["channel_id_test.cc"],
    deps = [
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)
