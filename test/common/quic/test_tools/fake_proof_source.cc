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

#include "test/common/quic/test_tools/fake_proof_source.h"

#include "common/quic/platform/api/quic_logging.h"
#include "common/quic/platform/api/quic_ptr_util.h"
#include "test/common/quic/test_tools/crypto_test_utils.h"

namespace gfe_quic {
namespace test {

FakeProofSource::FakeProofSource()
    : delegate_(crypto_test_utils::ProofSourceForTesting()) {}

FakeProofSource::~FakeProofSource() {}

FakeProofSource::PendingOp::~PendingOp() = default;

FakeProofSource::GetProofOp::GetProofOp(
    const QuicSocketAddress& server_addr,
    string hostname,
    string server_config,
    QuicTransportVersion transport_version,
    string chlo_hash,
    std::unique_ptr<ProofSource::Callback> callback,
    ProofSource* delegate)
    : server_address_(server_addr),
      hostname_(std::move(hostname)),
      server_config_(std::move(server_config)),
      transport_version_(transport_version),
      chlo_hash_(std::move(chlo_hash)),
      callback_(std::move(callback)),
      delegate_(delegate) {}

FakeProofSource::GetProofOp::~GetProofOp() = default;

void FakeProofSource::GetProofOp::Run() {
  // Note: relies on the callback being invoked synchronously
  delegate_->GetProof(server_address_, hostname_, server_config_,
                      transport_version_, chlo_hash_, std::move(callback_));
}

FakeProofSource::ComputeSignatureOp::ComputeSignatureOp(
    const QuicSocketAddress& server_address,
    string hostname,
    uint16_t sig_alg,
    QuicStringPiece in,
    std::unique_ptr<ProofSource::SignatureCallback> callback,
    ProofSource* delegate)
    : server_address_(server_address),
      hostname_(std::move(hostname)),
      sig_alg_(sig_alg),
      in_(in),
      callback_(std::move(callback)),
      delegate_(delegate) {}

FakeProofSource::ComputeSignatureOp::~ComputeSignatureOp() = default;

void FakeProofSource::ComputeSignatureOp::Run() {
  delegate_->ComputeTlsSignature(server_address_, hostname_, sig_alg_, in_,
                                 std::move(callback_));
}

void FakeProofSource::Activate() {
  active_ = true;
}

void FakeProofSource::GetProof(
    const QuicSocketAddress& server_address,
    const string& hostname,
    const string& server_config,
    QuicTransportVersion transport_version,
    QuicStringPiece chlo_hash,
    std::unique_ptr<ProofSource::Callback> callback) {
  if (!active_) {
    delegate_->GetProof(server_address, hostname, server_config,
                        transport_version, chlo_hash, std::move(callback));
    return;
  }

  pending_ops_.push_back(QuicMakeUnique<GetProofOp>(
      server_address, hostname, server_config, transport_version,
      string(chlo_hash), std::move(callback), delegate_.get()));
}

QuicReferenceCountedPointer<ProofSource::Chain> FakeProofSource::GetCertChain(
    const QuicSocketAddress& server_address,
    const string& hostname) {
  return delegate_->GetCertChain(server_address, hostname);
}

void FakeProofSource::ComputeTlsSignature(
    const QuicSocketAddress& server_address,
    const string& hostname,
    uint16_t signature_algorithm,
    QuicStringPiece in,
    std::unique_ptr<ProofSource::SignatureCallback> callback) {
  QUIC_LOG(INFO) << "FakeProofSource::ComputeTlsSignature";
  if (!active_) {
    QUIC_LOG(INFO) << "Not active - directly calling delegate";
    delegate_->ComputeTlsSignature(
        server_address, hostname, signature_algorithm, in, std::move(callback));
    return;
  }

  QUIC_LOG(INFO) << "Adding pending op";
  pending_ops_.push_back(QuicMakeUnique<ComputeSignatureOp>(
      server_address, hostname, signature_algorithm, in, std::move(callback),
      delegate_.get()));
}

int FakeProofSource::NumPendingCallbacks() const {
  return pending_ops_.size();
}

void FakeProofSource::InvokePendingCallback(int n) {
  CHECK(NumPendingCallbacks() > n);

  pending_ops_[n]->Run();

  auto it = pending_ops_.begin() + n;
  pending_ops_.erase(it);
}

}  // namespace test
}  // namespace gfe_quic
