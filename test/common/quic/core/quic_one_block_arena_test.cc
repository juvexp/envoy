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

#include "common/quic/core/quic_one_block_arena.h"

#include <cstdint>

#include "common/quic/platform/api/quic_containers.h"
#include "test/common/quic/platform/api/quic_test.h"
#include "test/common/quic/test_tools/quic_test_utils.h"

namespace gfe_quic {
namespace {

static const uint32_t kMaxAlign = 8;

struct TestObject {
  uint32_t value;
};

class QuicOneBlockArenaTest : public QuicTest {};

TEST_F(QuicOneBlockArenaTest, AllocateSuccess) {
  QuicOneBlockArena<1024> arena;
  QuicArenaScopedPtr<TestObject> ptr = arena.New<TestObject>();
  EXPECT_TRUE(ptr.is_from_arena());
}

TEST_F(QuicOneBlockArenaTest, Exhaust) {
  QuicOneBlockArena<1024> arena;
  for (size_t i = 0; i < 1024 / kMaxAlign; ++i) {
    QuicArenaScopedPtr<TestObject> ptr = arena.New<TestObject>();
    EXPECT_TRUE(ptr.is_from_arena());
  }
  QuicArenaScopedPtr<TestObject> ptr;
  EXPECT_QUIC_BUG(ptr = arena.New<TestObject>(),
                  "Ran out of space in QuicOneBlockArena");
  EXPECT_FALSE(ptr.is_from_arena());
}

TEST_F(QuicOneBlockArenaTest, NoOverlaps) {
  QuicOneBlockArena<1024> arena;
  std::vector<QuicArenaScopedPtr<TestObject>> objects;
  QuicIntervalSet<uintptr_t> used;
  for (size_t i = 0; i < 1024 / kMaxAlign; ++i) {
    QuicArenaScopedPtr<TestObject> ptr = arena.New<TestObject>();
    EXPECT_TRUE(ptr.is_from_arena());

    uintptr_t begin = reinterpret_cast<uintptr_t>(ptr.get());
    uintptr_t end = begin + sizeof(TestObject);
    EXPECT_FALSE(used.Contains(begin));
    EXPECT_FALSE(used.Contains(end - 1));
    used.Add(begin, end);
  }
}

}  // namespace
}  // namespace gfe_quic
