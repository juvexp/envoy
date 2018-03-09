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
    name = "quic_aligned_lib",
    hdrs = [
        "quic_aligned.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_aligned_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_export_lib",
    hdrs = [
        "quic_export.h",
    ],
    deps = ["//source/common/quic/platform/impl:quic_export_impl_lib"],
)

envoy_cc_library(
    name = "quic_flags_lib",
    hdrs = [
        "quic_flags.h",
    ],
    deps = ["//source/common/quic/platform/impl:quic_flags_impl_lib"],
)

envoy_cc_library(
    name = "quic_ip_address_lib",
    srcs = [
        "quic_ip_address.cc",
    ],
    hdrs = [
        "quic_ip_address.h",
    ],
    deps = [
        ":quic_export_lib",
        ":quic_string_lib",
        "//source/common/quic/platform/impl:quic_ip_address_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_socket_address_lib",
    srcs = [
        "quic_socket_address.cc",
    ],
    hdrs = [
        "quic_socket_address.h",
    ],
    deps = [
        ":quic_export_lib",
        ":quic_ip_address_lib",
        ":quic_string_lib",
        "//source/common/quic/platform/impl:quic_socket_address_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_ip_address_family_lib",
    hdrs = [
        "quic_ip_address_family.h",
    ],
)

envoy_cc_library(
    name = "quic_clock_lib",
    srcs = ["quic_clock.cc"],
    hdrs = ["quic_clock.h"],
    deps = [
        ":quic_export_lib",
        "//source/common/quic/core:quic_time_lib",
    ],
)

envoy_cc_library(
    name = "quic_mutex_lib",
    srcs = [
        "quic_mutex.cc",
    ],
    hdrs = [
        "quic_mutex.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/impl:quic_mutex_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_ref_counted_lib",
    hdrs = [
        "quic_reference_counted.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_ref_counted_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_str_cat_lib",
    hdrs = [
        "quic_str_cat.h",
    ],
    deps = [
        ":quic_string_lib",
        "//source/common/quic/platform/impl:quic_str_cat_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_lru_cache_lib",
    hdrs = [
        "quic_lru_cache.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_lru_cache_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_text_utils_lib",
    hdrs = [
        "quic_text_utils.h",
    ],
    deps = [
        ":quic_string_lib",
        ":quic_string_piece_lib",
        "//source/common/quic/platform/impl:quic_text_utils_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_bug_tracker_lib",
    hdrs = [
        "quic_bug_tracker.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_bug_tracker_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_logging_lib",
    hdrs = [
        "quic_logging.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_logging_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_fallthrough_lib",
    hdrs = [
        "quic_fallthrough.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_fallthrough_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_ptr_util_lib",
    hdrs = [
        "quic_ptr_util.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_ptr_util_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_stack_trace_lib",
    hdrs = [
        "quic_stack_trace.h",
    ],
    deps = [
        ":quic_string_lib",
        "//source/common/quic/platform/impl:quic_stack_trace_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_map_util_lib",
    hdrs = [
        "quic_map_util.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_map_util_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_hostname_utils_lib",
    srcs = ["quic_hostname_utils.cc"],
    hdrs = ["quic_hostname_utils.h"],
    deps = [
        ":quic_export_lib",
        ":quic_string_piece_lib",
        "//base",
        "//source/common/quic/platform/impl:quic_hostname_utils_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_url_utils_lib",
    srcs = ["quic_url_utils.cc"],
    hdrs = [
        "quic_url_utils.h",
    ],
    deps = [
        ":quic_export_lib",
        ":quic_string_lib",
        ":quic_string_piece_lib",
        "//base",
        "//source/common/quic/platform/impl:quic_url_utils_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_containers_lib",
    hdrs = [
        "quic_containers.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_containers_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_url_lib",
    srcs = ["quic_url.cc"],
    hdrs = [
        "quic_url.h",
    ],
    deps = [
        ":quic_export_lib",
        ":quic_string_lib",
        ":quic_string_piece_lib",
        "//source/common/quic/platform/impl:quic_url_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_estimate_memory_usage_lib",
    hdrs = [
        "quic_estimate_memory_usage.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_estimate_memory_usage_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_flag_utils_lib",
    hdrs = [
        "quic_flag_utils.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_flag_utils_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_endian_lib",
    hdrs = [
        "quic_endian.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_endian_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_string_piece_lib",
    hdrs = [
        "quic_string_piece.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_string_piece_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_pcc_sender_lib",
    hdrs = [
        "quic_pcc_sender.h",
    ],
    deps = [
        "//source/common/quic/core/congestion_control:quic_congestion_control_hdrs",
        "//source/common/quic/platform/impl:quic_pcc_sender_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_mem_slice_lib",
    hdrs = [
        "quic_mem_slice.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_mem_slice_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_mem_slice_span_lib",
    hdrs = [
        "quic_mem_slice_span.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_mem_slice_span_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_optional_lib",
    hdrs = [
        "quic_optional.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_optional_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_prefetch_lib",
    hdrs = [
        "quic_prefetch.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_prefetch_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_test_random_lib",
    hdrs = [
        "quic_test_random.h",
    ],
    deps = [
        ":quic_string_lib",
        "//source/common/quic/platform/impl:quic_test_random_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_reconstruct_object_lib",
    hdrs = [
        "quic_reconstruct_object.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_reconstruct_object_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_string_lib",
    hdrs = [
        "quic_string.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_string_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_string_utils_lib",
    hdrs = [
        "quic_string_utils.h",
    ],
    deps = [
        ":quic_string_lib",
        ":quic_string_piece_lib",
        "//source/common/quic/platform/impl:quic_string_utils_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_arraysize_lib",
    hdrs = [
        "quic_arraysize.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_arraysize_impl_lib",
    ],
)

envoy_cc_library(
    name = "quic_singleton_lib",
    hdrs = [
        "quic_singleton.h",
    ],
    deps = [
        "//source/common/quic/platform/impl:quic_singleton_impl_lib",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
