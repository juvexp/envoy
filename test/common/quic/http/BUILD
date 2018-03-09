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


################################################################################
# Tests and test support.

envoy_cc_test(
    name = "quic_http_constants_test",
    srcs = [
        "quic_http_constants_test.cc",
    ],
    deps = [
        "//source/common/quic/http:quic_http_constants_lib",
        "//base",
    ],
)

envoy_cc_test_library(
    name = "quic_http_constants_test_util_lib",
    srcs = [
        "quic_http_constants_test_util.cc",
    ],
    hdrs = [
        "quic_http_constants_test_util.h",
    ],
    deps = [
        "//source/common/quic/http:quic_http_constants_lib",
        "//base",
    ],
)

envoy_cc_test(
    name = "quic_http_structures_test",
    srcs = [
        "quic_http_structures_test.cc",
    ],
    # Sharded because there are many slow EXPECT_DEBUG_DEATH tests.
    shard_count = 10,
    deps = [
        "//source/common/quic/http:quic_http_structures_lib",
        ":quic_http_structures_test_util_lib",
        "//base",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//strings",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_structures_test_util_lib",
    srcs = [
        "quic_http_structures_test_util.cc",
    ],
    hdrs = [
        "quic_http_structures_test_util.h",
    ],
    deps = [
        "//source/common/quic/http:quic_http_constants_lib",
        ":quic_http_constants_test_util_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//base",
        "//test/common/quic/http/tools:quic_http_frame_builder_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
    ],
)
