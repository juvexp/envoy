# Description: Tools in support of http2 decoder/encoder.
# Please put rules in lexical order by name within sections.

package(default_visibility = ["//source/common/quic/http:gfe_quic"])

load("//gfe/gfe2/tools:build_defs.bzl", "envoy_cc_test_library")

################################################################################
# Libraries.

################################################################################
# Tests of the above libraries and test support.

################################################################################
# Binaries (e.g. cmdline tools).

envoy_cc_binary(
    name = "quic_http_frame_formatter",
    testonly = 1,
    srcs = ["quic_http_frame_formatter_main.cc"],
    deps = [
        ":quic_http_frame_formatter_lib",
        "//base",
        "//file/base",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//util/task:status",
        "//util/task:statusor",
    ],
)
