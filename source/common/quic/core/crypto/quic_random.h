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

#ifndef GFE_QUIC_CORE_CRYPTO_QUIC_RANDOM_H_
#define GFE_QUIC_CORE_CRYPTO_QUIC_RANDOM_H_

#include <cstddef>
#include <cstdint>

#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// The interface for a random number generator.
class QUIC_EXPORT_PRIVATE QuicRandom {
 public:
  virtual ~QuicRandom() {}

  // Returns the default random number generator, which is cryptographically
  // secure and thread-safe.
  static QuicRandom* GetInstance();

  // Generates |len| random bytes in the |data| buffer.
  virtual void RandBytes(void* data, size_t len) = 0;

  // Returns a random number in the range [0, kuint64max].
  virtual uint64_t RandUint64() = 0;

  // Reseeds the random number generator with additional entropy input.
  // NOTE: the constructor of a QuicRandom object is responsible for seeding
  // itself with enough entropy input.
  virtual void Reseed(const void* additional_entropy, size_t entropy_len) = 0;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_QUIC_RANDOM_H_
