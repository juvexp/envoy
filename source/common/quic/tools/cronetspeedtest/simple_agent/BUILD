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
        "//uploader/agent:scotty_agent_proto",
        "//util/task:status",
        "//webutil/http",
    ],
)
