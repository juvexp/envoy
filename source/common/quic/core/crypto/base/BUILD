# Classes in this directory implement a common cryptographic API between
# Chromium and the GFE so that QUIC crypto code can be written once.
#
# Primarily this consists of reimplementing existing Chromium crypto/ APIs in
# google3.

package(default_visibility = [":quic_crypto"])

package_group(
    name = "quic_crypto",
    packages = [
        "//source/common/quic/core/crypto",
    ],
)

envoy_cc_library(
    name = "hkdf",
    srcs = ["hkdf.cc"],
    hdrs = ["hkdf.h"],
    visibility = ["//source/common/quic/core:internal"],
    deps = [
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//third_party/openssl:crypto",
    ],
)

envoy_cc_library(
    name = "hmac",
    srcs = ["hmac.cc"],
    hdrs = ["hmac.h"],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_logging_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//third_party/openssl:crypto",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
