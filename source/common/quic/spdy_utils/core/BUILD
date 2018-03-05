# Copyright 2009 Google Inc. All Rights Reserved.
# Author: fenix@google.com (Roberto Peon)
#
# Package:
#  gfe/spdy/core
#
# Description
#   This is the reference implementation of the HTTP/2 protocol (its framing,
#   to be more accurate). It is replicated in Chromium source tree and in that
#   sense is shared between Google3 and Chromium. It is intended to go under
#   net/spdy in the near future, but for now has to live in the gfe tree.

package(default_visibility = [
    "//gfe/gfe2:gfe2_internal",
    "//source/common/quic:__subpackages__",
    "//source/common/quic/spdy_utils:__subpackages__",
])

envoy_cc_library(
    name = "http2_deframer_lib",
    srcs = [
        "http2_frame_decoder_adapter.cc",
    ],
    hdrs = [
        "http2_frame_decoder_adapter.h",
    ],
    visibility = [
        "//gfe/gfe2:gfe2_internal",
        "//source/common/quic:__subpackages__",
        "//source/common/quic/spdy_utils:__subpackages__",
        "//net/floo/polyjuice/dobby:__subpackages__",
    ],
    deps = [
        ":spdy_alt_svc_wire_format_lib",
        ":spdy_bug_tracker",
        ":spdy_header_block_lib",
        ":spdy_headers_handler_interface",
        ":spdy_protocol_lib",
        "//base",
        "//gfe/http2:http2_constants_lib",
        "//gfe/http2:http2_structures_lib",
        "//gfe/http2/decoder:decode_buffer_lib",
        "//gfe/http2/decoder:decode_status_lib",
        "//gfe/http2/decoder:http2_frame_decoder_lib",
        "//gfe/http2/decoder:http2_frame_decoder_listener_lib",
        "//gfe/http2/platform/api:http2_string_lib",
        "//source/common/quic/spdy_utils/core/hpack:hpack_decoder_adapter_lib",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_flags_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//strings",
        "//util/endian",
        "//util/gtl/labs:optional",
    ],
)

envoy_cc_library(
    name = "spdy_framer_lib",
    srcs = [
        "spdy_frame_builder.cc",
        "spdy_framer.cc",
    ],
    hdrs = [
        "spdy_frame_builder.h",
        "spdy_framer.h",
    ],
    visibility = [
        "//borg/monitoring/b3m:__pkg__",
        "//gfe/gfe2:gfe2_internal",
        "//gfe/http2/tools:__pkg__",
        "//source/common/quic:__subpackages__",
        "//source/common/quic/spdy_utils:__subpackages__",
        "//net/floo/polyjuice/dobby:__subpackages__",
    ],
    deps = [
        ":spdy_alt_svc_wire_format_lib",
        ":spdy_bug_tracker",
        ":spdy_frame_reader_lib",
        ":spdy_header_block_lib",
        ":spdy_headers_handler_interface",
        ":spdy_protocol_lib",
        ":zero_copy_output_buffer_lib",
        "//base",
        "//gfe/gfe2/base:flag_varz_utils",
        "//source/common/quic/spdy_utils/core/hpack:hpack_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//strings",
        "//testing/production_stub/public:gunit_prod",
        "//third_party/absl/strings",
        "//util/gtl:lazy_static_ptr",
        "//util/task:status",
    ],
)

envoy_cc_library(
    name = "spdy_alt_svc_wire_format_lib",
    srcs = ["spdy_alt_svc_wire_format.cc"],
    hdrs = ["spdy_alt_svc_wire_format.h"],
    deps = [
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//third_party/absl/container:inlined_vector",
        "//util/gtl:inlined_vector",
    ],
)

envoy_cc_library(
    name = "spdy_bug_tracker",
    hdrs = [
        "spdy_bug_tracker.h",
    ],
    defines = select({
        "//tools/cc_target_os:android": ["SPDY_GENERIC_BUG"],
        "//tools/cc_target_os:darwin": ["SPDY_GENERIC_BUG"],
        "//tools/cc_target_os:ios": ["SPDY_GENERIC_BUG"],
        "//tools/cc_target_os:nacl": ["SPDY_GENERIC_BUG"],
        "//tools/cc_target_os:windows": ["SPDY_GENERIC_BUG"],
        "//conditions:default": [],
    }),
    deps = select({
        "//tools/cc_target_os:android": [],
        "//tools/cc_target_os:darwin": [],
        "//tools/cc_target_os:ios": [],
        "//tools/cc_target_os:nacl": [],
        "//tools/cc_target_os:windows": [],
        "//conditions:default": ["//gfe/gfe2/base:bug_utils"],
    }),
)

