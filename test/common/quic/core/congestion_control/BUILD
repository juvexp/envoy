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
    name = "windowed_filter_test",
    srcs = [
        "windowed_filter_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:rtt_stats_lib",
        "//source/common/quic/core/congestion_control:windowed_filter_lib",
        "//source/common/quic/core:quic_bandwidth_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "rtt_stats_test",
    srcs = [
        "rtt_stats_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:rtt_stats_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:rtt_stats_peer",
    ],
)

envoy_cc_test(
    name = "pacing_sender_test",
    srcs = [
        "pacing_sender_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:pacing_sender_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_clock",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test(
    name = "cubic_bytes_test",
    srcs = [
        "cubic_bytes_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:tcp_cubic_bytes_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_clock",
    ],
)

envoy_cc_test(
    name = "hybrid_slow_start_test",
    srcs = [
        "hybrid_slow_start_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:tcp_cubic_helper",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "prr_sender_test",
    srcs = [
        "prr_sender_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:tcp_cubic_helper",
        "//source/common/quic/core:quic_constants_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//test/common/quic/platform/api:quic_test_lib",
    ],
)

envoy_cc_test(
    name = "tcp_cubic_sender_bytes_test",
    srcs = [
        "tcp_cubic_sender_bytes_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:quic_congestion_control_lib",
        "//source/common/quic/core/congestion_control:rtt_stats_lib",
        "//source/common/quic/core/congestion_control:tcp_cubic_bytes_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_clock",
        "//test/common/quic/test_tools:quic_config_peer",
    ],
)

envoy_cc_test(
    name = "general_loss_algorithm_test",
    srcs = [
        "general_loss_algorithm_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:general_loss_algorithm_lib",
        "//source/common/quic/core/congestion_control:rtt_stats_lib",
        "//source/common/quic/core:quic_unacked_packet_map_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_clock",
    ],
)

envoy_cc_test(
    name = "bandwidth_sampler_test",
    srcs = ["bandwidth_sampler_test.cc"],
    deps = [
        "//source/common/quic/core/congestion_control:bandwidth_sampler_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_clock",
    ],
)

envoy_cc_test(
    name = "bbr_sender_test",
    srcs = [
        "bbr_sender_test.cc",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:bbr_lib",
        "//source/common/quic/core/congestion_control:rtt_stats_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_clock",
        "//test/common/quic/test_tools:quic_config_peer",
        "//test/common/quic/test_tools:quic_sent_packet_manager_peer",
        "//test/common/quic/test_tools:quic_test_utils",
        "//test/common/quic/test_tools/simulator:quic_endpoint_lib",
        "//test/common/quic/test_tools/simulator:simulator_lib",
    ],
)

envoy_cc_test(
    name = "send_algorithm_test",
    srcs = [
        "send_algorithm_test.cc",
    ],
    shard_count = 6,
    deps = [
        "//source/common/quic/core/congestion_control:quic_congestion_control_hdrs",
        "//source/common/quic/core/congestion_control:quic_congestion_control_lib",
        "//source/common/quic/core/congestion_control:rtt_stats_lib",
        "//source/common/quic/core:quic_types_lib",
        "//source/common/quic/core:quic_utils_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:mock_clock",
        "//test/common/quic/test_tools:quic_config_peer",
        "//test/common/quic/test_tools:quic_sent_packet_manager_peer",
        "//test/common/quic/test_tools:quic_test_utils",
        "//test/common/quic/test_tools/simulator:quic_endpoint_lib",
        "//test/common/quic/test_tools/simulator:simulator_lib",
    ],
)
