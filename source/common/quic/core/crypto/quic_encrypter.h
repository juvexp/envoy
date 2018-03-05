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

#ifndef GFE_QUIC_CORE_CRYPTO_QUIC_ENCRYPTER_H_
#define GFE_QUIC_CORE_CRYPTO_QUIC_ENCRYPTER_H_

#include <cstddef>

#include "common/quic/core/quic_packets.h"
#include "common/quic/platform/api/quic_export.h"
#include "common/quic/platform/api/quic_string_piece.h"

namespace gfe_quic {

class QUIC_EXPORT_PRIVATE QuicEncrypter {
 public:
  virtual ~QuicEncrypter() {}

  static QuicEncrypter* Create(QuicTag algorithm);

  // Creates an IETF QuicEncrypter based on |cipher_suite| which must be an id
  // returned by SSL_CIPHER_get_id. The caller is responsible for taking
  // ownership of the new QuicEncrypter.
  static QuicEncrypter* CreateFromCipherSuite(uint32_t cipher_suite);

  // Sets the encryption key. Returns true on success, false on failure.
  //
  // NOTE: The key is the client_write_key or server_write_key derived from
  // the master secret.
  virtual bool SetKey(QuicStringPiece key) = 0;

  // Sets the fixed initial bytes of the nonce. Returns true on success,
  // false on failure. This method must only be used with Google QUIC crypters.
  //
  // NOTE: The nonce prefix is the client_write_iv or server_write_iv
  // derived from the master secret. A 64-bit packet number will
  // be appended to form the nonce.
  //
  //                          <------------ 64 bits ----------->
  //   +---------------------+----------------------------------+
  //   |    Fixed prefix     |      packet number      |
  //   +---------------------+----------------------------------+
  //                          Nonce format
  //
  // The security of the nonce format requires that QUIC never reuse a
  // packet number, even when retransmitting a lost packet.
  virtual bool SetNoncePrefix(QuicStringPiece nonce_prefix) = 0;

  // Sets |iv| as the initialization vector to use when constructing the nonce.
  // Returns true on success, false on failure. This method must only be used
  // with IETF QUIC crypters.
  //
  // Google QUIC and IETF QUIC use different nonce constructions. This method
  // must be used when using IETF QUIC; SetNoncePrefix must be used when using
  // Google QUIC.
  //
  // The nonce is constructed as follows (draft-ietf-quic-tls section 5.3):
  //
  //    <---------------- max(8, N_MIN) bytes ----------------->
  //   +--------------------------------------------------------+
  //   |                 packet protection IV                   |
  //   +--------------------------------------------------------+
  //                             XOR
  //                          <------------ 64 bits ----------->
  //   +---------------------+----------------------------------+
  //   |        zeroes       |   reconstructed packet number    |
  //   +---------------------+----------------------------------+
  //
  // The nonce is the packet protection IV (|iv|) XOR'd with the left-padded
  // reconstructed packet number.
  //
  // The security of the nonce format requires that QUIC never reuse a
  // packet number, even when retransmitting a lost packet.
  virtual bool SetIV(QuicStringPiece iv) = 0;

  // Writes encrypted |plaintext| and a MAC over |plaintext| and
  // |associated_data| into output. Sets |output_length| to the number of
  // bytes written. Returns true on success or false if there was an error.
  // |packet_number| is appended to the |nonce_prefix| value provided in
  // SetNoncePrefix() to form the nonce. |output| must not overlap with
  // |associated_data|. If |output| overlaps with |plaintext| then
  // |plaintext| must be <= |output|.
  virtual bool EncryptPacket(QuicTransportVersion version,
                             QuicPacketNumber packet_number,
                             QuicStringPiece associated_data,
                             QuicStringPiece plaintext,
                             char* output,
                             size_t* output_length,
                             size_t max_output_length) = 0;

  // GetKeySize() and GetNoncePrefixSize() tell the HKDF class how many bytes
  // of key material needs to be derived from the master secret.
  // NOTE: the sizes returned by GetKeySize() and GetNoncePrefixSize() are
  // also correct for the QuicDecrypter of the same algorithm. So only
  // QuicEncrypter has these two methods.

  // Returns the size in bytes of a key for the algorithm.
  virtual size_t GetKeySize() const = 0;
  // Returns the size in bytes of the fixed initial part of the nonce.
  virtual size_t GetNoncePrefixSize() const = 0;

  // Returns the size in bytes of an IV to use with the algorithm.
  virtual size_t GetIVSize() const = 0;

  // Returns the maximum length of plaintext that can be encrypted
  // to ciphertext no larger than |ciphertext_size|.
  virtual size_t GetMaxPlaintextSize(size_t ciphertext_size) const = 0;

  // Returns the length of the ciphertext that would be generated by encrypting
  // to plaintext of size |plaintext_size|.
  virtual size_t GetCiphertextSize(size_t plaintext_size) const = 0;

  // For use by unit tests only.
  virtual QuicStringPiece GetKey() const = 0;
  virtual QuicStringPiece GetNoncePrefix() const = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_QUIC_ENCRYPTER_H_