envoy_cc_library(
    name = "write_scheduler_lib",
    hdrs = ["write_scheduler.h"],
    deps = [
        ":spdy_protocol_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
    ],
)

envoy_cc_library(
    name = "array_output_buffer_lib",
    srcs = ["array_output_buffer.cc"],
    hdrs = ["array_output_buffer.h"],
    deps = [
        ":zero_copy_output_buffer_lib",
        "//base",
    ],
)

envoy_cc_library(
    name = "spdy_header_block_lib",
    srcs = ["spdy_header_block.cc"],
    hdrs = ["spdy_header_block.h"],
    visibility = [
        "//gfe/gfe2:gfe2_internal",
        "//gfe/http2/tools:__pkg__",
        "//source/common/quic:__subpackages__",
        "//source/common/quic/spdy_utils:__subpackages__",
        "//net/grpc/internal/src/core/quic:__pkg__",
        "//video/streaming/quic:__pkg__",
    ],
    deps = [
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_estimate_memory_usage_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//util/gtl:linked_hash_map",
    ],
)

envoy_cc_library(
    name = "spdy_pinnable_buffer_piece",
    srcs = ["spdy_pinnable_buffer_piece.cc"],
    hdrs = ["spdy_pinnable_buffer_piece.h"],
    deps = [
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
    ],
)

envoy_cc_library(
    name = "spdy_prefixed_buffer_reader",
    srcs = ["spdy_prefixed_buffer_reader.cc"],
    hdrs = ["spdy_prefixed_buffer_reader.h"],
    deps = [
        ":spdy_pinnable_buffer_piece",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
    ],
)

envoy_cc_library(
    name = "spdy_headers_handler_interface",
    hdrs = ["spdy_headers_handler_interface.h"],
    visibility = [
        "//gfe/gfe2:gfe2_internal",
        "//source/common/quic:__subpackages__",
        "//source/common/quic/spdy_utils:__subpackages__",
        "//net/floo/polyjuice/dobby:__subpackages__",
    ],
    deps = [
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
    ],
)

envoy_cc_library(
    name = "priority_write_scheduler_lib",
    hdrs = ["priority_write_scheduler.h"],
    deps = [
        ":spdy_bug_tracker",
        ":spdy_protocol_lib",
        ":write_scheduler_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
    ],
)

envoy_cc_library(
    name = "spdy_frame_reader_lib",
    srcs = ["spdy_frame_reader.cc"],
    hdrs = ["spdy_frame_reader.h"],
    deps = [
        ":spdy_protocol_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
    ],
)

# Include this target if all you need is the protocol definitions.
envoy_cc_library(
    name = "spdy_protocol_lib",
    srcs = ["spdy_protocol.cc"],
    hdrs = [
        "spdy_bitmasks.h",
        "spdy_protocol.h",
    ],
    visibility = [
        "//borg/monitoring/b3m:__pkg__",
        "//gfe/gfe2:gfe2_internal",
        "//gfe/http2/tools:__pkg__",
        "//source/common/quic:__subpackages__",
        "//source/common/quic/spdy_utils:__subpackages__",
        "//net/floo/polyjuice/dobby:__subpackages__",
    ],
    deps = [
        ":spdy_alt_svc_wire_format_lib",
        ":spdy_bug_tracker",
        ":spdy_header_block_lib",
        "//base",
        "//source/common/quic/spdy_utils/platform/api:spdy_export_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_flags_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_ptr_util_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_piece_lib",
        "//source/common/quic/spdy_utils/platform/api:spdy_string_utils_lib",
        "//util/gtl:linked_hash_map",
    ],
)

envoy_cc_library(
    name = "zero_copy_output_buffer_lib",
    hdrs = ["zero_copy_output_buffer.h"],
    deps = [
        "//base",
    ],
)

filegroup(
    name = "exported_header_files",
    srcs = glob(["**/*.h"]),
    visibility = ["//gfe/tools/chromium:__pkg__"],
)
