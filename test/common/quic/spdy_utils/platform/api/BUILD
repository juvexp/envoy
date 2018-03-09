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
    name = "spdy_mem_slice_test",
    srcs = ["spdy_mem_slice_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/platform/api:spdy_mem_slice_lib",
    ],
)

envoy_cc_test(
    name = "spdy_string_utils_test",
    srcs = ["spdy_string_utils_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
    ],
)
