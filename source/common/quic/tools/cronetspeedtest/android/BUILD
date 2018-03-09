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

android_binary(
    name = "cronetspeedtest",
    srcs = [
        "SpeedTestActivity.java",
    ],
    custom_package = "gfe.quic.tools.cronetspeedtest.android",
    manifest = "AndroidManifest.xml",
    multidex = "native",
    resource_files = glob(["res/**"]),
    deps = [
        "//third_party/java/android_libs/cronet",
        "//third_party/java/android_libs/guava_jdk5:base",
    ],
)
