# Description:
#   Fuzzing utilities, binaries, and tests HPACK.

envoy_cc_library(
    name = "hpack_fuzz_util",
    srcs = ["hpack_fuzz_util.cc"],
    hdrs = ["hpack_fuzz_util.h"],
    deps = [
        "//base",
        "//source/common/quic/spdy_utils/core/hpack:hpack_decoder_adapter_lib",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//util/random:random_no_openssl",
    ],
)

envoy_cc_binary(
    name = "hpack_example_generator",
    srcs = ["hpack_example_generator.cc"],
    deps = [
        ":hpack_fuzz_util",
        "//base",
        "//file/base",
        "//file/localfile",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//util/random:random_no_openssl",
        "//util/task:status",
    ],
)
