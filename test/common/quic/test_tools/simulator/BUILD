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
    name = "simulator_lib",
    srcs = [
        "actor.cc",
        "alarm_factory.cc",
        "link.cc",
        "packet_filter.cc",
        "port.cc",
        "queue.cc",
        "simulator.cc",
        "switch.cc",
        "traffic_policer.cc",
    ],
    hdrs = [
        "actor.h",
        "alarm_factory.h",
        "link.h",
        "packet_filter.h",
        "port.h",
        "queue.h",
        "simulator.h",
        "switch.h",
        "traffic_policer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_alarm_factory_lib",
        "//source/common/quic/core:quic_alarm_lib",
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/core:quic_time_lib",
        "//source/common/quic/core/crypto:quic_random_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_clock_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
    ],
)

envoy_cc_test(
    name = "simulator_test",
    srcs = ["simulator_test.cc"],
    deps = [
        ":simulator_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)

envoy_cc_test_library(
    name = "quic_endpoint_lib",
    srcs = [
        "quic_endpoint.cc",
    ],
    hdrs = [
        "quic_endpoint.h",
    ],
    deps = [
        ":simulator_lib",
        "//base",
        "//source/common/quic/core:quic_connection_lib",
        "//source/common/quic/core:quic_data_lib",
        "//source/common/quic/core:quic_packet_writer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core:quic_stream_frame_data_producer_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/core/crypto:quic_encryption_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//test/common/quic/test_tools:quic_test_utils",
        "//test/common/quic/test_tools:simple_session_notifier_lib",
    ],
)

envoy_cc_test(
    name = "quic_endpoint_test",
    srcs = ["quic_endpoint_test.cc"],
    deps = [
        ":quic_endpoint_lib",
        ":simulator_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)
