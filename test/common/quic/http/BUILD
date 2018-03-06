# *EXPERIMENTAL* HTTP/2 encoder/decoder. Do NOT depend upon in production code.

# Please put rules in lexical order by name within sections.

package_group(
    name = "gfe_quic",
    packages = [
        # Temporarily allow gfe2 and spdy access to all targets.
        # TODO: Figure out which targets to expose.
        "//gfe/gfe2/...",
        "//source/common/quic/http/...",
        "//source/common/quic/spdy_utils/...",
    ],
)

package(default_visibility = ["//source/common/quic/http:gfe_quic"])
################################################################################
# Libraries (i.e. production code).

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
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test_library(
    name = "quic_http_constants_test_util_lib",
    testonly = 1,
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
        "//source/common/quic/http:quic_http_structures_test_util_lib",
        "//base",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
        "//strings",
        "//testing/base/public:gunit_main",
        "//util/random/utils:test_random_seed_environment",
    ],
)

envoy_cc_test_library(
    name = "quic_http_structures_test_util_lib",
    testonly = 1,
    srcs = [
        "quic_http_structures_test_util.cc",
    ],
    hdrs = [
        "quic_http_structures_test_util.h",
    ],
    deps = [
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_constants_test_util_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//base",
        "//source/common/quic/http/tools:quic_http_frame_builder_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_test_random_lib",
        "//testing/base/public:gunit_for_library",
    ],
)
