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
    name = "quic_frames_lib",
    srcs = [
        "quic_ack_frame.cc",
        "quic_blocked_frame.cc",
        "quic_connection_close_frame.cc",
        "quic_frame.cc",
        "quic_goaway_frame.cc",
        "quic_ietf_blocked_frame.cc",
        "quic_ietf_max_stream_id_frame.cc",
        "quic_ietf_stream_id_blocked_frame.cc",
        "quic_padding_frame.cc",
        "quic_path_challenge_frame.cc",
        "quic_path_response_frame.cc",
        "quic_ping_frame.cc",
        "quic_rst_stream_frame.cc",
        "quic_stop_sending_frame.cc",
        "quic_stop_waiting_frame.cc",
        "quic_stream_frame.cc",
        "quic_window_update_frame.cc",
    ],
    hdrs = [
        "quic_ack_frame.h",
        "quic_blocked_frame.h",
        "quic_connection_close_frame.h",
        "quic_control_frame.h",
        "quic_frame.h",
        "quic_goaway_frame.h",
        "quic_ietf_blocked_frame.h",
        "quic_ietf_max_stream_id_frame.h",
        "quic_ietf_stream_id_blocked_frame.h",
        "quic_mtu_discovery_frame.h",
        "quic_padding_frame.h",
        "quic_path_challenge_frame.h",
        "quic_path_response_frame.h",
        "quic_ping_frame.h",
        "quic_rst_stream_frame.h",
        "quic_stop_sending_frame.h",
        "quic_stop_waiting_frame.h",
        "quic_stream_frame.h",
        "quic_window_update_frame.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/core:quic_buffer_allocator_lib",
        "//source/common/quic/core:quic_constants_lib",
        "//source/common/quic/core:quic_error_codes_lib",
        "//source/common/quic/core:quic_types_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_containers_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_flag_utils_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
