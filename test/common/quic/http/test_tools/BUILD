# Test-only utilities and their tests.

package(default_visibility = ["//source/common/quic/http:gfe_quic"])

# Libraries

envoy_cc_test_library(
    name = "quic_http_frame_parts_lib",
    testonly = 1,
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
        "//source/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/tools:quic_http_gunit_helpers_lib",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_optional_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//strings",
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test_library(
    name = "quic_http_frame_parts_collector_lib",
    testonly = 1,
    srcs = [
        "quic_http_frame_parts_collector.cc",
    ],
    hdrs = [
        "quic_http_frame_parts_collector.h",
    ],
    deps = [
        "//source/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//base",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_test_util_lib",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test_library(
    name = "quic_http_frame_parts_collector_listener_lib",
    testonly = 1,
    srcs = [
        "quic_http_frame_parts_collector_listener.cc",
    ],
    hdrs = [
        "quic_http_frame_parts_collector_listener.h",
    ],
    deps = [
        "//source/common/quic/http/test_tools:quic_http_frame_parts_collector_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//testing/base/public:gunit_for_library",
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
        "//source/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//base",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//testing/base/public:gunit_main",
        "//util/random/utils:test_random_seed_environment",
    ],
)
