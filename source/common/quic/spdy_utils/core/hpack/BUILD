# Description:
#   Implementation of HTTP/2's HPACK compression (RFC7541).

package(default_visibility = [":internal"])

package_group(
    name = "internal",
    packages = [
        "//source/common/quic/http/...",
        "//source/common/quic/spdy_utils/...",
    ],
)

package_group(
    name = "gfe2",
    packages = [
        "//gfe/gfe2/spdy/...",
        "//gfe/gfe2/test_tools/fuzzing/http2/...",
    ],
)

envoy_cc_library(
    name = "hpack_lib",
    srcs = [
        "hpack_constants.cc",
        "hpack_encoder.cc",
        "hpack_entry.cc",
        "hpack_header_table.cc",
        "hpack_huffman_table.cc",
        "hpack_output_stream.cc",
        "hpack_static_table.cc",
    ],
    hdrs = [
        "hpack_constants.h",
        "hpack_encoder.h",
        "hpack_entry.h",
        "hpack_header_table.h",
        "hpack_huffman_table.h",
        "hpack_output_stream.h",
        "hpack_static_table.h",
    ],
    visibility = [
        ":gfe2",
        ":internal",
    ],
    deps = [
        "//base",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_estimate_memory_usage_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//strings",
        "//util/hash",
    ],
)

envoy_cc_library(
    name = "hpack_decoder_adapter_lib",
    srcs = ["hpack_decoder_adapter.cc"],
    hdrs = [
        "hpack_decoder_adapter.h",
    ],
    deps = [
        ":hpack_lib",
        "//base",
        "//gfe/http2/decoder:decode_buffer_lib",
        "//gfe/http2/decoder:decode_status_lib",
        "//gfe/http2/hpack:hpack_constants_lib",
        "//gfe/http2/hpack:hpack_string_lib",
        "//gfe/http2/hpack/decoder:hpack_decoder_lib",
        "//gfe/http2/hpack/decoder:hpack_decoder_listener_lib",
        "//gfe/http2/hpack/decoder:hpack_decoder_tables_lib",
        "//source/common/quic/spdy_utils/core:spdy_header_block_lib",
        "//source/common/quic/spdy_utils/core:spdy_headers_handler_interface",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
    ],
)
