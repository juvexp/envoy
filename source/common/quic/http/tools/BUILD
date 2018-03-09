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


################################################################################
# Libraries.

################################################################################
# Tests of the above libraries and test support.

################################################################################
# Binaries (e.g. cmdline tools).

envoy_cc_binary(
    name = "quic_http_frame_formatter",
    testonly = 1,
    srcs = ["quic_http_frame_formatter_main.cc"],
    deps = [
        ":quic_http_frame_formatter_lib",
        "//base",
        "//file/base",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//util/task:status",
        "//util/task:statusor",
    ],
)
