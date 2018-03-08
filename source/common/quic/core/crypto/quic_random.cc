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

#include "common/quic/core/crypto/quic_random.h"

#include "base/macros.h"
#include "common/quic/platform/api/quic_bug_tracker.h"
#include "common/quic/platform/api/quic_singleton.h"
#include "third_party/openssl/rand.h"

namespace gfe_quic {

namespace {

class DefaultRandom : public QuicRandom {
 public:
  static DefaultRandom* GetInstance();

  // QuicRandom implementation
  void RandBytes(void* data, size_t len) override;
  uint64_t RandUint64() override;
  void Reseed(const void* additional_entropy, size_t entropy_len) override;

 private:
  DefaultRandom() {}
  ~DefaultRandom() override {}

  friend QuicSingletonFriend<DefaultRandom>;
  DISALLOW_COPY_AND_ASSIGN(DefaultRandom);
};

DefaultRandom* DefaultRandom::GetInstance() {
  return QuicSingleton<DefaultRandom>::get();
}

void DefaultRandom::RandBytes(void* data, size_t len) {
  int rv = RAND_bytes(reinterpret_cast<unsigned char*>(data), len);
  if (rv != 1) {
    QUIC_BUG << "RAND_bytes returned " << rv;
  }
}

uint64_t DefaultRandom::RandUint64() {
  uint64_t value;
  RandBytes(&value, sizeof(value));
  return value;
}

void DefaultRandom::Reseed(const void* additional_entropy, size_t entropy_len) {
  RAND_seed(additional_entropy, entropy_len);
}

}  // namespace

// static
QuicRandom* QuicRandom::GetInstance() {
  return DefaultRandom::GetInstance();
}

}  // namespace gfe_quic
