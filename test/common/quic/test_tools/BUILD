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


envoy_cc_test_library(
    name = "rtt_stats_peer",
    srcs = [
        "rtt_stats_peer.cc",
    ],
    hdrs = [
        "rtt_stats_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core/congestion_control:rtt_stats_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_client_promised_info_peer",
    srcs = [
        "quic_client_promised_info_peer.cc",
    ],
    hdrs = [
        "quic_client_promised_info_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_client_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_config_peer",
    srcs = [
        "quic_config_peer.cc",
    ],
    hdrs = [
        "quic_config_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/platform/api:quic_uint128_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_crypto_server_config_peer",
    srcs = [
        "quic_crypto_server_config_peer.cc",
    ],
    hdrs = [
        "quic_crypto_server_config_peer.h",
    ],
    deps = [
        ":mock_clock",
        ":mock_random",
        ":quic_test_utils",
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_framer_peer",
    srcs = [
        "quic_framer_peer.cc",
    ],
    hdrs = [
        "quic_framer_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_packet_creator_peer",
    srcs = [
        "quic_packet_creator_peer.cc",
    ],
    hdrs = [
        "quic_packet_creator_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packet_creator_lib",
        "//source/common/quic/core:quic_packets_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_packet_generator_peer",
    srcs = [
        "quic_packet_generator_peer.cc",
    ],
    hdrs = [
        "quic_packet_generator_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packet_creator_lib",
        "//source/common/quic/core:quic_packet_generator_lib",
        "//source/common/quic/core:quic_packets_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_sent_packet_manager_peer",
    srcs = [
        "quic_sent_packet_manager_peer.cc",
    ],
    hdrs = [
        "quic_sent_packet_manager_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_sent_packet_manager_lib",
        "//source/common/quic/core/congestion_control:quic_congestion_control_hdrs",
    ],
)

envoy_cc_test_library(
    name = "quic_sustained_bandwidth_recorder_peer",
    srcs = [
        "quic_sustained_bandwidth_recorder_peer.cc",
    ],
    hdrs = [
        "quic_sustained_bandwidth_recorder_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_sustained_bandwidth_recorder_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_test_server",
    srcs = [
        "quic_test_server.cc",
    ],
    hdrs = [
        "quic_test_server.h",
    ],
    visibility = [
        ":internal",
        "//borg/monitoring/b3m:__pkg__",
    ],
    deps = [
        "//source/common/quic/core:quic_epoll_alarm_factory_lib",
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/tools:quic_server_lib",
        "//source/common/quic/tools:quic_simple_server_session_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_buffered_packet_store_peer",
    srcs = [
        "quic_buffered_packet_store_peer.cc",
    ],
    hdrs = [
        "quic_buffered_packet_store_peer.h",
    ],
    deps = [
        "//source/common/quic/core:quic_alarm_lib",
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_spdy_stream_peer",
    srcs = [
        "quic_spdy_stream_peer.cc",
    ],
    hdrs = [
        "quic_spdy_stream_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_spdy_session_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_session_peer",
    srcs = [
        "quic_session_peer.cc",
    ],
    hdrs = [
        "quic_session_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_spdy_session_peer",
    srcs = [
        "quic_spdy_session_peer.cc",
    ],
    hdrs = [
        "quic_spdy_session_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_stream_sequencer_buffer_peer",
    srcs = [
        "quic_stream_sequencer_buffer_peer.cc",
    ],
    hdrs = [
        "quic_stream_sequencer_buffer_peer.h",
    ],
    deps = [
        "//source/common/quic/core:quic_stream_sequencer_buffer_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_stream_sequencer_peer",
    srcs = [
        "quic_stream_sequencer_peer.cc",
    ],
    hdrs = [
        "quic_stream_sequencer_peer.h",
    ],
    deps = [
        ":quic_stream_sequencer_buffer_peer",
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_test_utils",
    srcs = [
        "crypto_test_utils.cc",
        "crypto_test_utils_google3.cc",
        "mock_quic_session_visitor.cc",
        "mock_quic_time_wait_list_manager.cc",
        "quic_client_peer.cc",
        "quic_connection_peer.cc",
        "quic_dispatcher_peer.cc",
        "quic_server_peer.cc",
        "quic_test_utils.cc",
    ],
    hdrs = [
        "crypto_test_utils.h",
        "crypto_test_utils_google3.h",
        "mock_quic_session_visitor.h",
        "mock_quic_time_wait_list_manager.h",
        "quic_client_peer.h",
        "quic_connection_peer.h",
        "quic_dispatcher_peer.h",
        "quic_server_peer.h",
        "quic_test_utils.h",
    ],
    data = [
        "//gfe/quic/core/crypto/testdata",
    ],
    deps = [
        ":mock_clock",
        ":mock_random",
        ":quic_framer_peer",
        ":quic_packet_generator_peer",
        ":quic_sent_packet_manager_peer",
        ":quic_stream_peer",
        ":simple_quic_framer",
        "//base",
        "//file/base",
        "//gfe/gfe2/test_tools:failure_lib",
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:quic_connection_close_delegate_interface_lib",
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_connection_stats_lib",
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packet_creator_lib",
        "//source/common/quic/core:quic_packet_reader_lib",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_received_packet_manager_lib",
        "//source/common/quic/core:quic_sent_packet_manager_lib",
        "//source/common/quic/core:quic_server_id_lib",
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/core:quic_spdy_session_lib",
        "//source/common/quic/core:quic_time_wait_list_manager_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/congestion_control:quic_congestion_control_hdrs",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:proof_source",
        "//source/common/quic/core/crypto:proof_source_google3",
        "//source/common/quic/core/crypto:proof_verifier_google3",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_endian_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/tools:quic_client_lib",
        "//source/common/quic/tools:quic_server_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//net/httpsconnection:cert_key",
        "//net/httpsconnection:certificates_proto",
        "//net/httpsconnection:file_cert_key_loader",
        "//net/httpsconnection:sslcertificateprovider",
        "//net/httpsconnection:sslconnectionbase",
        "//third_party/openssl:crypto",
    ],
)

envoy_cc_test(
    name = "quic_test_utils_test",
    srcs = [
        "quic_test_utils_test.cc",
    ],
    deps = [
        ":quic_test_utils",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test_library(
    name = "bad_packet_writer_lib",
    srcs = [
        "bad_packet_writer.cc",
    ],
    hdrs = [
        "bad_packet_writer.h",
    ],
    deps = [
        "//source/common/quic/core:quic_packet_writer_lib",
    ],
)

envoy_cc_test_library(
    name = "packet_reordering_writer_lib",
    srcs = [
        "packet_reordering_writer.cc",
    ],
    hdrs = [
        "packet_reordering_writer.h",
    ],
    deps = [
        "//source/common/quic/core:quic_packet_writer_lib",
    ],
)

envoy_cc_test_library(
    name = "packet_dropping_test_writer_lib",
    srcs = [
        "packet_dropping_test_writer.cc",
    ],
    hdrs = [
        "packet_dropping_test_writer.h",
    ],
    deps = [
        ":quic_test_client_lib",
        ":quic_test_utils",
        "//base",
        "//source/common/quic/core:quic_alarm_lib",
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//gfe/quic/platform/impl:quic_socket_utils_lib",
    ],
)

envoy_cc_test_library(
    name = "limited_mtu_test_writer_lib",
    srcs = [
        "limited_mtu_test_writer.cc",
    ],
    hdrs = [
        "limited_mtu_test_writer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/core:quic_packets_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_test_client_lib",
    srcs = [
        "quic_test_client.cc",
    ],
    hdrs = [
        "quic_test_client.h",
    ],
    deps = [
        ":quic_spdy_session_peer",
        ":quic_stream_peer",
        ":quic_test_utils",
        "//base",
        "//gfe/gfe2/base:epoll_executor",
        "//gfe/gfe2/quic:gfe_quic_dispatcher_lib",
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:quic_epoll_connection_helper_lib",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packet_creator_lib",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_server_id_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core:spdy_utils_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_stack_trace_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/platform/api:quic_url_lib",
        "//source/common/quic/tools:quic_client_lib",
        "//third_party/openssl:crypto",
    ],
)

envoy_cc_test_library(
    name = "quic_stream_peer",
    srcs = [
        "quic_stream_peer.cc",
    ],
    hdrs = [
        "quic_stream_peer.h",
    ],
    deps = [
        ":quic_stream_send_buffer_peer",
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_stream_send_buffer_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_stream_send_buffer_peer",
    srcs = [
        "quic_stream_send_buffer_peer.cc",
    ],
    hdrs = [
        "quic_stream_send_buffer_peer.h",
    ],
    deps = [
        "//source/common/quic/core:quic_stream_send_buffer_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_flow_controller_peer",
    srcs = [
        "quic_flow_controller_peer.cc",
    ],
    hdrs = [
        "quic_flow_controller_peer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_session_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_time_wait_list_manager_peer",
    srcs = [
        "quic_time_wait_list_manager_peer.cc",
    ],
    hdrs = [
        "quic_time_wait_list_manager_peer.h",
    ],
    deps = ["//source/common/quic/core:quic_time_wait_list_manager_lib"],
)

envoy_cc_test_library(
    name = "server_thread_lib",
    srcs = [
        "server_thread.cc",
    ],
    hdrs = [
        "server_thread.h",
    ],
    deps = [
        "quic_test_utils",
        "//base",
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_server_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_mutex_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/tools:quic_server_lib",
        "//thread",
    ],
)

envoy_cc_test_library(
    name = "mock_clock",
    srcs = [
        "mock_clock.cc",
    ],
    hdrs = [
        "mock_clock.h",
    ],
    deps = [
        "//base",
        "//gfe/gfe2/test_tools:includes",
        "//gfe/quic/platform/impl:quic_epoll_clock_lib",
    ],
)

envoy_cc_test_library(
    name = "mock_random",
    srcs = [
        "mock_random.cc",
    ],
    hdrs = [
        "mock_random.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core/crypto:quic_random_lib",
    ],
)

envoy_cc_test_library(
    name = "mock_quic_dispatcher",
    srcs = [
        "mock_quic_dispatcher.cc",
    ],
    hdrs = [
        "mock_quic_dispatcher.h",
    ],
    deps = [
        ":quic_test_utils",
        "//base",
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/tools:quic_server_lib",
    ],
)

envoy_cc_test_library(
    name = "mock_quic_spdy_client_stream",
    srcs = [
        "mock_quic_spdy_client_stream.cc",
    ],
    hdrs = [
        "mock_quic_spdy_client_stream.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:quic_header_list_lib",
        "//source/common/quic/core:quic_packets_lib",
    ],
)

envoy_cc_test_library(
    name = "mock_quic_client_promised_info",
    srcs = [
        "mock_quic_client_promised_info.cc",
    ],
    hdrs = [
        "mock_quic_client_promised_info.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_client_lib",
        "//source/common/quic/core:quic_packets_lib",
    ],
)

envoy_cc_test_library(
    name = "simple_quic_framer",
    srcs = [
        "simple_quic_framer.cc",
    ],
    hdrs = [
        "simple_quic_framer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_test_library(
    name = "simple_data_producer",
    srcs = [
        "simple_data_producer.cc",
    ],
    hdrs = [
        "simple_data_producer.h",
    ],
    deps = [
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/core:quic_stream_frame_data_producer_lib",
        "//source/common/quic/core:quic_stream_send_buffer_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
    ],
)

envoy_cc_test_library(
    name = "simple_session_notifier_lib",
    srcs = [
        "simple_session_notifier.cc",
    ],
    hdrs = [
        "simple_session_notifier.h",
    ],
    deps = [
        ":quic_test_utils",
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:session_notifier_interface_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
    ],
)

envoy_cc_test(
    name = "simple_session_notifier_test",
    srcs = ["simple_session_notifier_test.cc"],
    deps = [
        ":quic_test_utils",
        ":simple_session_notifier_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test_library(
    name = "fake_proof_source",
    srcs = [
        "fake_proof_source.cc",
    ],
    hdrs = [
        "fake_proof_source.h",
    ],
    deps = [
        ":quic_test_utils",
        "//source/common/quic/core/crypto:proof_source",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_test_library(
    name = "failing_proof_source",
    srcs = [
        "failing_proof_source.cc",
    ],
    hdrs = [
        "failing_proof_source.h",
    ],
    deps = [
        "//source/common/quic/core/crypto:proof_source",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_test(
    name = "crypto_test_utils_test",
    srcs = ["crypto_test_utils_test.cc"],
    deps = [
        ":quic_test_utils",
        "//source/common/quic/core:quic_session_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)
