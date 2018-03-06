# Experimental HTTP/2 decoder.
# Please put rules in lexical order by name within sections.

package(default_visibility = ["//source/common/quic/http/decoder:internal"])

package_group(
    name = "internal",
    packages = [
        # Temporarily allow gfe2 and spdy access to all targets.
        # TODO: Figure out which targets to expose.
        "//source/common/quic/...",
    ],
)

################################################################################
# Libraries (i.e. production code).

################################################################################
# Tests and test support.

envoy_cc_test(
    name = "quic_http_decode_buffer_test",
    size = "small",
    srcs = [
        "quic_http_decode_buffer_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//base",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test(
    name = "quic_http_decode_structures_test",
    size = "small",
    srcs = [
        "quic_http_decode_structures_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_decode_structures_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/tools:quic_http_frame_builder_lib",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_frame_decoder_state_test_util_lib",
    testonly = 1,
    srcs = [
        "quic_http_frame_decoder_state_test_util.cc",
    ],
    hdrs = [
        "quic_http_frame_decoder_state_test_util.h",
    ],
    deps = [
        "//source/common/quic/http/decoder:quic_http_frame_decoder_state_lib",
        "//source/common/quic/http/decoder:quic_http_structure_decoder_test_util_lib",
        "//base",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_http_frame_decoder_listener_test_util_lib",
    testonly = 1,
    srcs = [
        "quic_http_frame_decoder_listener_test_util.cc",
    ],
    hdrs = [
        "quic_http_frame_decoder_listener_test_util.h",
    ],
    deps = [
        "//source/common/quic/http/decoder:quic_http_frame_decoder_listener_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//testing/base/public:gunit_for_library",
    ],
)

envoy_cc_test(
    name = "quic_http_frame_decoder_test",
    size = "small",
    srcs = [
        "quic_http_frame_decoder_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder:quic_http_frame_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http/test_tools:quic_http_frame_parts_collector_listener_lib",
        "//source/common/quic/http/test_tools:quic_http_frame_parts_lib",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_reconstruct_object_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//testing/base/public:gunit_main",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test(
    name = "quic_http_structure_decoder_remaining_payload_test",
    size = "small",
    srcs = [
        "quic_http_structure_decoder_remaining_payload_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_structure_decoder_lib",
        "//source/common/quic/http/decoder:quic_http_structure_decoder_test_util_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_reconstruct_object_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//testing/base/public:gunit_main",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test(
    name = "quic_http_structure_decoder_test",
    size = "small",
    srcs = [
        "quic_http_structure_decoder_test.cc",
    ],
    deps = [
        "//source/common/quic/http/decoder:quic_http_decode_buffer_lib",
        "//source/common/quic/http/decoder:quic_http_decode_status_lib",
        "//source/common/quic/http/decoder:quic_http_structure_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_test_util_lib",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_reconstruct_object_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//testing/base/public:gunit_main",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_structure_decoder_test_util_lib",
    testonly = 1,
    srcs = [
        "quic_http_structure_decoder_test_util.cc",
    ],
    hdrs = [
        "quic_http_structure_decoder_test_util.h",
    ],
    deps = [
        "//source/common/quic/http/decoder:quic_http_structure_decoder_lib",
        "//base",
        "//source/common/quic/http/tools:quic_http_random_decoder_test_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
    ],
)
