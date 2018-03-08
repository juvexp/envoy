load("//security/fuzzing/blaze:cc_fuzz_target.bzl", "cc_fuzz_target")

cc_fuzz_target(
    name = "quic_framer_fuzzer",
    srcs = ["quic_framer_fuzzer.cc"],
    # Faster Google > GFE > GFE2 > QUIC
    componentid = 81235,
    deps = [
        "//base",
        "//source/common/quic/core:quic_framer_lib",
        "//source/common/quic/core:quic_packets_lib",
        "//source/common/quic/core/crypto:crypto_handshake_lib",
        "//source/common/quic/platform/api:quic_string_piece_lib",
        "//test/common/quic/test_tools:quic_test_utils",
        "//third_party/absl/strings",
    ],
)
