# Experimental HTTP/2 decoder.
# Please put rules in lexical order by name within sections.

package(default_visibility = [":internal"])

package_group(
    name = "internal",
    packages = [
        # Temporarily allow gfe2 and spdy access to all targets.
        # TODO: Figure out which targets to expose.
        "//source/common/quic/...",
    ],
)

################################################################################
# Libraries (i.e. production code).

envoy_cc_library(
    name = "quic_http_deframer_lib",
    srcs = [
        "quic_http_frame_decoder_adapter.cc",
    ],
    hdrs = [
        "quic_http_frame_decoder_adapter.h",
    ],
    deps = [
        ":quic_http_decode_buffer_lib",
        ":quic_http_decode_status_lib",
        ":quic_http_frame_decoder_lib",
        ":quic_http_frame_decoder_listener_lib",
        "//base",
        "//gfe/gfe2:feature_flags_lib",
        "//gfe/gfe2/base:bug_utils",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/platform/api:quic_flags_lib",
        "//source/common/quic/platform/api:quic_optional_lib",
        "//source/common/quic/platform/api:quic_ptr_util_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//source/common/quic/platform/api:quic_string_utils_lib",
        "//source/common/quic/spdy_utils/core:http2_deframer_lib",
        "//source/common/quic/spdy_utils/core:spdy_alt_svc_wire_format_lib",
        "//source/common/quic/spdy_utils/core:spdy_bug_tracker",
        "//source/common/quic/spdy_utils/core:spdy_framer_lib",
        "//source/common/quic/spdy_utils/core:spdy_header_block_lib",
        "//source/common/quic/spdy_utils/core:spdy_headers_handler_interface",
        "//source/common/quic/spdy_utils/core:spdy_protocol_lib",
        "//source/common/quic/spdy_utils/core/hpack:hpack_decoder_adapter_lib",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//strings",
        "//third_party/absl/strings",
        "//util/endian",
    ],
)

envoy_cc_library(
    name = "quic_http_decode_buffer_lib",
    srcs = [
        "quic_http_decode_buffer.cc",
    ],
    hdrs = [
        "quic_http_decode_buffer.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
    ],
)

# Provides functions for decoding each type of structure.
# TODO: Rename to http2_structure_decoder_functions_lib,
# or combine with http2_structure_decoder_lib.
envoy_cc_library(
    name = "quic_http_decode_structures_lib",
    srcs = [
        "quic_http_decode_structures.cc",
    ],
    hdrs = [
        "quic_http_decode_structures.h",
    ],
    deps = [
        ":quic_http_decode_buffer_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_decode_status_lib",
    srcs = [
        "quic_http_decode_status.cc",
    ],
    hdrs = [
        "quic_http_decode_status.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_frame_decoder_state_lib",
    srcs = [
        "quic_http_frame_decoder_state.cc",
    ],
    hdrs = [
        "quic_http_frame_decoder_state.h",
    ],
    deps = [
        ":quic_http_decode_buffer_lib",
        ":quic_http_decode_status_lib",
        ":quic_http_frame_decoder_listener_lib",
        ":quic_http_structure_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_frame_decoder_lib",
    srcs = [
        "quic_http_frame_decoder.cc",
    ],
    hdrs = [
        "quic_http_frame_decoder.h",
        "quic_http_frame_decoder_state.h",
    ],
    deps = [
        ":quic_http_decode_buffer_lib",
        ":quic_http_decode_status_lib",
        ":quic_http_frame_decoder_listener_lib",
        ":quic_http_frame_decoder_state_lib",
        ":quic_http_structure_decoder_lib",
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_altsvc_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_continuation_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_data_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_goaway_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_headers_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_ping_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_priority_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_push_promise_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_rst_stream_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_settings_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_unknown_payload_decoder_lib",
        "//source/common/quic/http/decoder/payload_decoders:quic_http_window_update_payload_decoder_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
        "//source/common/quic/platform/api:quic_logging_lib",
    ],
)

envoy_cc_library(
    name = "quic_http_frame_decoder_listener_lib",
    srcs = [
        "quic_http_frame_decoder_listener.cc",
    ],
    hdrs = [
        "quic_http_frame_decoder_listener.h",
    ],
    deps = [
        "//base",
        "//source/common/quic/http:quic_http_constants_lib",
        "//source/common/quic/http:quic_http_structures_lib",
    ],
)

# Class wrapping the fast decoding functions of decode_structures_lib,
# with a buffer for those rare cases where the structure is split across
# transport buffers.
envoy_cc_library(
    name = "quic_http_structure_decoder_lib",
    srcs = [
        "quic_http_structure_decoder.cc",
    ],
    hdrs = [
        "quic_http_structure_decoder.h",
    ],
    deps = [
        ":quic_http_decode_buffer_lib",
        ":quic_http_decode_status_lib",
        ":quic_http_decode_structures_lib",
        "//base",
        "//source/common/quic/http:quic_http_structures_lib",
        "//source/common/quic/platform/api:quic_bug_tracker_lib",
        "//source/common/quic/platform/api:quic_export_lib",
    ],
)

################################################################################
# Tests and test support.
