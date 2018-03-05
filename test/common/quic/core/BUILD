# Description:
#   Code for QUIC (see go/quic) which is used in production, either in
#   the GFE, gRPC or Chrome and which is shared between them.

package(default_visibility = ["//source/common/quic/core:internal"])

load("//gfe/gfe2/tools:build_defs.bzl", "envoy_cc_test_library")
load("//tools/build_rules:build_test.bzl", "build_test")

package_group(
    name = "internal",

    # If you are adding a package to this list, please make sure to also update
    # _quic_test_patterns in
    # //depot/google3/gfe/tools/continuous_builds/gfe.blueprint otherwise you
    # risk build breakage.
    packages = [
        "//gfe/cluster_broker/...",
        "//gfe/gfe2/...",
        "//source/common/quic/...",
        "//gfe/test_tools/...",
        "//gfe/tools/...",
        "//net/hostdatapath/common/protocols",
        "//third_party/pcc_quic/...",
        "//video/streaming/quic",
    ],
)

envoy_cc_test(
    name = "host_port_pair_test",
    srcs = [
        "host_port_pair_test.cc",
    ],
    deps = [
        "//source/common/quic/core:host_port_pair_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_server_id_test",
    srcs = [
        "quic_server_id_test.cc",
    ],
    deps = [
        "//source/common/quic/core:quic_server_id_lib",
        "//source/common/quic/platform/api:quic_estimate_memory_usage_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_time_test",
    srcs = [
        "quic_time_test.cc",
    ],
    deps = [
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:mock_clock",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_arena_scoped_ptr_test",
    srcs = ["quic_arena_scoped_ptr_test.cc"],
    deps = [
        "//source/common/quic/core:quic_arena_scoped_ptr_lib",
        "//source/common/quic/core:quic_one_block_arena_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_one_block_arena_test",
    srcs = ["quic_one_block_arena_test.cc"],
    deps = [
        "//source/common/quic/core:quic_one_block_arena_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_bandwidth_test",
    srcs = [
        "quic_bandwidth_test.cc",
    ],
    deps = [
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_buffer_pool_google3_test",
    srcs = ["quic_buffer_pool_google3_test.cc"],
    deps = [
        "//source/common/quic/core:quic_buffer_pool_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_alarm_test",
    srcs = ["quic_alarm_test.cc"],
    deps = [
        "//source/common/quic/core:quic_alarm_lib",
        "//base",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_header_list_test",
    srcs = ["quic_header_list_test.cc"],
    deps = [
        "//source/common/quic/core:quic_header_list_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_tag_test",
    srcs = ["quic_tag_test.cc"],
    deps = [
        "//source/common/quic/core:quic_tag_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_error_codes_test",
    srcs = ["quic_error_codes_test.cc"],
    deps = [
        "//source/common/quic/core:quic_error_codes_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_versions_test",
    srcs = ["quic_versions_test.cc"],
    deps = [
        "//source/common/quic/core:quic_versions_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_version_manager_test",
    srcs = ["quic_version_manager_test.cc"],
    deps = [
        "//source/common/quic/core:quic_version_manager_lib",
        "//source/common/quic/core:quic_versions_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_utils_test",
    srcs = ["quic_utils_test.cc"],
    deps = [
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_write_blocked_list_test",
    srcs = ["quic_write_blocked_list_test.cc"],
    deps = [
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_unacked_packet_map_test",
    srcs = ["quic_unacked_packet_map_test.cc"],
    deps = [
        "//source/common/quic/core:quic_unacked_packet_map_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_sent_packet_manager_test",
    srcs = ["quic_sent_packet_manager_test.cc"],
    deps = [
        "//source/common/quic/core:quic_pending_retransmission_lib",
        "//source/common/quic/core:quic_sent_packet_manager_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_sent_packet_manager_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_sustained_bandwidth_recorder_test",
    srcs = ["quic_sustained_bandwidth_recorder_test.cc"],
    deps = [
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_sustained_bandwidth_recorder_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_connection_test",
    srcs = ["quic_connection_test.cc"],
    shard_count = 16,
    deps = [
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//source/common/quic/core/congestion_control:quic_congestion_control_hdrs",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_ref_counted_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:mock_clock",
        "//source/common/quic/test_tools:mock_random",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_framer_peer",
        "//source/common/quic/test_tools:quic_packet_creator_peer",
        "//source/common/quic/test_tools:quic_packet_generator_peer",
        "//source/common/quic/test_tools:quic_sent_packet_manager_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:simple_data_producer",
        "//source/common/quic/test_tools:simple_quic_framer",
        "//source/common/quic/test_tools:simple_session_notifier_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_epoll_alarm_factory_test",
    srcs = ["quic_epoll_alarm_factory_test.cc"],
    deps = [
        "//source/common/quic/core:quic_epoll_alarm_factory_lib",
        "//gfe/gfe2/test_tools:includes",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/impl:quic_epoll_clock_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_epoll_connection_helper_test",
    srcs = ["quic_epoll_connection_helper_test.cc"],
    deps = [
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//gfe/gfe2/test_tools:includes",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_received_packet_manager_test",
    srcs = ["quic_received_packet_manager_test.cc"],
    deps = [
        "//source/common/quic/core:quic_connection_stats_lib",
        "//source/common/quic/core:quic_received_packet_manager_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_simple_buffer_allocator_test",
    srcs = ["quic_simple_buffer_allocator_test.cc"],
    deps = [
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_socket_address_coder_test",
    srcs = ["quic_socket_address_coder_test.cc"],
    deps = [
        "//source/common/quic/core:quic_socket_address_coder_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_flow_controller_test",
    srcs = ["quic_flow_controller_test.cc"],
    deps = [
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_flow_controller_peer",
        "//source/common/quic/test_tools:quic_sent_packet_manager_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_config_test",
    srcs = ["quic_config_test.cc"],
    deps = [
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_spdy_stream_test",
    srcs = ["quic_spdy_stream_test.cc"],
    deps = [
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/test_tools:quic_flow_controller_peer",
        "//source/common/quic/test_tools:quic_session_peer",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_headers_stream_test",
    srcs = ["quic_headers_stream_test.cc"],
    deps = [
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        "//source/common/quic/spdy_utils/core:spdy_alt_svc_wire_format_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_server_session_base_test",
    srcs = ["quic_server_session_base_test.cc"],
    deps = [
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:fake_proof_source",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_crypto_server_config_peer",
        "//source/common/quic/test_tools:quic_sent_packet_manager_peer",
        "//source/common/quic/test_tools:quic_session_peer",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_spdy_stream_peer",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_sustained_bandwidth_recorder_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/tools:quic_simple_server_session_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_spdy_server_stream_base_test",
    srcs = ["quic_spdy_server_stream_base_test.cc"],
    deps = [
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_spdy_session_test",
    srcs = ["quic_spdy_session_test.cc"],
    tags = ["xplatform"],
    deps = [
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_flow_controller_peer",
        "//source/common/quic/test_tools:quic_session_peer",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_spdy_stream_peer",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_stream_send_buffer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_session_test",
    srcs = ["quic_session_test.cc"],
    tags = ["xplatform"],
    deps = [
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_flow_controller_peer",
        "//source/common/quic/test_tools:quic_session_peer",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_stream_send_buffer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_crypto_stream_test",
    srcs = ["quic_crypto_stream_test.cc"],
    deps = [
        "//source/common/quic/core:quic_session_lib",
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "tls_handshaker_test",
    srcs = ["tls_handshaker_test.cc"],
    deps = [
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:fake_proof_source",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_stream_sequencer_test",
    srcs = ["quic_stream_sequencer_test.cc"],
    deps = [
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:mock_clock",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_stream_sequencer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_stream_sequencer_buffer_test",
    srcs = ["quic_stream_sequencer_buffer_test.cc"],
    deps = [
        "//source/common/quic/core:quic_stream_sequencer_buffer_lib",
        "//base",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:mock_clock",
        "//source/common/quic/test_tools:quic_stream_sequencer_buffer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "spdy_utils_test",
    srcs = ["spdy_utils_test.cc"],
    deps = [
        "//source/common/quic/core:spdy_utils_lib",
        "//base",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_stream_test",
    srcs = ["quic_stream_test.cc"],
    deps = [
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//base",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_test_mem_slice_vector_lib",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_flow_controller_peer",
        "//source/common/quic/test_tools:quic_session_peer",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_stream_sequencer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_framer_test",
    srcs = ["quic_framer_test.cc"],
    deps = [
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_framer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:simple_data_producer",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_ietf_framer_test",
    srcs = ["quic_ietf_framer_test.cc"],
    deps = [
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_framer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:simple_data_producer",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_packet_creator_test",
    srcs = ["quic_packet_creator_test.cc"],
    deps = [
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_packet_creator_lib",
        "//source/common/quic/core:quic_pending_retransmission_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_framer_peer",
        "//source/common/quic/test_tools:quic_packet_creator_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:simple_data_producer",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_packet_generator_test",
    srcs = ["quic_packet_generator_test.cc"],
    deps = [
        "//source/common/quic/core:quic_packet_generator_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:mock_random",
        "//source/common/quic/test_tools:quic_framer_peer",
        "//source/common/quic/test_tools:quic_packet_creator_peer",
        "//source/common/quic/test_tools:quic_packet_generator_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:simple_data_producer",
        "//source/common/quic/test_tools:simple_quic_framer",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_time_wait_list_manager_test",
    srcs = ["quic_time_wait_list_manager_test.cc"],
    deps = [
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_epoll_alarm_factory_lib",
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_wait_list_manager_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//gfe/gfe2/test_tools:includes",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:quic_time_wait_list_manager_peer",
        "//testing/base/public:gunit_main",
        "//third_party/absl/time",
    ],
)

envoy_cc_test(
    name = "quic_stream_send_buffer_test",
    srcs = ["quic_stream_send_buffer_test.cc"],
    deps = [
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/core:quic_stream_send_buffer_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_stream_send_buffer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_control_frame_manager_test",
    srcs = ["quic_control_frame_manager_test.cc"],
    deps = [
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_crypto_server_stream_test",
    srcs = ["quic_crypto_server_stream_test.cc"],
    deps = [
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:failing_proof_source",
        "//source/common/quic/test_tools:fake_proof_source",
        "//source/common/quic/test_tools:quic_crypto_server_config_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_spdy_client_session_test",
    srcs = ["quic_spdy_client_session_test.cc"],
    deps = [
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:mock_quic_spdy_client_stream",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_packet_creator_peer",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
        "//third_party/absl/base",
    ],
)

envoy_cc_test(
    name = "quic_crypto_client_stream_test",
    srcs = ["quic_crypto_client_stream_test.cc"],
    deps = [
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_server_id_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_stream_sequencer_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:simple_quic_framer",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_spdy_client_stream_test",
    size = "small",
    srcs = ["quic_spdy_client_stream_test.cc"],
    deps = [
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//base",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_client_promised_info_test",
    size = "small",
    srcs = ["quic_client_promised_info_test.cc"],
    deps = [
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//base",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_client_promised_info_peer",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_client_push_promise_index_test",
    size = "small",
    srcs = ["quic_client_push_promise_index_test.cc"],
    deps = [
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//base",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:mock_quic_client_promised_info",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "chlo_extractor_test",
    srcs = ["chlo_extractor_test.cc"],
    deps = [
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "stateless_rejector_test",
    srcs = ["stateless_rejector_test.cc"],
    deps = [
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:proof_source",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/test_tools:quic_crypto_server_config_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_dispatcher_test",
    srcs = ["quic_dispatcher_test.cc"],
    deps = [
        "//source/common/quic/core:quic_epoll_alarm_factory_lib",
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_time_wait_list_manager_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//gfe/gfe2/base:epoll_executor",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:fake_proof_source",
        "//source/common/quic/test_tools:quic_buffered_packet_store_peer",
        "//source/common/quic/test_tools:quic_crypto_server_config_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:quic_time_wait_list_manager_peer",
        "//source/common/quic/tools:quic_simple_server_session_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_buffered_packet_store_test",
    srcs = ["quic_buffered_packet_store_test.cc"],
    deps = [
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:mock_clock",
        "//source/common/quic/test_tools:quic_buffered_packet_store_peer",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "quic_data_writer_test",
    srcs = ["quic_data_writer_test.cc"],
    deps = [
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "packet_number_indexed_queue_test",
    srcs = ["packet_number_indexed_queue_test.cc"],
    deps = [
        "//source/common/quic/core:packet_number_indexed_queue_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_library(
    name = "end_to_end_test_lib",
    testonly = 1,
    srcs = [
        "end_to_end_test.cc",
    ],
    deps = [
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packet_creator_lib",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//base",
        "//gfe/gfe2/base:epoll_executor",
        "//gfe/gfe2/jetstream:http_constants_lib",
        "//gfe/gfe2/test_tools:valgrind_utils_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_test_loopback_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/platform/impl:quic_socket_utils_lib",
        "//source/common/quic/test_tools:bad_packet_writer_lib",
        "//source/common/quic/test_tools:packet_dropping_test_writer_lib",
        "//source/common/quic/test_tools:packet_reordering_writer_lib",
        "//source/common/quic/test_tools:quic_config_peer",
        "//source/common/quic/test_tools:quic_flow_controller_peer",
        "//source/common/quic/test_tools:quic_sent_packet_manager_peer",
        "//source/common/quic/test_tools:quic_session_peer",
        "//source/common/quic/test_tools:quic_spdy_session_peer",
        "//source/common/quic/test_tools:quic_stream_peer",
        "//source/common/quic/test_tools:quic_stream_sequencer_peer",
        "//source/common/quic/test_tools:quic_test_client_lib",
        "//source/common/quic/test_tools:quic_test_server",
        "//source/common/quic/test_tools:quic_test_utils",
        "//source/common/quic/test_tools:server_thread_lib",
        "//source/common/quic/tools:quic_client_lib",
        "//source/common/quic/tools:quic_http_response_cache_lib",
        "//source/common/quic/tools:quic_server_lib",
        "//source/common/quic/tools:quic_simple_server_session_lib",
        "//net/util",
        "//testing/base/public:gunit_main",
        "//util/time:clock",
    ],
)

envoy_cc_test(
    name = "end_to_end_test",
    size = "large",
    timeout = "moderate",
    data = [
        "//tools/test:broken_under_ipv6_only",
    ],
    shard_count = 36,
    deps = [
        "//source/common/quic/core:end_to_end_test_lib",
    ],
)
