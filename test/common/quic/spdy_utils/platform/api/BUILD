# Description:
#     Platform independent API used in shared spdy code.

package(default_visibility = ["//source/common/quic/spdy_utils/platform/api:internal"])

package_group(
    name = "internal",
    packages = [
        "//source/common/quic/spdy_utils/...",
    ],
)

envoy_cc_test(
    name = "spdy_mem_slice_test",
    srcs = ["spdy_mem_slice_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/platform/api:spdy_mem_slice_lib",
        "//testing/base/public:gunit_main",
    ],
)

envoy_cc_test(
    name = "spdy_string_utils_test",
    srcs = ["spdy_string_utils_test.cc"],
    deps = [
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//testing/base/public:gunit_main",
    ],
)
