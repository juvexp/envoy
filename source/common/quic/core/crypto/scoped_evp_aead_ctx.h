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

#ifndef GFE_QUIC_CORE_CRYPTO_SCOPED_EVP_AEAD_CTX_H_
#define GFE_QUIC_CORE_CRYPTO_SCOPED_EVP_AEAD_CTX_H_

#include "base/macros.h"
#include "third_party/openssl/evp.h"

namespace gfe_quic {

// ScopedEVPAEADCtx manages an EVP_AEAD_CTX object and calls the needed cleanup
// functions when it goes out of scope.
class ScopedEVPAEADCtx {
 public:
  ScopedEVPAEADCtx();
  ~ScopedEVPAEADCtx();

  EVP_AEAD_CTX* get();

 private:
  EVP_AEAD_CTX ctx_;

  DISALLOW_COPY_AND_ASSIGN(ScopedEVPAEADCtx);
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_CRYPTO_SCOPED_EVP_AEAD_CTX_H_
