# Copyright 2012 Google Inc. All Rights Reserved.
# Author: alyssar@google.com (Alyssa Wilk)
#
# Description:
#   test tools for quic.

package_group(
    name = "internal",

    # If you are adding a package to this list, please make sure to also update
    # _quic_test_patterns in
    # //depot/google3/gfe/tools/continuous_builds/gfe.blueprint otherwise you
    # risk build breakage.
    packages = [
        "//borg/monitoring/b3m/...",
        "//gfe/cluster_broker/...",
        "//gfe/gfe2/...",
        "//source/common/quic/...",
        "//gfe/test_tools/...",
        "//gfe/tools/...",
        "//net/bandaid/internetto/wtrace/agent/...",
        "//net/grpc/internal/src/core/quic/...",
        "//net/grpc/internal/test/cpp/end2end/...",
        "//third_party/pcc_quic/...",
        "//video/streaming/quic/...",
        "//video/streaming/testing/perf/...",
    ],
)

package(
    default_testonly = 1,
    default_visibility = [":internal"],
)

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
