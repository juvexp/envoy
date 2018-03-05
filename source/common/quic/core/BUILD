# Description:
#   Code for QUIC (see go/quic) which is used in production, either in
#   the GFE, gRPC or Chrome and which is shared between them.

package(default_visibility = [":internal"])

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

envoy_cc_library(
    name = "quic_blocked_writer_interface_lib",
    hdrs = [
        "quic_blocked_writer_interface.h",
    ],
    deps = ["//source/common/quic/platform/api:quic_export_lib"],
)

envoy_cc_library(
    name = "host_port_pair_lib",
    srcs = [
        "host_port_pair.cc",
    ],
    hdrs = [
        "host_port_pair.h",
    ],
    deps = [
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_packet_reader_lib",
    srcs = [
        "quic_packet_reader.cc",
    ],
    hdrs = [
        "quic_packet_reader.h",
        "quic_process_packet_interface.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_packets_lib",
        "//base",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/impl:quic_socket_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_packet_writer_lib",
    srcs = [
        "quic_default_packet_writer.cc",
        "quic_packet_writer_wrapper.cc",
    ],
    hdrs = [
        "quic_default_packet_writer.h",
        "quic_packet_writer.h",
        "quic_packet_writer_wrapper.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_packets_lib",
        ":quic_types_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/impl:quic_socket_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_server_id_lib",
    srcs = [
        "quic_server_id.cc",
    ],
    hdrs = [
        "privacy_mode.h",
        "quic_server_id.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
        "//net/hostdatapath/testing/quic_fetcher:__pkg__",
    ],
    deps = [
        ":host_port_pair_lib",
        "//base",
        "//source/common/quic/platform/api:quic_estimate_memory_usage_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_time_lib",
    srcs = [
        "quic_time.cc",
    ],
    hdrs = [
        "quic_time.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_arena_scoped_ptr_lib",
    hdrs = ["quic_arena_scoped_ptr.h"],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_aligned_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "quic_one_block_arena_lib",
    hdrs = ["quic_one_block_arena.h"],
    deps = [
        ":quic_arena_scoped_ptr_lib",
        ":quic_types_lib",
        "//base",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "quic_bandwidth_lib",
    srcs = [
        "quic_bandwidth.cc",
    ],
    hdrs = [
        "quic_bandwidth.h",
    ],
    deps = [
        ":quic_constants_lib",
        ":quic_time_lib",
        ":quic_types_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_buffer_pool_lib",
    hdrs = [
        "quic_buffer_pool_google3.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_buffer_allocator_lib",
        "//base",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
    ],
)

envoy_cc_library(
    name = "quic_alarm_lib",
    srcs = [
        "quic_alarm.cc",
    ],
    hdrs = [
        "quic_alarm.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_arena_scoped_ptr_lib",
        ":quic_time_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_data_lib",
    srcs = [
        "quic_data_reader.cc",
        "quic_data_writer.cc",
    ],
    hdrs = [
        "quic_data_reader.h",
        "quic_data_writer.h",
    ],
    deps = [
        ":quic_packets_lib",
        ":quic_types_lib",
        ":quic_utils_lib",
        "//base",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_endian_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_library(
    name = "quic_header_list_lib",
    srcs = ["quic_header_list.cc"],
    hdrs = ["quic_header_list.h"],
    deps = [
        ":quic_packets_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/spdy_utils/core:spdy_header_block_lib",
        "//source/common/quic/spdy_utils/core:spdy_headers_handler_interface",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
    ],
)

envoy_cc_library(
    name = "quic_connection_close_delegate_interface_lib",
    hdrs = ["quic_connection_close_delegate_interface.h"],
    deps = [
        ":quic_error_codes_lib",
        ":quic_types_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_tag_lib",
    srcs = [
        "quic_tag.cc",
    ],
    hdrs = [
        "quic_tag.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_constants_lib",
    srcs = ["quic_constants.cc"],
    hdrs = ["quic_constants.h"],
    deps = [
        ":quic_types_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_error_codes_lib",
    srcs = [
        "quic_error_codes.cc",
    ],
    hdrs = [
        "quic_error_codes.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_utils_lib",
    srcs = ["quic_utils.cc"],
    hdrs = ["quic_utils.h"],
    visibility = [
        ":internal",
        "//borg/monitoring/b3m:__pkg__",
        "//net/bandaid/internetto:__subpackages__",
        "//net/bandaid/internetto/wtrace/agent:__pkg__",
        "//net/grpc/internal/src/core/quic:__pkg__",
        "//net/hostdatapath/testing/quic_fetcher:__pkg__",
        # For // video/streaming/testing/perf:vstress_lib.
        "//video/streaming/testing/perf:__pkg__",
    ],
    deps = [
        ":quic_constants_lib",
        ":quic_error_codes_lib",
        ":quic_types_lib",
        "//base",
        "//source/common/quic/platform/api:quic_aligned_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_prefetch_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_library(
    name = "quic_versions_lib",
    srcs = [
        "quic_versions.cc",
    ],
    hdrs = [
        "quic_versions.h",
    ],
    visibility = [
        ":internal",
        "//security/alia_handshaker:__subpackages__",
    ],
    deps = [
        ":quic_tag_lib",
        ":quic_types_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_endian_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_version_manager_lib",
    srcs = [
        "quic_version_manager.cc",
    ],
    hdrs = [
        "quic_version_manager.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_versions_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
    ],
)

envoy_cc_library(
    name = "quic_ack_listener_interface_lib",
    srcs = ["quic_ack_listener_interface.cc"],
    hdrs = ["quic_ack_listener_interface.h"],
    deps = [
        ":quic_time_lib",
        ":quic_types_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_ref_counted_lib",
    ],
)

envoy_cc_library(
    name = "session_notifier_interface_lib",
    hdrs = ["session_notifier_interface.h"],
    deps = [
        ":quic_time_lib",
        "//source/common/quic/core/frames:quic_frames_lib",
    ],
)

envoy_cc_library(
    name = "quic_pending_retransmission_lib",
    hdrs = ["quic_pending_retransmission.h"],
    deps = [
        ":quic_transmission_info_lib",
        ":quic_types_lib",
        "//source/common/quic/core/frames:quic_frames_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_transmission_info_lib",
    srcs = ["quic_transmission_info.cc"],
    hdrs = ["quic_transmission_info.h"],
    deps = [
        ":quic_ack_listener_interface_lib",
        ":quic_types_lib",
        "//source/common/quic/core/frames:quic_frames_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_packets_lib",
    srcs = [
        "quic_packets.cc",
        "quic_write_blocked_list.cc",
    ],
    hdrs = [
        "quic_packets.h",
        "quic_write_blocked_list.h",
    ],
    visibility = [
        ":internal",
        "//borg/monitoring/b3m:__pkg__",
        "//net/bandaid/internetto/wtrace/agent:__pkg__",
        "//net/grpc/internal/src/core/quic:__pkg__",
        "//net/hostdatapath/testing/quic_fetcher:__pkg__",
        # For //video/streaming/testing/perf:vstress_lib.
        "//video/streaming/testing/perf:__pkg__",
    ],
    deps = [
        ":quic_ack_listener_interface_lib",
        ":quic_bandwidth_lib",
        ":quic_constants_lib",
        ":quic_error_codes_lib",
        ":quic_time_lib",
        ":quic_types_lib",
        ":quic_utils_lib",
        ":quic_versions_lib",
        "//base",
        "//source/common/quic/core/frames:quic_frames_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/spdy_utils/core:priority_write_scheduler_lib",
    ],
)

envoy_cc_library(
    name = "quic_types_lib",
    srcs = [
        "quic_types.cc",
    ],
    hdrs = [
        "quic_types.h",
    ],
    deps = [
        ":quic_time_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "spdy_utils_lib",
    srcs = [
        "spdy_utils.cc",
    ],
    hdrs = [
        "spdy_utils.h",
    ],
    visibility = [
        ":internal",
        # For //borg/monitoring/b3m:probes.
        "//borg/monitoring/b3m:__pkg__",
        "//net/grpc/internal/src/core/quic:__pkg__",
        "//net/bandaid/internetto/wtrace/agent:__pkg__",
        "//net/hostdatapath/testing/quic_fetcher:__pkg__",
    ],
    deps = [
        ":quic_header_list_lib",
        ":quic_packets_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/platform/api:quic_url_utils_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
    ],
)

envoy_cc_library(
    name = "quic_unacked_packet_map_lib",
    srcs = [
        "quic_unacked_packet_map.cc",
    ],
    hdrs = [
        "quic_unacked_packet_map.h",
    ],
    deps = [
        ":quic_connection_stats_lib",
        ":quic_packets_lib",
        ":quic_transmission_info_lib",
        ":quic_utils_lib",
        ":session_notifier_interface_lib",
        "//base",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_sent_packet_manager_lib",
    srcs = [
        "quic_sent_packet_manager.cc",
    ],
    hdrs = [
        "quic_sent_packet_manager.h",
    ],
    deps = [
        ":quic_connection_stats_lib",
        ":quic_packets_lib",
        ":quic_pending_retransmission_lib",
        ":quic_sustained_bandwidth_recorder_lib",
        ":quic_transmission_info_lib",
        ":quic_types_lib",
        ":quic_unacked_packet_map_lib",
        ":quic_utils_lib",
        "//base",
        "//source/common/quic/core/congestion_control:general_loss_algorithm_lib",
        "//source/common/quic/core/congestion_control:pacing_sender_lib",
        "//source/common/quic/core/congestion_control:quic_congestion_control_lib",
        "//source/common/quic/core/congestion_control:rtt_stats_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_sustained_bandwidth_recorder_lib",
    srcs = [
        "quic_sustained_bandwidth_recorder.cc",
    ],
    hdrs = [
        "quic_sustained_bandwidth_recorder.h",
    ],
    deps = [
        ":quic_bandwidth_lib",
        ":quic_time_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "quic_connection_stats_lib",
    srcs = ["quic_connection_stats.cc"],
    hdrs = [
        "quic_connection_stats.h",
    ],
    deps = [
        ":quic_bandwidth_lib",
        ":quic_packets_lib",
        ":quic_time_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_connection_lib",
    srcs = [
        "quic_connection.cc",
    ],
    hdrs = [
        "quic_connection.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_alarm_factory_lib",
        ":quic_alarm_lib",
        ":quic_bandwidth_lib",
        ":quic_blocked_writer_interface_lib",
        ":quic_config_lib",
        ":quic_connection_stats_lib",
        ":quic_framer_lib",
        ":quic_one_block_arena_lib",
        ":quic_packet_creator_lib",
        ":quic_packet_generator_lib",
        ":quic_packet_writer_lib",
        ":quic_packets_lib",
        ":quic_pending_retransmission_lib",
        ":quic_received_packet_manager_lib",
        ":quic_sent_packet_manager_lib",
        ":quic_time_lib",
        ":quic_types_lib",
        ":quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_alarm_factory_lib",
    hdrs = [
        "quic_alarm_factory.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        "quic_alarm_lib",
        "quic_one_block_arena_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_epoll_alarm_factory_lib",
    srcs = [
        "quic_epoll_alarm_factory.cc",
    ],
    hdrs = [
        "quic_epoll_alarm_factory.h",
    ],
    deps = [
        ":quic_alarm_factory_lib",
        ":quic_alarm_lib",
        ":quic_arena_scoped_ptr_lib",
        ":quic_one_block_arena_lib",
        "//base",
        "//gfe/gfe2/base:epoll_executor",
    ],
)

envoy_cc_library(
    name = "quic_epoll_connection_helper_lib",
    srcs = [
        "quic_epoll_connection_helper.cc",
    ],
    hdrs = [
        "quic_epoll_connection_helper.h",
    ],
    deps = [
        ":quic_buffer_pool_lib",
        ":quic_connection_lib",
        ":quic_packet_writer_lib",
        ":quic_packets_lib",
        ":quic_simple_buffer_allocator_lib",
        ":quic_time_lib",
        ":quic_utils_lib",
        "//base",
        "//gfe/gfe2/base:epoll_executor",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/impl:quic_epoll_clock_lib",
        "//source/common/quic/platform/impl:quic_socket_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_received_packet_manager_lib",
    srcs = ["quic_received_packet_manager.cc"],
    hdrs = ["quic_received_packet_manager.h"],
    deps = [
        ":quic_config_lib",
        ":quic_connection_stats_lib",
        ":quic_framer_lib",
        ":quic_packets_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "quic_buffer_allocator_lib",
    srcs = [
        "quic_buffer_allocator.cc",
    ],
    hdrs = [
        "quic_buffer_allocator.h",
    ],
    deps = ["//source/common/quic/platform/api:quic_export_lib"],
)

envoy_cc_library(
    name = "quic_simple_buffer_allocator_lib",
    srcs = [
        "quic_simple_buffer_allocator.cc",
    ],
    hdrs = [
        "quic_simple_buffer_allocator.h",
    ],
    visibility = [
        ":internal",
        # For grpc-over-quic
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_buffer_allocator_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_socket_address_coder_lib",
    srcs = ["quic_socket_address_coder.cc"],
    hdrs = ["quic_socket_address_coder.h"],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_session_lib",
    srcs = [
        "quic_control_frame_manager.cc",
        "quic_crypto_client_handshaker.cc",
        "quic_crypto_client_stream.cc",
        "quic_crypto_handshaker.cc",
        "quic_crypto_server_handshaker.cc",
        "quic_crypto_server_stream.cc",
        "quic_crypto_stream.cc",
        "quic_flow_controller.cc",
        "quic_session.cc",
        "quic_stream.cc",
        "quic_stream_sequencer.cc",
        "tls_client_handshaker.cc",
        "tls_handshaker.cc",
        "tls_server_handshaker.cc",
    ],
    hdrs = [
        "quic_control_frame_manager.h",
        "quic_crypto_client_handshaker.h",
        "quic_crypto_client_stream.h",
        "quic_crypto_handshaker.h",
        "quic_crypto_server_handshaker.h",
        "quic_crypto_server_stream.h",
        "quic_crypto_stream.h",
        "quic_flow_controller.h",
        "quic_session.h",
        "quic_stream.h",
        "quic_stream_sequencer.h",
        "tls_client_handshaker.h",
        "tls_handshaker.h",
        "tls_server_handshaker.h",
    ],
    visibility = [
        ":internal",
        # For //borg/monitoring/b3m:probes.
        "//borg/monitoring/b3m:__pkg__",
        # For grpc-over-quic
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_config_lib",
        ":quic_connection_lib",
        ":quic_constants_lib",
        ":quic_packet_creator_lib",
        ":quic_packets_lib",
        ":quic_server_id_lib",
        ":quic_stream_frame_data_producer_lib",
        ":quic_stream_send_buffer_lib",
        ":quic_stream_sequencer_buffer_lib",
        ":quic_types_lib",
        ":quic_utils_lib",
        ":session_notifier_interface_lib",
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/core/frames:quic_frames_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_mem_slice_span_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_ref_counted_lib",
        "//source/common/quic/platform/api:quic_singleton_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//third_party/openssl:crypto",
        "//third_party/openssl:ssl",
    ],
)

envoy_cc_library(
    name = "quic_spdy_session_lib",
    srcs = [
        "quic_headers_stream.cc",
        "quic_server_session_base.cc",
        "quic_spdy_server_stream_base.cc",
        "quic_spdy_session.cc",
        "quic_spdy_stream.cc",
    ],
    hdrs = [
        "quic_headers_stream.h",
        "quic_server_session_base.h",
        "quic_spdy_server_stream_base.h",
        "quic_spdy_session.h",
        "quic_spdy_stream.h",
    ],
    visibility = [
        ":internal",
        # For //borg/monitoring/b3m:probes.
        "//borg/monitoring/b3m:__pkg__",
        # For grpc-over-quic
        "//net/grpc/internal/src/core/quic:__pkg__",
    ],
    deps = [
        ":quic_connection_lib",
        ":quic_error_codes_lib",
        ":quic_header_list_lib",
        ":quic_packets_lib",
        ":quic_session_lib",
        ":quic_utils_lib",
        ":spdy_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/proto:portable_quic_core_proto",
        "//source/common/quic/http/decoder:quic_http_deframer_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
    ],
)

envoy_cc_library(
    name = "quic_stream_sequencer_buffer_lib",
    srcs = [
        "quic_stream_sequencer_buffer.cc",
    ],
    hdrs = [
        "quic_stream_sequencer_buffer.h",
    ],
    deps = [
        ":quic_packets_lib",
        "//base",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_stack_trace_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_library(
    name = "quic_config_lib",
    srcs = [
        "quic_config.cc",
    ],
    hdrs = [
        "quic_config.h",
    ],
    visibility = [
        ":internal",
        "//net/grpc/internal/src/core/quic:__pkg__",
        # For //video/streaming/testing/perf:vstress_client_test.
        "//video/streaming/testing/perf:__pkg__",
    ],
    deps = [
        ":quic_packets_lib",
        ":quic_socket_address_coder_lib",
        ":quic_time_lib",
        ":quic_utils_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_library(
    name = "quic_framer_lib",
    srcs = [
        "quic_framer.cc",
    ],
    hdrs = [
        "quic_framer.h",
    ],
    deps = [
        ":quic_data_lib",
        ":quic_packets_lib",
        ":quic_socket_address_coder_lib",
        ":quic_stream_frame_data_producer_lib",
        ":quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_aligned_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_endian_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_packet_creator_lib",
    srcs = [
        "quic_packet_creator.cc",
    ],
    hdrs = [
        "quic_packet_creator.h",
    ],
    deps = [
        ":quic_connection_close_delegate_interface_lib",
        ":quic_data_lib",
        ":quic_framer_lib",
        ":quic_packets_lib",
        ":quic_pending_retransmission_lib",
        ":quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_aligned_lib",
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_library(
    name = "quic_packet_generator_lib",
    srcs = [
        "quic_packet_generator.cc",
    ],
    hdrs = [
        "quic_packet_generator.h",
    ],
    deps = [
        ":quic_packet_creator_lib",
        ":quic_pending_retransmission_lib",
        ":quic_sent_packet_manager_lib",
        ":quic_types_lib",
        ":quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "quic_time_wait_list_manager_lib",
    srcs = ["quic_time_wait_list_manager.cc"],
    hdrs = ["quic_time_wait_list_manager.h"],
    deps = [
        ":quic_blocked_writer_interface_lib",
        ":quic_connection_lib",
        ":quic_framer_lib",
        ":quic_packet_writer_lib",
        ":quic_packets_lib",
        ":quic_session_lib",
        ":quic_utils_lib",
        "//base",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
    ],
)

envoy_cc_library(
    name = "quic_stream_frame_data_producer_lib",
    hdrs = [
        "quic_stream_frame_data_producer.h",
    ],
    deps = [
        ":quic_types_lib",
    ],
)

envoy_cc_library(
    name = "quic_stream_send_buffer_lib",
    srcs = [
        "quic_stream_send_buffer.cc",
    ],
    hdrs = [
        "quic_stream_send_buffer.h",
    ],
    deps = [
        ":quic_data_lib",
        ":quic_utils_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/frames:quic_frames_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_mem_slice_lib",
    ],
)

envoy_cc_library(
    name = "quic_server_lib",
    srcs = [
        "chlo_extractor.cc",
        "quic_buffered_packet_store.cc",
        "quic_dispatcher.cc",
        "quic_per_connection_packet_writer.cc",
        "stateless_rejector.cc",
    ],
    hdrs = [
        "chlo_extractor.h",
        "quic_buffered_packet_store.h",
        "quic_dispatcher.h",
        "quic_per_connection_packet_writer.h",
        "stateless_rejector.h",
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
        ":quic_alarm_factory_lib",
        ":quic_alarm_lib",
        ":quic_blocked_writer_interface_lib",
        ":quic_connection_lib",
        ":quic_framer_lib",
        ":quic_packet_reader_lib",
        ":quic_packet_writer_lib",
        ":quic_packets_lib",
        ":quic_session_lib",
        ":quic_time_lib",
        ":quic_time_wait_list_manager_lib",
        ":quic_utils_lib",
        ":quic_version_manager_lib",
        "//base",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_map_util_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_socket_address_lib",
        "//source/common/quic/platform/api:quic_stack_trace_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_client_lib",
    srcs = [
        "quic_client_promised_info.cc",
        "quic_client_push_promise_index.cc",
        "quic_spdy_client_session.cc",
        "quic_spdy_client_session_base.cc",
        "quic_spdy_client_stream.cc",
    ],
    hdrs = [
        "quic_client_promised_info.h",
        "quic_client_push_promise_index.h",
        "quic_spdy_client_session.h",
        "quic_spdy_client_session_base.h",
        "quic_spdy_client_stream.h",
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
        ":quic_alarm_lib",
        ":quic_packets_lib",
        ":quic_server_id_lib",
        ":quic_session_lib",
        ":quic_spdy_session_lib",
        ":quic_types_lib",
        ":spdy_utils_lib",
        "//base",
        "//gfe/gfe2/base:epoll_executor",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
    ],
)

envoy_cc_library(
    name = "packet_number_indexed_queue_lib",
    hdrs = ["packet_number_indexed_queue.h"],
    deps = [
        ":quic_types_lib",
        "//base",
        "//source/common/quic/platform/api:quic_containers_lib",
    ],
)

# This should list envoy_cc_library targets that wouldn't otherwise
# be depended upon by a cross-platform test.
build_test(
    name = "quic_core_pg3_build",
    tags = [
        # Include in cross-platform builds.
        "pg3",
        # Exclude from production platform builds which have test/binary coverage
        # of the build of these targets.
        "portable_test",
    ],
    targets = [
        ":quic_server_lib",
        ":quic_session_lib",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
