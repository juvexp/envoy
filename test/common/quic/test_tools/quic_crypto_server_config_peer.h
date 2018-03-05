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

#ifndef GFE_QUIC_TEST_TOOLS_QUIC_CRYPTO_SERVER_CONFIG_PEER_H_
#define GFE_QUIC_TEST_TOOLS_QUIC_CRYPTO_SERVER_CONFIG_PEER_H_

#include "common/quic/core/crypto/quic_crypto_server_config.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {
namespace test {

// Peer for accessing otherwise private members of a QuicCryptoServerConfig.
class QuicCryptoServerConfigPeer {
 public:
  explicit QuicCryptoServerConfigPeer(QuicCryptoServerConfig* server_config)
      : server_config_(server_config) {}

  // Returns the primary config.
  QuicReferenceCountedPointer<QuicCryptoServerConfig::Config>
  GetPrimaryConfig();

  // Returns the config associated with |config_id|.
  QuicReferenceCountedPointer<QuicCryptoServerConfig::Config> GetConfig(
      string config_id);

  // Returns a pointer to the ProofSource object.
  ProofSource* GetProofSource() const;

  // Reset the proof_source_ member.
  void ResetProofSource(std::unique_ptr<ProofSource> proof_source);

  // Generates a new valid source address token.
  string NewSourceAddressToken(string config_id,
                               SourceAddressTokens previous_tokens,
                               const QuicIpAddress& ip,
                               QuicRandom* rand,
                               QuicWallTime now,
                               CachedNetworkParameters* cached_network_params);

  // Attempts to validate the tokens in |tokens|.
  HandshakeFailureReason ValidateSourceAddressTokens(
      string config_id,
      QuicStringPiece tokens,
      const QuicIpAddress& ip,
      QuicWallTime now,
      CachedNetworkParameters* cached_network_params);

  // Attempts to validate the single source address token in |token|.
  HandshakeFailureReason ValidateSingleSourceAddressToken(
      QuicStringPiece token,
      const QuicIpAddress& ip,
      QuicWallTime now);

  // CheckConfigs compares the state of the Configs in |server_config_| to the
  // description given as arguments.
  // The first of each pair is the server config ID of a Config. The second is a
  // boolean describing whether the config is the primary. For example:
  //   CheckConfigs(std::vector<std::pair<ServerConfigID, bool>>());  // checks
  //   that no Configs are loaded.
  //
  //   // Checks that exactly three Configs are loaded with the given IDs and
  //   // status.
  //   CheckConfigs(
  //     {{"id1", false},
  //      {"id2", true},
  //      {"id3", false}});
  void CheckConfigs(
      std::vector<std::pair<ServerConfigID, bool>> expected_ids_and_status);

  // ConfigsDebug returns a string that contains debugging information about
  // the set of Configs loaded in |server_config_| and their status.
  string ConfigsDebug() SHARED_LOCKS_REQUIRED(server_config_->configs_lock_);

  void SelectNewPrimaryConfig(int seconds);

  static string CompressChain(
      QuicCompressedCertsCache* compressed_certs_cache,
      const QuicReferenceCountedPointer<ProofSource::Chain>& chain,
      const string& client_common_set_hashes,
      const string& client_cached_cert_hashes,
      const CommonCertSets* common_sets);

  uint32_t source_address_token_future_secs();

  uint32_t source_address_token_lifetime_secs();

 private:
  QuicCryptoServerConfig* server_config_;
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_QUIC_CRYPTO_SERVER_CONFIG_PEER_H_
