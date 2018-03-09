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
    name = "quic_client_test",
    srcs = ["quic_client_test.cc"],
    data = [
        "//tools/test:broken_under_ipv6_only",
    ],
    deps = [
        "//source/common/quic/tools:quic_client_lib",
        "//base",
        "//file/base:path",
        "//file/util:linux_fileops",
        "//gfe/gfe2/base:epoll_executor",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/platform/api:quic_test_loopback_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_test_utils",
        "//net/util",
    ],
)

envoy_cc_test(
    name = "quic_simple_crypto_server_stream_helper_test",
    srcs = ["quic_simple_crypto_server_stream_helper_test.cc"],
    deps = [
        "//source/common/quic/tools:quic_simple_server_session_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_random",
    ],
)

envoy_cc_test(
    name = "quic_simple_server_session_test",
    srcs = ["quic_simple_server_session_test.cc"],
    deps = [
        "//source/common/quic/tools:quic_simple_server_session_lib",
        "//base",
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//test/common/quic/test_tools:quic_config_peer",
        "//test/common/quic/test_tools:quic_sent_packet_manager_peer",
        "//test/common/quic/test_tools:quic_session_peer",
        "//test/common/quic/test_tools:quic_spdy_session_peer",
        "//test/common/quic/test_tools:quic_spdy_stream_peer",
        "//test/common/quic/test_tools:quic_stream_peer",
        "//test/common/quic/test_tools:quic_sustained_bandwidth_recorder_peer",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "quic_simple_server_stream_test",
    srcs = ["quic_simple_server_stream_test.cc"],
    deps = [
        "//source/common/quic/tools:quic_http_response_cache_lib",
        "//source/common/quic/tools:quic_simple_server_session_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_spdy_session_peer",
        "//test/common/quic/test_tools:quic_stream_peer",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "quic_http_response_cache_test",
    srcs = ["quic_http_response_cache_test.cc"],
    data = ["//source/common/quic/test_tools:quic_http_response_cache_data"],
    deps = [
        "//source/common/quic/tools:quic_http_response_cache_lib",
        "//file/localfile",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_test(
    name = "quic_server_test",
    srcs = ["quic_server_test.cc"],
    data = [
        "//tools/test:broken_under_ipv6_only",
    ],
    deps = [
        "//source/common/quic/tools:quic_server_lib",
        "//source/common/quic/tools:quic_simple_server_session_lib",
        "//base",
        "//source/common/quic/core:quic_epoll_alarm_factory_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/platform/api:quic_test_loopback_lib",
        "//test/common/quic/test_tools:mock_quic_dispatcher",
        "//test/common/quic/test_tools:quic_test_utils",
        "//net/util",
    ],
)
