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


filegroup(
    name = "test_files",
    srcs = ["examples.hpack"],
)
