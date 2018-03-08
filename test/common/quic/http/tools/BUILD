# Description: Tools in support of http2 decoder/encoder.
# Please put rules in lexical order by name within sections.

package(default_visibility = ["//gfe/quic/http:gfe_quic"])

load("//gfe/gfe2/tools:build_defs.bzl", "cc_test_library")

################################################################################
# Libraries.

################################################################################
# Tests of the above libraries and test support.

envoy_cc_test_library(
    name = "quic_http_frame_formatter_lib",
    srcs = [
        "quic_http_frame_formatter.cc",
    ],
    hdrs = ["quic_http_frame_formatter.h"],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_structures_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//strings",
        "//third_party/absl/strings",
        "//util/regexp/re2",
        "//util/task:status",
        "//util/task:statusor",
    ],
)

envoy_cc_test(
    name = "quic_http_frame_formatter_test",
    srcs = [
        "quic_http_frame_formatter_test.cc",
    ],
    deps = [
        ":quic_http_frame_formatter_lib",
        "//base",
        "//testing/base/public:gunit_main",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_gunit_helpers_lib",
    hdrs = ["quic_http_gunit_helpers.h"],
    deps = [
        "//base",
        "//gfe/gfe2/test_tools:failure_lib",
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test_library(
    name = "quic_http_frame_builder_lib",
    srcs = [
        "quic_http_frame_builder.cc",
    ],
    hdrs = ["quic_http_frame_builder.h"],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test(
    name = "quic_http_frame_builder_test",
    srcs = [
        "quic_http_frame_builder_test.cc",
    ],
    deps = [
        ":quic_http_frame_builder_lib",
        "//base",
        "//testing/base/public:gunit_main",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_message_generator_lib",
    srcs = [
        "quic_http_message_generator.cc",
    ],
    hdrs = [
        "quic_http_message_generator.h",
    ],
    deps = [
        ":quic_http_message_interface_lib",
        ":quic_http_random_util_lib",
        "//base",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
        "//strings",
    ],
)

envoy_cc_test_library(
    name = "quic_http_message_interface_lib",
    srcs = [
        "quic_http_message_interface.cc",
    ],
    hdrs = ["quic_http_message_interface.h"],
    deps = [
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

cc_test_library(
    name = "quic_http_random_decoder_test_lib",
    srcs = [
        "quic_http_random_decoder_test.cc",
    ],
    hdrs = ["quic_http_random_decoder_test.h"],
    deps = [
        ":quic_http_gunit_helpers_lib",
        "//base",
        "//gfe/gfe2/test_tools:failure_lib",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
        "//testing/base/public:gunit_for_library",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test(
    name = "quic_http_random_decoder_test_test",
    srcs = [
        "quic_http_random_decoder_test_test.cc",
    ],
    deps = [
        ":quic_http_random_decoder_test_lib",
        "//base",
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_random_util_lib",
    srcs = [
        "quic_http_random_util.cc",
    ],
    hdrs = [
        "quic_http_random_util.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
        "//util/random:random_no_openssl",
    ],
)

envoy_cc_test(
    name = "quic_http_random_util_test",
    srcs = [
        "quic_http_random_util_test.cc",
    ],
    deps = [
        ":quic_http_random_util_lib",
        "//base",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_stream_generator_lib",
    srcs = [
        "quic_http_stream_generator.cc",
    ],
    hdrs = [
        "quic_http_stream_generator.h",
    ],
    deps = [
        ":quic_http_message_interface_lib",
        ":quic_http_random_util_lib",
        ":quic_http_stream_id_generator_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_header_block_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
    ],
)

envoy_cc_test(
    name = "quic_http_stream_generator_test",
    srcs = [
        "quic_http_stream_generator_test.cc",
    ],
    deps = [
        ":quic_http_frame_formatter_lib",
        ":quic_http_message_generator_lib",
        ":quic_http_message_interface_lib",
        ":quic_http_stream_generator_lib",
        "//base",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_stream_id_generator_lib",
    srcs = [
        "quic_http_stream_id_generator.cc",
    ],
    hdrs = [
        "quic_http_stream_id_generator.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_test_random_lib",
    ],
)

################################################################################
# Binaries (e.g. cmdline tools).
