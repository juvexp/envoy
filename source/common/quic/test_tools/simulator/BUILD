# Description:
#   Code for simulating a network.  Used in QUIC tests.  Shared with Chrome.

package(default_visibility = [":internal"])

package_group(
    name = "internal",
    packages = [
        "//gfe/gfe2/...",
        "//source/common/quic/...",
        "//third_party/pcc_quic/...",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
