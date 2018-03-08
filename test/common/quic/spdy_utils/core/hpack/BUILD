# Description:
#   Implementation of HTTP/2's HPACK compression (RFC7541).

package(default_visibility = [":internal"])

package_group(
    name = "internal",
    packages = [
        "//gfe/quic/http/...",
        "//gfe/spdy/...",
    ],
)

package_group(
    name = "gfe2",
    packages = [
        "//gfe/gfe2/spdy/...",
        "//gfe/gfe2/test_tools/fuzzing/http2/...",
    ],
)

envoy_cc_test(
    name = "hpack_decoder_adapter_test",
    srcs = ["hpack_decoder_adapter_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core/hpack:hpack_decoder_adapter_lib",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//base",
        "//gfe/http2/hpack/decoder:hpack_decoder_state_lib",
        "//gfe/http2/hpack/decoder:hpack_decoder_tables_lib",
        "//gfe/http2/hpack/tools:hpack_block_builder_lib",
        "//test/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test(
    name = "hpack_encoder_test",
    srcs = ["hpack_encoder_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//base",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test(
    name = "hpack_entry_test",
    srcs = ["hpack_entry_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//base",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "hpack_header_table_test",
    srcs = ["hpack_header_table_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//base",
        "//gfe/gfe2/spdy:hpack_policy_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "hpack_huffman_table_test",
    srcs = ["hpack_huffman_table_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//base",
        "//gfe/http2/hpack/huffman:hpack_huffman_decoder_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "hpack_output_stream_test",
    srcs = ["hpack_output_stream_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//base",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "hpack_round_trip_test",
    srcs = ["hpack_round_trip_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core/hpack:hpack_decoder_adapter_lib",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//base",
        "//test/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
    ],
)

envoy_cc_test(
    name = "hpack_static_table_test",
    srcs = ["hpack_static_table_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//testing/base/public:gunit_main",
    ],
)
