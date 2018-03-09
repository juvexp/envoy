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


envoy_cc_test_library(
    name = "quic_http_settings_lib",
    srcs = [
        "quic_http_settings.cc",
    ],
    hdrs = ["quic_http_settings.h"],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
    ],
)

################################################################################
# Tests and test support.

envoy_cc_test(
    name = "quic_http_settings_test",
    srcs = ["quic_http_settings_test.cc"],
    deps = [
        ":quic_http_settings_lib",
        "//base",
        "//test/common/quic/http/tools:quic_http_gunit_helpers_lib",
    ],
)
