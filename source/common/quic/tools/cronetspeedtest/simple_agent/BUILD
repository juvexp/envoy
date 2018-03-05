package(default_visibility = ["//uploader:uploader_default_visibility_pkg"])

envoy_cc_binary(
    name = "simple_agent",
    srcs = ["simple_agent.cc"],
    deps = [
        "//base",
        "//blobstore2/public:blob_client",
        "//file/base",
        "//file/colossus/public:cns",
        "//net/base",
        "//net/http",
        "//net/rpc2",
        "//strings",
        "//third_party/absl/strings",
        "//uploader/agent:scotty_agent_proto",
        "//util/task:status",
        "//webutil/http",
    ],
)
