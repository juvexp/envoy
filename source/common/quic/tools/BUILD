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


envoy_cc_library(
    name = "quic_client_lib",
    srcs = [
        "quic_client.cc",
        "quic_client_base.cc",
        "quic_client_epoll_network_helper.cc",
        "quic_spdy_client_base.cc",
    ],
    hdrs = [
        "quic_client.h",
        "quic_client_base.h",
        "quic_client_epoll_network_helper.h",
        "quic_spdy_client_base.h",
    ],
    visibility = [
        ":internal",
        "//borg/monitoring/b3m:__pkg__",
        "//net/bandaid/internetto/wtrace/agent:__pkg__",
        "//net/grpc/examples/experimental_quic_server:__pkg__",
        "//net/grpc/internal/src/core/quic:__pkg__",
        "//net/grpc/internal/test/cpp/end2end:__pkg__",
        "//net/hostdatapath/testing/quic_fetcher:__pkg__",
    ],
    deps = [
        "//base",
        "//gfe/gfe2/base:epoll_executor",
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_epoll_alarm_factory_lib",
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//source/common/quic/core:quic_packet_reader_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_server_id_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/platform/impl:quic_socket_utils_lib",
    ],
)

envoy_cc_binary(
    name = "quic_client",
    srcs = [
        "quic_client_bin.cc",
    ],
    deps = [
        ":quic_client_lib",
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_server_id_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:proof_verifier_google3",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/platform/api:quic_url_lib",
        "//net/base:ipaddress",
        "//net/dns:hostlookup",
    ],
)

envoy_cc_library(
    name = "quic_simple_server_session_lib",
    srcs = [
        "quic_simple_crypto_server_stream_helper.cc",
        "quic_simple_server_session.cc",
        "quic_simple_server_stream.cc",
    ],
    hdrs = [
        "quic_simple_crypto_server_stream_helper.h",
        "quic_simple_server_session.h",
        "quic_simple_server_stream.h",
    ],
    visibility = [
        ":internal",
        # For //borg/monitoring/b3m:probes.
        "//borg/monitoring/b3m:__pkg__",
        # For grpc-over-quic
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_http_response_cache_lib",
        "//base",
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//webutil/http",
    ],
)

envoy_cc_library(
    name = "quic_http_response_cache_lib",
    srcs = [
        "quic_http_response_cache.cc",
    ],
    hdrs = [
        "quic_http_response_cache.h",
    ],
    visibility = [
        ":internal",
        "//borg/monitoring/b3m:__pkg__",
        "//gfe/test_tools/requests_test/backends:__pkg__",
        # For //video/streaming/testing/perf:vstress_client_test.
        "//video/streaming/testing/perf:__pkg__",
    ],
    deps = [
        "//base",
        "//file/base",
        "//file/base:path",
        "//source/common/quic/core:spdy_utils_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_mutex_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/platform/api:quic_url_lib",
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
    ],
)

envoy_cc_binary(
    name = "quic_server",
    srcs = [
        "quic_server_bin.cc",
    ],
    deps = [
        ":quic_server_lib",
        "//base",
        "//source/common/quic/core/crypto:proof_source_google3",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//net/httpsconnection:certificates_proto",
        "//net/httpsconnection:sslconnectionbase",
    ],
)

envoy_cc_library(
    name = "quic_server_lib",
    srcs = [
        "quic_server.cc",
        "quic_simple_dispatcher.cc",
    ],
    hdrs = [
        "quic_server.h",
        "quic_simple_dispatcher.h",
    ],
    visibility = [
        ":internal",
        "//borg/monitoring/b3m:__pkg__",
        # For //video/streaming/testing/perf:vstress_client_test.
        "//video/streaming/testing/perf:__pkg__",
        # For grpc-over-quic
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_http_response_cache_lib",
        ":quic_simple_server_session_lib",
        "//base",
        "//gfe/gfe2/base:epoll_executor",
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_epoll_alarm_factory_lib",
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packet_reader_lib",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/core:quic_version_manager_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/impl:quic_epoll_clock_lib",
        "//source/common/quic/platform/impl:quic_socket_utils_lib",
    ],
)

envoy_cc_binary(
    name = "quic_packet_printer",
    srcs = [
        "quic_packet_printer_bin.cc",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_binary(
    name = "quic_reject_reason_decoder",
    srcs = [
        "quic_reject_reason_decoder_bin.cc",
    ],
    deps = [
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

py_binary(
    name = "update_flags",
    srcs = ["update_flags.py"],
    deps = [
        "//pyglib",
        "//pyglib/flags",
        "//pyglib/logging",
    ],
)

py_library(
    name = "add-code-review-urls",
    srcs = ["add-code-review-urls.py"],
    deps = [
        "//pyglib",
        "//pyglib/flags",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
