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


filegroup(
    name = "quic_http_response_cache_data",
    srcs = glob(
        [
            "quic_http_response_cache_data/test.example.com/*",
            "quic_http_response_cache_data_with_push/test.example.com/*",
        ],
    ),
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
