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

#include "common/quic/core/quic_arena_scoped_ptr.h"

#include "common/quic/core/quic_one_block_arena.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace {

enum class TestParam { kFromHeap, kFromArena };

struct TestObject {
  explicit TestObject(uintptr_t value) : value(value) { buffer.resize(1024); }
  uintptr_t value;

  // Ensure that we have a non-trivial destructor that will leak memory if it's
  // not called.
  std::vector<char> buffer;
};

class QuicArenaScopedPtrParamTest : public QuicTestWithParam<TestParam> {
 protected:
  QuicArenaScopedPtr<TestObject> CreateObject(uintptr_t value) {
    QuicArenaScopedPtr<TestObject> ptr;
    switch (GetParam()) {
      case TestParam::kFromHeap:
        ptr = QuicArenaScopedPtr<TestObject>(new TestObject(value));
        CHECK(!ptr.is_from_arena());
        break;
      case TestParam::kFromArena:
        ptr = arena_.New<TestObject>(value);
        CHECK(ptr.is_from_arena());
        break;
    }
    return ptr;
  }

 private:
  QuicOneBlockArena<1024> arena_;
};

INSTANTIATE_TEST_CASE_P(QuicArenaScopedPtrParamTest,
                        QuicArenaScopedPtrParamTest,
                        testing::Values(TestParam::kFromHeap,
                                        TestParam::kFromArena));

TEST_P(QuicArenaScopedPtrParamTest, NullObjects) {
  QuicArenaScopedPtr<TestObject> def;
  QuicArenaScopedPtr<TestObject> null(nullptr);
  EXPECT_EQ(def, null);
  EXPECT_EQ(def, nullptr);
  EXPECT_EQ(null, nullptr);
}

TEST_P(QuicArenaScopedPtrParamTest, FromArena) {
  QuicOneBlockArena<1024> arena_;
  EXPECT_TRUE(arena_.New<TestObject>(0).is_from_arena());
  EXPECT_FALSE(
      QuicArenaScopedPtr<TestObject>(new TestObject(0)).is_from_arena());
}

TEST_P(QuicArenaScopedPtrParamTest, Assign) {
  QuicArenaScopedPtr<TestObject> ptr = CreateObject(12345);
  ptr = CreateObject(54321);
  EXPECT_EQ(54321u, ptr->value);
}

TEST_P(QuicArenaScopedPtrParamTest, MoveConstruct) {
  QuicArenaScopedPtr<TestObject> ptr1 = CreateObject(12345);
  QuicArenaScopedPtr<TestObject> ptr2(std::move(ptr1));
  EXPECT_EQ(nullptr, ptr1);
  EXPECT_EQ(12345u, ptr2->value);
}

TEST_P(QuicArenaScopedPtrParamTest, Accessors) {
  QuicArenaScopedPtr<TestObject> ptr = CreateObject(12345);
  EXPECT_EQ(12345u, (*ptr).value);
  EXPECT_EQ(12345u, ptr->value);
  // We explicitly want to test that get() returns a valid pointer to the data,
  // but the call looks redundant.
  EXPECT_EQ(12345u, ptr.get()->value);  // NOLINT
}

TEST_P(QuicArenaScopedPtrParamTest, Reset) {
  QuicArenaScopedPtr<TestObject> ptr = CreateObject(12345);
  ptr.reset(new TestObject(54321));
  EXPECT_EQ(54321u, ptr->value);
}

TEST_P(QuicArenaScopedPtrParamTest, Swap) {
  QuicArenaScopedPtr<TestObject> ptr1 = CreateObject(12345);
  QuicArenaScopedPtr<TestObject> ptr2 = CreateObject(54321);
  ptr1.swap(ptr2);
  EXPECT_EQ(12345u, ptr2->value);
  EXPECT_EQ(54321u, ptr1->value);
}

}  // namespace
}  // namespace gfe_quic
