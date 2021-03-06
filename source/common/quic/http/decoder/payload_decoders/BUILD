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
    name = "quic_http_altsvc_payload_decoder_lib",
    srcs = [
        "quic_http_altsvc_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_altsvc_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_continuation_payload_decoder_lib",
    srcs = [
        "quic_http_continuation_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_continuation_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_data_payload_decoder_lib",
    srcs = [
        "quic_http_data_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_data_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_goaway_payload_decoder_lib",
    srcs = [
        "quic_http_goaway_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_goaway_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_headers_payload_decoder_lib",
    srcs = [
        "quic_http_headers_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_headers_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_ping_payload_decoder_lib",
    srcs = [
        "quic_http_ping_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_ping_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_priority_payload_decoder_lib",
    srcs = [
        "quic_http_priority_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_priority_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_push_promise_payload_decoder_lib",
    srcs = [
        "quic_http_push_promise_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_push_promise_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_fallthrough_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_rst_stream_payload_decoder_lib",
    srcs = [
        "quic_http_rst_stream_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_rst_stream_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_settings_payload_decoder_lib",
    srcs = [
        "quic_http_settings_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_settings_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_unknown_payload_decoder_lib",
    srcs = [
        "quic_http_unknown_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_unknown_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_window_update_payload_decoder_lib",
    srcs = [
        "quic_http_window_update_payload_decoder.cc",
    ],
    hdrs = [
        "quic_http_window_update_payload_decoder.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_decode_structures_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

################################################################################
# Tests and test support.
