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


################################################################################
# Tests and test support.

envoy_cc_test(
    name = "quic_http_altsvc_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_altsvc_payload_decoder_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder/payload_decoders:quic_http_altsvc_payload_decoder_lib",
        ":quic_http_payload_decoder_base_test_util_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_continuation_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_continuation_payload_decoder_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder/payload_decoders:quic_http_continuation_payload_decoder_lib",
        ":quic_http_payload_decoder_base_test_util_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_data_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_data_payload_decoder_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder/payload_decoders:quic_http_data_payload_decoder_lib",
        ":quic_http_payload_decoder_base_test_util_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_goaway_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_goaway_payload_decoder_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder/payload_decoders:quic_http_goaway_payload_decoder_lib",
        ":quic_http_payload_decoder_base_test_util_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_headers_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_headers_payload_decoder_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder/payload_decoders:quic_http_headers_payload_decoder_lib",
        ":quic_http_payload_decoder_base_test_util_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

cc_test_library(
    name = "quic_http_payload_decoder_base_test_util_lib",
    srcs = [
        "quic_http_payload_decoder_base_test_util.cc",
    ],
    hdrs = [
        "quic_http_payload_decoder_base_test_util.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_constants_test_util_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//test/common/quic/http/decoder:quic_http_frame_decoder_state_test_util_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_reconstruct_object_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//util/random/utils:test_random_seed_environment",  # fixdeps: keep
    ],
)

envoy_cc_test(
    name = "quic_http_ping_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_ping_payload_decoder_test.cc",
    ],
    deps = [
        ":quic_http_payload_decoder_base_test_util_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_ping_payload_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_priority_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_priority_payload_decoder_test.cc",
    ],
    deps = [
        ":quic_http_payload_decoder_base_test_util_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_priority_payload_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_push_promise_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_push_promise_payload_decoder_test.cc",
    ],
    deps = [
        ":quic_http_payload_decoder_base_test_util_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_push_promise_payload_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_rst_stream_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_rst_stream_payload_decoder_test.cc",
    ],
    deps = [
        ":quic_http_payload_decoder_base_test_util_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_rst_stream_payload_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_constants_test_util_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_settings_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_settings_payload_decoder_test.cc",
    ],
    deps = [
        ":quic_http_payload_decoder_base_test_util_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_settings_payload_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_constants_test_util_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_unknown_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_unknown_payload_decoder_test.cc",
    ],
    deps = [
        ":quic_http_payload_decoder_base_test_util_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_unknown_payload_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_string_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_window_update_payload_decoder_test",
    size = "small",
    srcs = [
        "quic_http_window_update_payload_decoder_test.cc",
    ],
    deps = [
        ":quic_http_payload_decoder_base_test_util_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_window_update_payload_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//test/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
    ],
)
