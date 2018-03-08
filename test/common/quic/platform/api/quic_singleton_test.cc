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

#include "common/quic/platform/api/quic_singleton.h"

#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {
namespace {

class Foo {
 public:
  static Foo* GetInstance() { return gfe_quic::QuicSingleton<Foo>::get(); }

 private:
  Foo() = default;
  friend gfe_quic::QuicSingletonFriend<Foo>;
};

class QuicSingletonTest : public QuicTest {};

TEST_F(QuicSingletonTest, Get) {
  Foo* f1 = Foo::GetInstance();
  Foo* f2 = Foo::GetInstance();
  EXPECT_EQ(f1, f2);
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
