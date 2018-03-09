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
    name = "testdata",
    srcs = ["nonce.txt"] + glob([
        "*.crt",
        "*.key",
        "*.key.encrypted",
        "*.sct_list",
    ]),
)

go_test(
    name = "generate_test",
    srcs = ["generate_test.go"],
    data = ["//gfe/gfe2/ssl_cert_storage:ssl_key_encryptor"],
    defines_main = 1,
)
