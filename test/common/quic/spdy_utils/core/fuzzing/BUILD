# Description:
#   Fuzzing utilities, binaries, and tests HPACK.

envoy_cc_test(
    name = "hpack_fuzz_util_test",
    srcs = ["hpack_fuzz_util_test.cc"],
    data = ["//source/common/quic/spdy_utils/core/fuzzing/testdata:test_files"],
    deps = [
        "//source/common/quic/spdy_utils/core/fuzzing:hpack_fuzz_util",
        "//file/base",
        "//file/localfile",
        "//test/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//testing/base/public:gunit_main",
        "//util/random:random_no_openssl",
    ],
)
