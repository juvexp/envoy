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

#ifndef GFE_QUIC_HTTP_ENDPOINT_QUIC_HTTP_SETTINGS_H_
#define GFE_QUIC_HTTP_ENDPOINT_QUIC_HTTP_SETTINGS_H_

// QuicHttpSettings and QuicHttpSettingsListener support tracking the values of
// HTTP/2 settings' values. The settings are specified here:
// http://httpwg.org/specs/rfc7540.html#SettingValues

#include <cstdint>

#include "base/logging.h"
#include "common/quic/http/quic_http_constants.h"
#include "common/quic/http/quic_http_structures.h"

namespace gfe_quic {

// QuicHttpSettingsListener is notified when a setting in an QuicHttpSettings
// instance is changed, or when two QuicHttpSettings instances are compared
// and found to be different.
class QuicHttpSettingsListener {
 public:
  virtual ~QuicHttpSettingsListener();

  // The value provided to an QuicHttpSettings was invalid for the parameter
  // (e.g. ENABLE_PUSH with a value of 3), and not applied to that
  // QuicHttpSettings instance.
  virtual void OnSettingsError(QuicHttpSettingsParameter parameter,
                               uint32_t value) = 0;

  // The Header Table Size has changed from old_value to new_value.
  virtual uint32_t OnChangeHeaderTableSize(uint32_t old_value,
                                           uint32_t new_value) = 0;

  // Enable Push has changed from !new_value to new_value.
  virtual bool OnChangeEnablePush(bool new_value) = 0;

  // The Max Concurrent Streams has changed to new_value, either from unset
  // (i.e. has_old_value==false) or from old_value.
  virtual uint32_t OnChangeMaxConcurrentStreams(bool has_old_value,
                                                uint32_t old_value,
                                                uint32_t new_value) = 0;

  // The Initialize Window Size has changed from old_value to new_value, which
  // has already been validated (i.e. less than 2^31) at the connection level,
  // but not at the stream level; see the RFC for details:
  //   http://httpwg.org/specs/rfc7540.html#rfc.section.6.9.2
  virtual uint32_t OnChangeInitialWindowSize(uint32_t old_value,
                                             uint32_t new_value) = 0;

  // The Max Frame Size has changed from old_value to new_value, which
  // has been validated (i.e. 2^14 <= new_value AND new_value < 2^24).
  virtual uint32_t OnChangeMaxFrameSize(uint32_t old_value,
                                        uint32_t new_value) = 0;

  // The Max Header List Size has changed to new_value, either from unset
  // (i.e. has_old_value==false) or from old_value.
  virtual uint32_t OnChangeMaxHeaderListSize(bool has_old_value,
                                             uint32_t old_value,
                                             uint32_t new_value) = 0;
};

// Stores a value for each supported parameter (i.e. those defined in HTTP/2
// and any desired extensions). An endpoint might have one instance representing
// the settings most recently received from its peer, and might have several
// instances representing settings sent to the peer, but not yet acknowledged,
// plus one representing the last acknowledged settings, starting with those the
// RFC7540 states are the default values.
//
// In the comments below, section numbers refer to RFC7540.
//
// For all of the setter methods, opt_listener is an optional listener (i.e.
// nullptr is an acceptable value); if a listener is provided, and the value
// provided is not rejected and represents a change from the previous value,
// including having been unset, then the listener is notified of the change.
// If instead the value is rejected, and a listener is provided, then the
// listener is notified of the rejection (i.e. OnSettingsError is called).
class QuicHttpSettings {
 public:
  // Sets the fields to the default values.
  QuicHttpSettings();
  // Copies the values from other.
  QuicHttpSettings(const QuicHttpSettings& other) = default;
  ~QuicHttpSettings();

  // For each settings parameter, IFF the value has changed between old_settings
  // and this instance (assumed to be the new settings), announce the change to
  // the listener.
  void AnnounceChanges(const QuicHttpSettings& old_settings,
                       QuicHttpSettingsListener* listener) const;

  // Returns true unless the value MUST be rejected; for example, the
  // ENABLE_PUSH parameter MUST have a value of 0 or 1, any other value is
  // a connection-level protocol error. If the parameter is known, and if the
  // value is valid and different from the existing value, then the
  bool set_parameter(QuicHttpSettingsParameter parameter,
                     uint32_t value,
                     QuicHttpSettingsListener* opt_listener);

  bool set_parameter(const QuicHttpSettingFields& setting_fields,
                     QuicHttpSettingsListener* opt_listener);

  // Header Table Size allows the sender to inform the remote endpoint of the
  // maximum size of the header compression table used to decode header blocks,
  // in octets. The HPQUIC_HTTP_ACK encoder can select any size equal to or less
  // than this value by sending a Dynamic Table Size Update in an
  // HPQUIC_HTTP_ACK block. The initial value is 4,096 octets. [RFC7540]
  uint32_t header_table_size() const { return header_table_size_; }

  // Set the header table size to the provided value; all uint32 values are
  // acceptable.
  void set_header_table_size(uint32_t header_table_size,
                             QuicHttpSettingsListener* opt_listener);

