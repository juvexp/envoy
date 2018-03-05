// Copyright 2018 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// A binary wrapper for QuicServer.  It listens forever on --port
// (default 6121) until it's killed or ctrl-cd to death.

#include "base/commandlineflags.h"
#include "base/init_google.h"
#include "common/quic/core/crypto/proof_source_google3.h"
#include "common/quic/platform/api/quic_flags.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/tools/quic_server.h"
#include "net/httpsconnection/certificates.proto.h"
#include "net/httpsconnection/sslcontext.h"

DEFINE_int32(port, 6121, "The port the quic server will listen on.");
DEFINE_string(certificate_dir,
              "/google/src/head/depot/google3/gfe/quic/core/crypto/testdata",
              "The directory containing certificate files.");
DEFINE_string(intermediate_certificate_name,
              "intermediate.crt",
              "The name of the file containing the intermediate certificate.");
DEFINE_string(leaf_certificate_name,
              "test.example.com",
              "The name of the file containing the leaf certificate.");

std::unique_ptr<gfe_quic::ProofSource> CreateProofSource(
    const string& base_directory,
    const string& intermediate_cert_name,
    const string& leaf_cert_name) {
  SetQuicFlag(&FLAGS_disable_permission_validation, true);

  httpsconnection::CertificateConfig config;
  config.set_base_directory(base_directory);
  config.set_issuing_certificates_file(intermediate_cert_name);
  config.add_cert()->set_name(leaf_cert_name);

  auto ssl_ctx = std::make_shared<SSLContext>(
      SSLContext::SSL_SERVER_CONTEXT,
      SSLContext::SESSION_CACHE_SERVER |
          SSLContext::SESSION_CACHE_NO_INTERNAL_STORE);
  CHECK_OK(ssl_ctx->Initialize(config));

  return std::unique_ptr<gfe_quic::ProofSource>(
      new gfe_quic::ProofSourceGoogle3(ssl_ctx, "unused_cert_mpm_version"));
}

int main(int argc, char* argv[]) {
  InitGoogle(argv[0], &argc, &argv, true);

  gfe_quic::QuicHttpResponseCache response_cache;
  gfe_quic::QuicServer server(
      CreateProofSource(FLAGS_certificate_dir,
                        FLAGS_intermediate_certificate_name,
                        FLAGS_leaf_certificate_name),
      &response_cache);

  if (!server.CreateUDPSocketAndListen(gfe_quic::QuicSocketAddress(
          gfe_quic::QuicIpAddress::Any6(), FLAGS_port))) {
    return 1;
  }

  while (true) {
    server.WaitForEvents();
  }
}
