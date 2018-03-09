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
    name = "quic_frames_test",
    srcs = ["quic_frames_test.cc"],
    deps = [
        "//source/common/quic/core/frames:quic_frames_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//test/common/quic/test_tools:quic_test_utils",
    ],
)
