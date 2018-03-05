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