  // This setting can be used to disable server push (Section 8.2). An endpoint
  // MUST NOT send a PUSH_PROMISE frame if it receives this parameter set to a
  // value of 0. An endpoint that has both set this parameter to 0 and had it
  // acknowledged MUST treat the receipt of a PUSH_PROMISE frame as a connection
  // error (Section 5.4.1) of type PROTOCOL_ERROR.
  //
  // The initial value is 1, which indicates that server push is permitted. Any
  // value other than 0 or 1 MUST be treated as a connection error (Section
  // 5.4.1) of type PROTOCOL_ERROR. [RFC7540]
  bool enable_push() const { return enable_push_; }

  // Set enable push to the provided value.
  void set_enable_push(bool enable_push,
                       QuicHttpSettingsListener* opt_listener);

  // Indicates the maximum number of concurrent streams that the sender will
  // allow. This limit is directional: it applies to the number of streams that
  // the sender permits the receiver to create. Initially, there is no limit to
  // this value. It is recommended that this value be no smaller than 100, so
  // as to not unnecessarily limit parallelism.
  //
  // A value of 0 for MAX_CONCURRENT_STREAMS SHOULD NOT be treated as
  // special by endpoints. A zero value does prevent the creation of new
  // streams; however, this can also happen for any limit that is exhausted with
  // active streams. Servers SHOULD only set a zero value for short durations;
  // if a server does not wish to accept requests, closing the connection is
  // more appropriate. [RFC7540]
  //
  // Initially has_max_concurrent_streams() will return false, indicating that
  // there is no limit to the number of concurrent connections. Once a value is
  // set, the setting is limited from then on (i.e. there is no way to return
  // to unlimited), and so has_max_concurrent_streams() returns true.
  uint32_t max_concurrent_streams() const {
    DCHECK(has_max_concurrent_streams_);
    return max_concurrent_streams_;
  }

  // Has a value been set, replacing the default (unlimited) setting?
  bool has_max_concurrent_streams() const {
    return has_max_concurrent_streams_;
  }

  // Set the max concurrent streams to the provided value; all uint32 values are
  // acceptable.
  void set_max_concurrent_streams(uint32_t max_concurrent_streams,
                                  QuicHttpSettingsListener* opt_listener);

  // Indicates the sender's initial window size (in octets) for stream-level
  // flow control. The initial value is 2^16-1 (65,535) octets.
  //
  // This setting affects the window size of all streams (see Section 6.9.2).
  //
  // Values above the maximum flow-control window size of 2^31-1 MUST be treated
  // as a connection error (Section 5.4.1) of type FLOW_CONTROL_ERROR. [RFC7540]
  uint32_t initial_window_size() const { return initial_window_size_; }

  // Set initial window size and returns true IFF the provided value is valid,
  // else does not modify the value and returns false.
  bool set_initial_window_size(uint32_t initial_window_size,
                               QuicHttpSettingsListener* opt_listener);

  // Indicates the size of the largest frame payload that the sender is willing
  // to receive, in octets.
  //
  // The initial value is 2^14 (16,384) octets. The value advertised by an
  // endpoint MUST be between this initial value and the maximum allowed frame
  // size (2^24-1 or 16,777,215 octets), inclusive. Values outside this range
  // MUST be treated as a connection error (Section 5.4.1) of type
  // PROTOCOL_ERROR. [RFC7540]
  uint32_t max_frame_size() const { return max_frame_size_; }

  // Sets the max frame size and returns true IFF the provided value is valid,
  // else does not modify the value and returns false.
  bool set_max_frame_size(uint32_t max_frame_size,
                          QuicHttpSettingsListener* opt_listener);

  // This advisory setting informs a peer of the maximum size of header list
  // that the sender is prepared to accept, in octets. The value is based on the
  // uncompressed size of header fields, including the length of the name and
  // value in octets plus an overhead of 32 octets for each header field.
  //
  // For any given request, a lower limit than what is advertised MAY be
  // enforced. The initial value of this setting is unlimited. [RFC7540]
  //
  // Do not call max_header_list_size() unless has_max_header_list_size()
  // returns true.
  uint32_t max_header_list_size() const {
    DCHECK(has_max_header_list_size_);
    return max_header_list_size_;
  }

  // Initially has_max_header_list_size() will return false, indicating that
  // there is no limit to the size of an HPQUIC_HTTP_ACK block. Once a value is
  // set, the setting is limited from then on (i.e. there is no way to return to
  // unlimited), and so has_max_header_list_size() returns true.
  bool has_max_header_list_size() const { return has_max_header_list_size_; }

  // Sets the max header list size; all uint32 values are acceptable.
  void set_max_header_list_size(uint32_t max_header_list_size,
                                QuicHttpSettingsListener* opt_listener);

 private:
  uint32_t header_table_size_;
  uint32_t max_concurrent_streams_;
  uint32_t initial_window_size_;
  uint32_t max_frame_size_;
  uint32_t max_header_list_size_;
  bool enable_push_;
  bool has_max_concurrent_streams_;
  bool has_max_header_list_size_;
};

}  // namespace gfe_quic

#endif  // GFE_QUIC_HTTP_ENDPOINT_QUIC_HTTP_SETTINGS_H_
