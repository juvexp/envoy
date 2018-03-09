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
    name = "windowed_filter_lib",
    hdrs = [
        "windowed_filter.h",
    ],
    deps = ["//source/common/quic/core:quic_time_lib"],
)

envoy_cc_library(
    name = "bandwidth_sampler_lib",
    srcs = [
        "bandwidth_sampler.cc",
    ],
    hdrs = [
        "bandwidth_sampler.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:packet_number_indexed_queue_lib",
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
    ],
)

envoy_cc_library(
    name = "pacing_sender_lib",
    srcs = [
        "pacing_sender.cc",
    ],
    hdrs = [
        "pacing_sender.h",
    ],
    deps = [
        ":quic_congestion_control_hdrs",
        "//base",
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "tcp_cubic_helper",
    srcs = [
        "hybrid_slow_start.cc",
        "prr_sender.cc",
    ],
    hdrs = [
        "hybrid_slow_start.h",
        "prr_sender.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "tcp_cubic_bytes_lib",
    srcs = [
        "cubic_bytes.cc",
        "tcp_cubic_sender_bytes.cc",
    ],
    hdrs = [
        "cubic_bytes.h",
        "tcp_cubic_sender_bytes.h",
    ],
    deps = [
        ":quic_congestion_control_hdrs",
        ":rtt_stats_lib",
        ":tcp_cubic_helper",
        "//base",
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_connection_stats_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "rtt_stats_lib",
    srcs = [
        "rtt_stats.cc",
    ],
    hdrs = [
        "rtt_stats.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "general_loss_algorithm_lib",
    srcs = [
        "general_loss_algorithm.cc",
    ],
    hdrs = [
        "general_loss_algorithm.h",
    ],
    deps = [
        ":quic_congestion_control_hdrs",
        ":rtt_stats_lib",
        "//base",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core:quic_unacked_packet_map_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
    ],
)

envoy_cc_library(
    name = "quic_congestion_control_hdrs",
    hdrs = [
        "loss_detection_interface.h",
        "send_algorithm_interface.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_connection_stats_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core:quic_types_lib",
        "//source/common/quic/core:quic_unacked_packet_map_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "quic_congestion_control_lib",
    srcs = [
        "send_algorithm_interface.cc",
    ],
    hdrs = [
        "loss_detection_interface.h",
        "send_algorithm_interface.h",
    ],
    deps = [
        ":bbr_lib",
        ":quic_congestion_control_hdrs",
        ":tcp_cubic_bytes_lib",
        "//base",
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_config_lib",
        "//source/common/quic/core:quic_connection_stats_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core:quic_types_lib",
        "//source/common/quic/core:quic_unacked_packet_map_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_pcc_sender_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_library(
    name = "bbr_lib",
    srcs = [
        "bbr_sender.cc",
    ],
    hdrs = [
        "bbr_sender.h",
    ],
    deps = [
        ":bandwidth_sampler_lib",
        ":quic_congestion_control_hdrs",
        ":rtt_stats_lib",
        ":windowed_filter_lib",
        "//base",
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core:quic_unacked_packet_map_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
