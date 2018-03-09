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
    name = "hpack_fuzz_util_test",
    srcs = ["hpack_fuzz_util_test.cc"],
    data = ["//source/common/quic/spdy_utils/core/fuzzing/testdata:test_files"],
    deps = [
        "//source/common/quic/spdy_utils/core/fuzzing:hpack_fuzz_util",
        "//file/base",
        "//file/localfile",
        "//test/common/quic/spdy_utils/core:spdy_test_utils_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//util/random:random_no_openssl",
    ],
)
