# Classes in this directory implement a common cryptographic API between
# Chromium and the GFE so that QUIC crypto code can be written once.
#
# Primarily this consists of reimplementing existing Chromium crypto/ APIs in
# google3.

package(default_visibility = [":quic_crypto"])

package_group(
    name = "quic_crypto",
    packages = [
        "//gfe/quic/core/crypto",
    ],
)

envoy_cc_test(
    name = "hkdf_test",
    srcs = ["hkdf_test.cc"],
    deps = [
        "//source/common/quic/core/crypto/base:hkdf",
        "//base",
        "//source/common/quic/platform/api:quic_string_lib",
        "//test/common/quic/platform/api:quic_test_lib",
        "//strings",
        "//testing/base/public:gunit_main",
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
        "//testing/base/public:gunit_main",
        "//third_party/absl/strings",
    ],
)
