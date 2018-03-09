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
    name = "quic_http_constants_lib",
    srcs = [
        "quic_http_constants.cc",
    ],
    hdrs = [
        "quic_http_constants.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_structures_lib",
    srcs = [
        "quic_http_structures.cc",
    ],
    hdrs = [
        "quic_http_structures.h",
    ],
    deps = [
        ":quic_http_constants_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

################################################################################
# Tests and test support.
