# *EXPERIMENTAL* HTTP/2 encoder/decoder. Do NOT depend upon in production code.

# Please put rules in lexical order by name within sections.

package_group(
    name = "gfe_quic",
    packages = [
        # Temporarily allow gfe2 and spdy access to all targets.
        # TODO: Figure out which targets to expose.
        "//gfe/gfe2/...",
        "//source/common/quic/http/...",
        "//source/common/quic/spdy_utils/...",
    ],
)

package(default_visibility = ["//source/common/quic/http:gfe_quic"])
################################################################################
# Libraries (i.e. production code).

envoy_cc_library(
    name = "quic_http_constants_lib",
    srcs = [
        "quic_http_constants.cc",
    ],
    hdrs = [
        "quic_http_constants.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_structures_lib",
    srcs = [
        "quic_http_structures.cc",
    ],
    hdrs = [
        "quic_http_structures.h",
    ],
    deps = [
        ":quic_http_constants_lib",
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_str_cat_lib",
        "//source/common/quic/platform/api:quic_string_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//source/common/quic/platform/api:quic_text_utils_lib",
    ],
)

################################################################################
# Tests and test support.
