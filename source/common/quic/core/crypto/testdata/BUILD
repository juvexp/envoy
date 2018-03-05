package_group(
    name = "internal",
    packages = [
        "//gfe/cluster_broker/...",
        "//gfe/gfe2/...",
        "//source/common/quic/...",
        "//net/grpc/internal/...",
    ],
)

package(
    default_testonly = 1,
    default_visibility = [":internal"],
)

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
