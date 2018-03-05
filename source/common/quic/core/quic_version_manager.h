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

#ifndef GFE_QUIC_CORE_QUIC_VERSION_MANAGER_H_
#define GFE_QUIC_CORE_QUIC_VERSION_MANAGER_H_

#include "common/quic/core/quic_versions.h"
#include "common/quic/platform/api/quic_export.h"

namespace gfe_quic {

// Used to generate filtered supported versions based on flags.
class QUIC_EXPORT_PRIVATE QuicVersionManager {
 public:
  explicit QuicVersionManager(ParsedQuicVersionVector supported_versions);
  virtual ~QuicVersionManager();

  // Returns currently supported QUIC versions.
  // TODO: Remove this method once it is unused.
  const QuicTransportVersionVector& GetSupportedTransportVersions();

  // Returns currently supported QUIC versions.
  const ParsedQuicVersionVector& GetSupportedVersions();

 protected:
  // Maybe refilter filtered_supported_versions_ based on flags.
  void MaybeRefilterSupportedVersions();

  // Refilters filtered_supported_versions_.
  virtual void RefilterSupportedVersions();

  const QuicTransportVersionVector& filtered_supported_versions() const {
    return filtered_transport_versions_;
  }

 private:
  // FLAGS_quic_enable_version_99
  bool enable_version_99_;
  // FLAGS_quic_enable_version_43
  bool enable_version_43_;
  // FLAGS_gfe2_reloadable_flag_quic_enable_version_42_2.
  bool enable_version_42_;
  // FLAGS_gfe2_reloadable_flag_quic_disable_version_41
  bool disable_version_41_;
  // FLAGS_gfe2_reloadable_flag_quic_disable_version_38
  bool disable_version_38_;
  // FLAGS_gfe2_reloadable_flag_quic_disable_version_37
  bool disable_version_37_;
  // The list of versions that may be supported.
  ParsedQuicVersionVector allowed_supported_versions_;
  // This vector contains QUIC versions which are currently supported based on
  // flags.
  ParsedQuicVersionVector filtered_supported_versions_;
  // This vector contains the transport versions from
  // |filtered_supported_versions_|. No guarantees are made that the same
  // transport version isn't repeated.
  QuicTransportVersionVector filtered_transport_versions_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_CORE_QUIC_VERSION_MANAGER_H_
