# *EXPERIMENTAL* HTTP/2 Endpoint: understands HTTP/2 at a level above decoding and encoding.

# Please put rules in lexical order by name within sections.

package(default_visibility = ["//gfe/quic/http:gfe_quic"])

################################################################################
# Libraries (i.e. production code).

envoy_cc_test_library(
    name = "quic_http_settings_lib",
    srcs = [
        "quic_http_settings.cc",
    ],
    hdrs = ["quic_http_settings.h"],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
    ],
)

################################################################################
# Tests and test support.

envoy_cc_test(
    name = "quic_http_settings_test",
    srcs = ["quic_http_settings_test.cc"],
    deps = [
        ":quic_http_settings_lib",
        "//base",
        "//test/common/quic/http/tools:quic_http_gunit_helpers_lib",
        "//testing/base/public:gunit_main",
    ],
)
