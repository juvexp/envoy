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
    name = "spdy_estimate_memory_usage_lib",
    hdrs = [
        "spdy_estimate_memory_usage.h",
    ],
    deps = [
        "//source/common/quic/spdy_utils/platform/impl:spdy_estimate_memory_usage_impl_lib",
    ],
)

envoy_cc_library(
    name = "spdy_export_lib",
    hdrs = [
        "spdy_export.h",
    ],
    deps = [
        "//source/common/quic/spdy_utils/platform/impl:spdy_export_impl_lib",
    ],
)

envoy_cc_library(
    name = "spdy_flags_lib",
    hdrs = ["spdy_flags.h"],
    deps = ["//source/common/quic/spdy_utils/platform/impl:spdy_flags_impl_lib"],
)

envoy_cc_library(
    name = "spdy_mem_slice_lib",
    hdrs = [
        "spdy_mem_slice.h",
    ],
    deps = [
        ":spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/impl:spdy_mem_slice_impl_lib",
    ],
)

envoy_cc_library(
    name = "spdy_ptr_util_lib",
    hdrs = [
        "spdy_ptr_util.h",
    ],
    deps = [
        "//source/common/quic/spdy_utils/platform/impl:spdy_ptr_util_impl_lib",
    ],
)

envoy_cc_library(
    name = "spdy_string_lib",
    hdrs = [
        "spdy_string.h",
    ],
    deps = [
        "//source/common/quic/spdy_utils/platform/impl:spdy_string_impl_lib",
    ],
)

envoy_cc_library(
    name = "spdy_string_piece_lib",
    hdrs = [
        "spdy_string_piece.h",
    ],
    deps = [
        "//source/common/quic/spdy_utils/platform/impl:spdy_string_piece_impl_lib",
    ],
)

envoy_cc_library(
    name = "spdy_string_utils_lib",
    hdrs = [
        "spdy_string_utils.h",
    ],
    deps = [
        ":spdy_mem_slice_lib",
        ":spdy_string_lib",
        ":spdy_string_piece_lib",
        "//source/common/quic/spdy_utils/platform/impl:spdy_string_utils_impl_lib",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
