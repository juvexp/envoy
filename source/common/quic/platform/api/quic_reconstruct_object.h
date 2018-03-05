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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_RECONSTRUCT_OBJECT_H_
#define GFE_QUIC_PLATFORM_API_QUIC_RECONSTRUCT_OBJECT_H_

#include <utility>

#include "common/quic/platform/impl/quic_reconstruct_object_impl.h"

class RandomBase;

namespace gfe_quic {
namespace test {

// Reconstruct an object so that it is initialized as when it was first
// constructed. Runs the destructor to handle objects that might own resources,
// and runs the constructor with the provided arguments, if any.
template <class T, class... Args>
void QuicReconstructObject(T* ptr, RandomBase* rng, Args&&... args) {
  QuicReconstructObjectImpl(ptr, rng, std::forward<Args>(args)...);
}

// This version applies default-initialization to the object.
template <class T>
void QuicDefaultReconstructObject(T* ptr, RandomBase* rng) {
  QuicDefaultReconstructObjectImpl(ptr, rng);
}

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_RECONSTRUCT_OBJECT_H_
