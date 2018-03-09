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


# Libraries

envoy_cc_test_library(
    name = "quic_http_frame_parts_lib",
    srcs = [
        "quic_http_frame_parts.cc",
    ],
    hdrs = [
        "quic_http_frame_parts.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/tools:quic_http_gunit_helpers_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_optional_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//strings",
    ],
)

envoy_cc_test_library(
    name = "quic_http_frame_parts_collector_lib",
    srcs = [
        "quic_http_frame_parts_collector.cc",
    ],
    hdrs = [
        "quic_http_frame_parts_collector.h",
    ],
    deps = [
        ":quic_http_frame_parts_lib",
        "//base",
        "//source/common/quic/http:quic_http_structures_lib",
        "//test/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//test/common/quic/http/decoder:quic_http_frame_decoder_listener_test_util_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_http_frame_parts_collector_listener_lib",
    srcs = [
        "quic_http_frame_parts_collector_listener.cc",
    ],
    hdrs = [
        "quic_http_frame_parts_collector_listener.h",
    ],
    deps = [
        ":quic_http_frame_parts_collector_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
    ],
)

# Tests

envoy_cc_test(
    name = "quic_http_frame_parts_test",
    size = "small",
    srcs = [
        "quic_http_frame_parts_test.cc",
    ],
    deps = [
        ":quic_http_frame_parts_lib",
        "//base",
        "//test/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//util/random/utils:test_random_seed_environment",
    ],
)
