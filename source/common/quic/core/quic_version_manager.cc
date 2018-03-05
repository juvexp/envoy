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
#include "common/quic/platform/api/quic_flags.h"

namespace gfe_quic {

QuicVersionManager::QuicVersionManager(
    ParsedQuicVersionVector supported_versions)
    : enable_version_99_(GetQuicFlag(FLAGS_quic_enable_version_99)),
      enable_version_43_(GetQuicReloadableFlag(quic_enable_version_43)),
      enable_version_42_(GetQuicReloadableFlag(quic_enable_version_42_2)),
      disable_version_41_(GetQuicReloadableFlag(quic_disable_version_41)),
      disable_version_38_(GetQuicReloadableFlag(quic_disable_version_38)),
      disable_version_37_(GetQuicReloadableFlag(quic_disable_version_37)),
      allowed_supported_versions_(std::move(supported_versions)) {
  RefilterSupportedVersions();
}

QuicVersionManager::~QuicVersionManager() {}

const QuicTransportVersionVector&
QuicVersionManager::GetSupportedTransportVersions() {
  MaybeRefilterSupportedVersions();
  return filtered_transport_versions_;
}

const ParsedQuicVersionVector& QuicVersionManager::GetSupportedVersions() {
  MaybeRefilterSupportedVersions();
  return filtered_supported_versions_;
}

void QuicVersionManager::MaybeRefilterSupportedVersions() {
  if (enable_version_99_ != GetQuicFlag(FLAGS_quic_enable_version_99) ||
      enable_version_43_ != GetQuicReloadableFlag(quic_enable_version_43) ||
      enable_version_42_ != GetQuicReloadableFlag(quic_enable_version_42_2) ||
      disable_version_41_ != GetQuicReloadableFlag(quic_disable_version_41) ||
      disable_version_38_ != GetQuicReloadableFlag(quic_disable_version_38) ||
      disable_version_37_ != GetQuicReloadableFlag(quic_disable_version_37)) {
    enable_version_99_ = GetQuicFlag(FLAGS_quic_enable_version_99);
    enable_version_43_ = GetQuicReloadableFlag(quic_enable_version_43);
    enable_version_42_ = GetQuicReloadableFlag(quic_enable_version_42_2);
    disable_version_37_ = GetQuicReloadableFlag(quic_disable_version_37);
    disable_version_38_ = GetQuicReloadableFlag(quic_disable_version_38);
    disable_version_41_ = GetQuicReloadableFlag(quic_disable_version_41);
    RefilterSupportedVersions();
  }
}

void QuicVersionManager::RefilterSupportedVersions() {
  filtered_supported_versions_ =
      FilterSupportedVersions(allowed_supported_versions_);
  filtered_transport_versions_.clear();
  for (ParsedQuicVersion version : filtered_supported_versions_) {
    filtered_transport_versions_.push_back(version.transport_version);
  }
}

}  // namespace gfe_quic
