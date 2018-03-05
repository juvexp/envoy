# Description:
#   Protobufs used in QUIC.

package_group(
    name = "internal",
    packages = [
        "//gfe/cluster_broker/...",
        "//gfe/gfe2/...",
        "//source/common/quic/...",
    ],
)

package(
    default_hdrs_check = "strict",
)

proto_library(
    name = "cached_network_parameters_proto",
    srcs = ["cached_network_parameters.proto"],
    cc_api_version = 2,
    java_api_version = 2,
    js_api_version = 2,
    visibility = [
        "//logs/proto/faster:__subpackages__",
    ],
    deps = [
        "//logs/proto/logs_annotations",
    ],
)

proto_library(
    name = "source_address_token_proto",
    srcs = ["source_address_token.proto"],
    cc_api_version = 2,
    deps = [
        ":cached_network_parameters_proto",
    ],
)

proto_library(
    name = "crypto_server_config_proto",
    srcs = ["crypto_server_config.proto"],
    cc_api_version = 2,
)

load("//net/proto2/contrib/portable/cc:portable_proto_build_defs.bzl", "portable_proto_library")

portable_proto_library(
    name = "portable_quic_core_proto",
    config = "quic_portable_proto_filter.asciipb",
    header_outs = [
        "cached_network_parameters.proto.h",
        "crypto_server_config.proto.h",
        "source_address_token.proto.h",
    ],
    output_google3_proto = True,
    proto_deps = [
        ":cached_network_parameters_proto",
        ":crypto_server_config_proto",
        ":source_address_token_proto",
    ],
    visibility = [":internal"],
)
