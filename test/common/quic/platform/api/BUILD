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
    name = "quic_ref_counted_test",
    srcs = ["quic_reference_counted_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_ref_counted_lib",
        ":quic_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_str_cat_test",
    srcs = ["quic_str_cat_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        ":quic_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_lru_cache_test",
    srcs = ["quic_lru_cache_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_lru_cache_lib",
        ":quic_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_text_utils_test",
    srcs = ["quic_text_utils_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_string_lib",
        ":quic_test_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_test_lib",
    hdrs = [
        "quic_test.h",
    ],
    deps = [
        "//test/common/quic/platform/impl:quic_test_impl_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_test_lib_no_google3",
    hdrs = [
        "quic_test.h",
    ],
    deps = [
        "//gfe/quic/platform/impl:quic_test_impl_lib_no_google3",
    ],
)

envoy_cc_test(
    name = "quic_hostname_utils_test",
    srcs = ["quic_hostname_utils_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_arraysize_lib",
        "//source/common/quic/platform/api:quic_hostname_utils_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        ":quic_test_lib",
    ],
)

envoy_cc_test(
    name = "quic_url_test",
    srcs = ["quic_url_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_string_lib",
        ":quic_test_lib",
        "//source/common/quic/platform/api:quic_url_lib",
    ],
)

envoy_cc_test(
    name = "quic_endian_test",
    srcs = ["quic_endian_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_endian_lib",
        ":quic_test_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_test_loopback_lib",
    srcs = ["quic_test_loopback.cc"],
    hdrs = [
        "quic_test_loopback.h",
    ],
    deps = [
        "//gfe/quic/platform/impl:quic_test_loopback_impl_lib",
    ],
)

envoy_cc_test(
    name = "quic_mem_slice_test",
    srcs = ["quic_mem_slice_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_mem_slice_lib",
        ":quic_test_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
    ],
)

envoy_cc_test_library(
    name = "quic_test_mem_slice_vector_lib",
    hdrs = [
        "quic_test_mem_slice_vector.h",
    ],
    deps = [
        "//source/common/quic/platform/api:quic_mem_slice_span_lib",
        "//gfe/quic/platform/impl:quic_test_mem_slice_vector_impl_lib",
    ],
)

envoy_cc_test(
    name = "quic_mem_slice_span_test",
    srcs = ["quic_mem_slice_span_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_mem_slice_span_lib",
        ":quic_test_lib",
        ":quic_test_mem_slice_vector_lib",
        "//source/common/quic/core:quic_simple_buffer_allocator_lib",
        "//source/common/quic/core:quic_stream_send_buffer_lib",
    ],
)

envoy_cc_test(
    name = "quic_string_utils_test",
    srcs = ["quic_string_utils_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
    ],
)

envoy_cc_test(
    name = "quic_singleton_test",
    srcs = ["quic_singleton_test.cc"],
    deps = [
        "//source/common/quic/platform/api:quic_singleton_lib",
        ":quic_test_lib",
    ],
)
