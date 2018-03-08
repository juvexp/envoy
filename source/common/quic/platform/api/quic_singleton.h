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

#ifndef GFE_QUIC_PLATFORM_API_QUIC_SINGLETON_H_
#define GFE_QUIC_PLATFORM_API_QUIC_SINGLETON_H_

#include "common/quic/platform/impl/quic_singleton_impl.h"

namespace gfe_quic {

// Singleton utility. Example usage:
//
// In your header:
//  #include "common/quic/platform/api/quic_singleton.h"
//  class Foo {
//   public:
//    static Foo* GetInstance();
//    void Bar() { ... }
//   private:
//    Foo() { ... }
//    friend gfe_quic::QuicSingletonFriend<Foo>;
//  };
//
// In your source file:
//  Foo* Foo::GetInstance() {
//    return gfe_quic::QuicSingleton<Foo>::get();
//  }
//
// To use the singleton:
//  Foo::GetInstance()->Bar();
//
// NOTE: The method accessing Singleton<T>::get() has to be named as GetInstance
// and it is important that Foo::GetInstance() is not inlined in the
// header. This makes sure that when source files from multiple targets include
// this header they don't end up with different copies of the inlined code
// creating multiple copies of the singleton.
template <typename T>
using QuicSingleton = QuicSingletonImpl<T>;

// Type that a class using QuicSingleton must declare as a friend, in order for
// QuicSingleton to be able to access the class's private constructor.
template <typename T>
using QuicSingletonFriend = QuicSingletonFriendImpl<T>;

}  // namespace gfe_quic

#endif  // GFE_QUIC_PLATFORM_API_QUIC_SINGLETON_H_
