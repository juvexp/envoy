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
