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

#include "common/quic/core/quic_version_manager.h"

#include "common/quic/core/quic_versions.h"
#include "common/quic/platform/api/quic_arraysize.h"
#include "common/quic/platform/api/quic_flags.h"
#include "test/common/quic/platform/api/quic_test.h"

namespace gfe_quic {
namespace test {
namespace {

class QuicVersionManagerTest : public QuicTest {};

TEST_F(QuicVersionManagerTest, QuicVersionManager) {
  static_assert(QUIC_ARRAYSIZE(kSupportedTransportVersions) == 8u,
                "Supported versions out of sync");
  SetQuicFlag(&FLAGS_quic_enable_version_99, false);
  SetQuicReloadableFlag(quic_enable_version_43, false);
  SetQuicReloadableFlag(quic_enable_version_42_2, false);
  SetQuicReloadableFlag(quic_disable_version_41, true);
  SetQuicReloadableFlag(quic_disable_version_38, true);
  SetQuicReloadableFlag(quic_disable_version_37, true);
  QuicVersionManager manager(AllSupportedVersions());

  EXPECT_EQ(FilterSupportedTransportVersions(AllSupportedTransportVersions()),
            manager.GetSupportedTransportVersions());

  EXPECT_EQ(QuicTransportVersionVector({QUIC_VERSION_39, QUIC_VERSION_35}),
            manager.GetSupportedTransportVersions());

  SetQuicReloadableFlag(quic_disable_version_37, false);
  EXPECT_EQ(QuicTransportVersionVector(
                {QUIC_VERSION_39, QUIC_VERSION_37, QUIC_VERSION_35}),
            manager.GetSupportedTransportVersions());

  SetQuicReloadableFlag(quic_disable_version_38, false);
  EXPECT_EQ(QuicTransportVersionVector({QUIC_VERSION_39, QUIC_VERSION_38,
                                        QUIC_VERSION_37, QUIC_VERSION_35}),
            manager.GetSupportedTransportVersions());

  SetQuicReloadableFlag(quic_disable_version_41, false);
  EXPECT_EQ(QuicTransportVersionVector({QUIC_VERSION_41, QUIC_VERSION_39,
                                        QUIC_VERSION_38, QUIC_VERSION_37,
                                        QUIC_VERSION_35}),
            manager.GetSupportedTransportVersions());

  SetQuicReloadableFlag(quic_enable_version_42_2, true);
  EXPECT_EQ(QuicTransportVersionVector({QUIC_VERSION_42, QUIC_VERSION_41,
                                        QUIC_VERSION_39, QUIC_VERSION_38,
                                        QUIC_VERSION_37, QUIC_VERSION_35}),
            manager.GetSupportedTransportVersions());

  SetQuicReloadableFlag(quic_enable_version_43, true);
  EXPECT_EQ(
      QuicTransportVersionVector(
          {QUIC_VERSION_43, QUIC_VERSION_42, QUIC_VERSION_41, QUIC_VERSION_39,
           QUIC_VERSION_38, QUIC_VERSION_37, QUIC_VERSION_35}),
      manager.GetSupportedTransportVersions());

  SetQuicFlag(&FLAGS_quic_enable_version_99, true);
  EXPECT_EQ(
      QuicTransportVersionVector(
          {QUIC_VERSION_99, QUIC_VERSION_43, QUIC_VERSION_42, QUIC_VERSION_41,
           QUIC_VERSION_39, QUIC_VERSION_38, QUIC_VERSION_37, QUIC_VERSION_35}),
      manager.GetSupportedTransportVersions());

  // Ensure that all versions are now supported.
  EXPECT_EQ(FilterSupportedTransportVersions(AllSupportedTransportVersions()),
            manager.GetSupportedTransportVersions());
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
