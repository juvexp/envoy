# Description:
#   Code describing QUIC frame types.

package(default_visibility = ["//source/common/quic/core/frames:internal"])

package_group(
    name = "internal",
    packages = [
        "//gfe/gfe2/...",
        "//source/common/quic/...",
    ],
)

envoy_cc_test(
    name = "quic_frames_test",
    srcs = ["quic_frames_test.cc"],
    deps = [
        "//source/common/quic/core/frames:quic_frames_lib",
        "//source/common/quic/platform/api:quic_test_lib",
        "//source/common/quic/test_tools:quic_test_utils",
        "//testing/base/public:gunit_main",
    ],
)
