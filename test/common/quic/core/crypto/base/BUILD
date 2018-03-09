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
    name = "hkdf_test",
    srcs = ["hkdf_test.cc"],
    deps = [
        "//source/common/quic/core/crypto/base:hkdf",
        "//base",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//strings",
    ],
)

envoy_cc_test(
    name = "hmac_test",
    size = "small",
    srcs = ["hmac_test.cc"],
    deps = [
        "//source/common/quic/core/crypto/base:hmac",
        "//base",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//strings",
    ],
)
