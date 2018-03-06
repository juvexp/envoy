# Copyright 2009 Google Inc. All Rights Reserved.
# Author: fenix@google.com (Roberto Peon)
#
# Package:
#  gfe/spdy/core
#
# Description
#   This is the reference implementation of the HTTP/2 protocol (its framing,
#   to be more accurate). It is replicated in Chromium source tree and in that
#   sense is shared between Google3 and Chromium. It is intended to go under
#   net/spdy in the near future, but for now has to live in the gfe tree.

package(default_visibility = [
    "//gfe/gfe2:gfe2_internal",
    "//source/common/quic:__subpackages__",
    "//source/common/quic/spdy_utils:__subpackages__",
])

envoy_cc_test(
    name = "spdy_alt_svc_wire_format_test",
    srcs = [
        "spdy_alt_svc_wire_format_test.cc",
    ],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_alt_svc_wire_format_lib",
        "//base",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test_library(
    name = "spdy_no_op_visitor_lib",
    testonly = 1,
    srcs = ["spdy_no_op_visitor.cc"],
    hdrs = ["spdy_no_op_visitor.h"],
    visibility = [
        "//gfe/gfe2:gfe2_internal",
        "//gfe/http2/tools:__pkg__",
        "//source/common/quic:__subpackages__",
        "//source/common/quic/spdy_utils:__subpackages__",
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
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "spdy_prefixed_buffer_reader_test",
    srcs = ["spdy_prefixed_buffer_reader_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_prefixed_buffer_reader",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test_library(
    name = "spdy_test_utils_lib",
    testonly = 1,
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
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test_library(
    name = "spdy_deframer_visitor_lib",
    testonly = 1,
    srcs = ["spdy_deframer_visitor.cc"],
    hdrs = ["spdy_deframer_visitor.h"],
    deps = [
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        "//source/common/quic/spdy_utils/core:mock_spdy_framer_visitor",
        "//source/common/quic/spdy_utils/core:spdy_frame_reader_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_test_utils_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_flags_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//strings:cord",
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test_library(
    name = "spdy_protocol_test_utils_lib",
    testonly = 1,
    srcs = ["spdy_protocol_test_utils.cc"],
    hdrs = ["spdy_protocol_test_utils.h"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test_library(
    name = "mock_spdy_framer_visitor",
    testonly = 1,
    srcs = [
        "mock_spdy_framer_visitor.cc",
    ],
    hdrs = [
        "mock_spdy_framer_visitor.h",
    ],
    deps = [
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test(
    name = "spdy_deframer_visitor_test",
    srcs = ["spdy_deframer_visitor_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:mock_spdy_framer_visitor",
        "//source/common/quic/spdy_utils/core:spdy_deframer_visitor_lib",
        "//source/common/quic/spdy_utils/core:spdy_frame_reader_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_test_utils_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//strings:cord",
        "//testing/base/public:gunit_main",
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
        "//source/common/quic/spdy_utils/core:mock_spdy_framer_visitor",
        "//source/common/quic/spdy_utils/core:spdy_frame_reader_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_flags_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//strings:cord",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "spdy_frame_builder_test",
    srcs = ["spdy_frame_builder_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:array_output_buffer_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "spdy_frame_reader_test",
    srcs = ["spdy_frame_reader_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_frame_reader_lib",
        "//base",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "spdy_header_block_test",
    srcs = ["spdy_header_block_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_header_block_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "array_output_buffer_test",
    srcs = ["array_output_buffer_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:array_output_buffer_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "spdy_protocol_test",
    srcs = ["spdy_protocol_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_flags_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "priority_write_scheduler_test",
    srcs = ["priority_write_scheduler_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core:priority_write_scheduler_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//testing/base/public:gunit_main",
    ],
)
