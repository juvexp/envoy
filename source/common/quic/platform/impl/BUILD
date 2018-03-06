licenses(["notice"])  # Apache 2

load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_library",
    "envoy_cc_test",
    "envoy_cc_test_library",
    "envoy_package",
)

package(default_visibility = [":internal"])

package_group(
    name = "internal",

    packages = [
        "//source/common/quic/...",
    ],
)

envoy_cc_library(
    name = "quic_string_impl_lib",
    hdrs = [
        "quic_string_impl.h",
    ],
)

envoy_cc_library(
    name = "quic_string_piece_impl_lib",
    hdrs = [
        "quic_string_piece_impl.h",
    ],
    deps = [
    ],
)

envoy_cc_library(
    name = "quic_string_utils_impl_lib",
    hdrs = [
        "quic_string_utils_impl.h",
    ],
    deps = [
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

envoy_cc_library(
    name = "quic_str_cat_impl_lib",
    hdrs = [
        "quic_str_cat_impl.h",
    ],
    deps = [
    ],
)
