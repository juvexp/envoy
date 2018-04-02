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
    name = "spdy_alt_svc_wire_format_test",
    srcs = [
        "spdy_alt_svc_wire_format_test.cc",
    ],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_alt_svc_wire_format_lib",
        "//base",
    ],
)

envoy_cc_test_library(
    name = "spdy_no_op_visitor_lib",
    srcs = ["spdy_no_op_visitor.cc"],
    hdrs = ["spdy_no_op_visitor.h"],
    visibility = [
        "//gfe/gfe2:gfe2_internal",
        "//gfe/http2/tools:__pkg__",
        "//gfe/quic:__subpackages__",
        "//gfe/spdy:__subpackages__",
    ],
    deps = [
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
    ],
)

envoy_cc_test(
    name = "spdy_pinnable_buffer_piece_test",
    srcs = ["spdy_pinnable_buffer_piece_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_pinnable_buffer_piece",
        "//source/common/quic/spdy_utils/core:spdy_prefixed_buffer_reader",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
    ],
)

envoy_cc_test(
    name = "spdy_prefixed_buffer_reader_test",
    srcs = ["spdy_prefixed_buffer_reader_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_prefixed_buffer_reader",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
    ],
)

envoy_cc_test_library(
    name = "spdy_test_utils_lib",
    srcs = ["spdy_test_utils.cc"],
    hdrs = ["spdy_test_utils.h"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_bug_tracker",
        "//source/common/quic/spdy_utils/core:spdy_header_block_lib",
        "//source/common/quic/spdy_utils/core:spdy_headers_handler_interface",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//base",
        "//gfe/gfe2/test_tools:failure_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//strings",
    ],
)

envoy_cc_test_library(
    name = "spdy_deframer_visitor_lib",
    srcs = ["spdy_deframer_visitor.cc"],
    hdrs = ["spdy_deframer_visitor.h"],
    deps = [
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        ":mock_spdy_framer_visitor",
        "//source/common/quic/spdy_utils/core:spdy_frame_reader_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        ":spdy_protocol_test_utils_lib",
        ":spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_flags_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//strings:cord",
    ],
)

envoy_cc_test_library(
    name = "spdy_protocol_test_utils_lib",
    srcs = ["spdy_protocol_test_utils.cc"],
    hdrs = ["spdy_protocol_test_utils.h"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        ":spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
    ],
)

envoy_cc_test_library(
    name = "mock_spdy_framer_visitor",
    srcs = [
        "mock_spdy_framer_visitor.cc",
    ],
    hdrs = [
        "mock_spdy_framer_visitor.h",
    ],
    deps = [
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        ":spdy_test_utils_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
    ],
)

envoy_cc_test(
    name = "spdy_deframer_visitor_test",
    srcs = ["spdy_deframer_visitor_test.cc"],
    deps = [
        ":mock_spdy_framer_visitor",
        ":spdy_deframer_visitor_lib",
        "//source/common/quic/spdy_utils/core:spdy_frame_reader_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        ":spdy_protocol_test_utils_lib",
        ":spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//strings:cord",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test(
    name = "spdy_framer_test",
    size = "medium",
    srcs = ["spdy_framer_test.cc"],
    shard_count = 5,
    deps = [
        "//source/common/quic/spdy_utils/core:array_output_buffer_lib",
        ":mock_spdy_framer_visitor",
        "//source/common/quic/spdy_utils/core:spdy_frame_reader_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        ":spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_flags_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//strings:cord",
    ],
)

envoy_cc_test(
    name = "spdy_frame_builder_test",
    srcs = ["spdy_frame_builder_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:array_output_buffer_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
    ],
)

envoy_cc_test(
    name = "spdy_frame_reader_test",
    srcs = ["spdy_frame_reader_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_frame_reader_lib",
        "//base",
    ],
)

envoy_cc_test(
    name = "spdy_header_block_test",
    srcs = ["spdy_header_block_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_header_block_lib",
        ":spdy_test_utils_lib",
    ],
)

envoy_cc_test(
    name = "array_output_buffer_test",
    srcs = ["array_output_buffer_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:array_output_buffer_lib",
    ],
)

envoy_cc_test(
    name = "spdy_protocol_test",
    srcs = ["spdy_protocol_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        ":spdy_test_utils_lib",
        "//base",
    ],
)

envoy_cc_test(
    name = "priority_write_scheduler_test",
    srcs = ["priority_write_scheduler_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:priority_write_scheduler_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        ":spdy_test_utils_lib",
    ],
)
