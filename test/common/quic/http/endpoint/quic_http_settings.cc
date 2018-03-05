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

#include "test/common/quic/http/endpoint/quic_http_settings.h"

namespace gfe_quic {

QuicHttpSettingsListener::~QuicHttpSettingsListener() {}

QuicHttpSettings::QuicHttpSettings()
    : header_table_size_(QuicHttpSettingsInfo::DefaultHeaderTableSize()),
      max_concurrent_streams_(0),
      initial_window_size_(QuicHttpSettingsInfo::DefaultInitialWindowSize()),
      max_frame_size_(QuicHttpSettingsInfo::DefaultMaxFrameSize()),
      max_header_list_size_(0),
      enable_push_(QuicHttpSettingsInfo::DefaultEnablePush()),
      has_max_concurrent_streams_(false),
      has_max_header_list_size_(false) {}

QuicHttpSettings::~QuicHttpSettings() {}

void QuicHttpSettings::AnnounceChanges(
    const QuicHttpSettings& old_settings,
    QuicHttpSettingsListener* listener) const {
  if (old_settings.header_table_size() != header_table_size_) {
    listener->OnChangeHeaderTableSize(old_settings.header_table_size(),
                                      header_table_size_);
  }

  if (old_settings.enable_push() != enable_push_) {
    listener->OnChangeEnablePush(enable_push_);
  }

  if (has_max_concurrent_streams_) {
    if (!old_settings.has_max_concurrent_streams()) {
      listener->OnChangeMaxConcurrentStreams(false, 0, max_concurrent_streams_);
    } else if (old_settings.max_concurrent_streams() !=
               max_concurrent_streams_) {
      listener->OnChangeMaxConcurrentStreams(
          true, old_settings.max_concurrent_streams(), max_concurrent_streams_);
    }
  } else {
    // Can't transition from having a Max Concurrent Streams value to not
    // having a value for the setting.
    DCHECK(!old_settings.has_max_concurrent_streams());
  }

  if (old_settings.initial_window_size() != initial_window_size_) {
    listener->OnChangeInitialWindowSize(old_settings.initial_window_size(),
                                        initial_window_size_);
  }

  if (old_settings.max_frame_size() != max_frame_size_) {
    listener->OnChangeMaxFrameSize(old_settings.max_frame_size(),
                                   max_frame_size_);
  }

  if (has_max_header_list_size_) {
    if (!old_settings.has_max_header_list_size()) {
      listener->OnChangeMaxHeaderListSize(false, 0, max_header_list_size_);
    } else if (old_settings.max_header_list_size() != max_header_list_size_) {
      listener->OnChangeMaxHeaderListSize(
          true, old_settings.max_header_list_size(), max_header_list_size_);
    }
  } else {
    // Can't transition from having a Max Header List Size value to not
    // having a value for the setting.
    DCHECK(!old_settings.has_max_header_list_size());
  }
}

bool QuicHttpSettings::set_parameter(QuicHttpSettingsParameter parameter,
                                     uint32_t value,
                                     QuicHttpSettingsListener* opt_listener) {
  switch (parameter) {
    case QuicHttpSettingsParameter::HEADER_TABLE_SIZE:
      set_header_table_size(value, opt_listener);
      return true;

    case QuicHttpSettingsParameter::ENABLE_PUSH:
      if (value > 1) {
        if (opt_listener != nullptr) {
          opt_listener->OnSettingsError(parameter, value);
        }
        return false;
      }
      set_enable_push(value == 1, opt_listener);
      return true;

    case QuicHttpSettingsParameter::MAX_CONCURRENT_STREAMS:
      set_max_concurrent_streams(value, opt_listener);
      return true;

    case QuicHttpSettingsParameter::INITIAL_WINDOW_SIZE:
      return set_initial_window_size(value, opt_listener);

    case QuicHttpSettingsParameter::MAX_FRAME_SIZE:
      return set_max_frame_size(value, opt_listener);

    case QuicHttpSettingsParameter::MAX_HEADER_LIST_SIZE:
      set_max_header_list_size(value, opt_listener);
      return true;
  }
  DVLOG(1) << "Ignoring unknown parameter: " << parameter << ", with value "
           << value;
  return true;
}

bool QuicHttpSettings::set_parameter(
    const QuicHttpSettingFields& setting_fields,
    QuicHttpSettingsListener* opt_listener) {
  return set_parameter(setting_fields.parameter, setting_fields.value,
                       opt_listener);
}

void QuicHttpSettings::set_header_table_size(
    uint32_t header_table_size,
    QuicHttpSettingsListener* opt_listener) {
  if (header_table_size_ != header_table_size) {
    uint32_t old_value = header_table_size_;
    header_table_size_ = header_table_size;
    if (opt_listener != nullptr) {
      opt_listener->OnChangeHeaderTableSize(old_value, header_table_size_);
    }
  }
}

void QuicHttpSettings::set_enable_push(bool enable_push,
                                       QuicHttpSettingsListener* opt_listener) {
  if (enable_push_ != enable_push) {
    enable_push_ = enable_push;
    if (opt_listener != nullptr) {
      opt_listener->OnChangeEnablePush(enable_push_);
    }
  }
}

void QuicHttpSettings::set_max_concurrent_streams(
    uint32_t max_concurrent_streams,
    QuicHttpSettingsListener* opt_listener) {
  if (!has_max_concurrent_streams_ ||
      max_concurrent_streams_ != max_concurrent_streams) {
    bool had_max_concurrent_streams = has_max_concurrent_streams_;
    uint32_t old_value = max_concurrent_streams_;
    max_concurrent_streams_ = max_concurrent_streams;
    has_max_concurrent_streams_ = true;
    if (opt_listener != nullptr) {
      opt_listener->OnChangeMaxConcurrentStreams(
          had_max_concurrent_streams, old_value, max_concurrent_streams_);
    }
  }
}

bool QuicHttpSettings::set_initial_window_size(
    uint32_t initial_window_size,
    QuicHttpSettingsListener* opt_listener) {
  // The value must be no greater than 2^31-1, else MUST be treated as a
  // connection error.
  if (initial_window_size > QuicHttpSettingsInfo::MaximumWindowSize()) {
    if (opt_listener != nullptr) {
      opt_listener->OnSettingsError(
          QuicHttpSettingsParameter::INITIAL_WINDOW_SIZE, initial_window_size);
    }
    return false;
  }
  if (initial_window_size_ != initial_window_size) {
    uint32_t old_value = initial_window_size_;
    initial_window_size_ = initial_window_size;
    if (opt_listener != nullptr) {
      opt_listener->OnChangeInitialWindowSize(old_value, initial_window_size_);
    }
  }
  return true;
}

bool QuicHttpSettings::set_max_frame_size(
    uint32_t max_frame_size,
    QuicHttpSettingsListener* opt_listener) {
  // The value MUST be between 2^14 (16,384) octets and 2^24-1 (16,777,215)
  // octets, inclusive, else MUST be treated as a connection error.
  if (QuicHttpSettingsInfo::MinimumMaxFrameSize() <= max_frame_size &&
      max_frame_size <= QuicHttpSettingsInfo::MaximumMaxFrameSize()) {
    if (max_frame_size_ != max_frame_size) {
      uint32_t old_value = max_frame_size_;
      max_frame_size_ = max_frame_size;
      if (opt_listener != nullptr) {
        opt_listener->OnChangeMaxFrameSize(old_value, max_frame_size_);
      }
    }
    return true;
  }
  if (opt_listener != nullptr) {
    opt_listener->OnSettingsError(QuicHttpSettingsParameter::MAX_FRAME_SIZE,
                                  max_frame_size);
  }
  return false;
}

void QuicHttpSettings::set_max_header_list_size(
    uint32_t max_header_list_size,
    QuicHttpSettingsListener* opt_listener) {
  if (!has_max_header_list_size_ ||
      max_header_list_size_ != max_header_list_size) {
    bool had_max_header_list_size = has_max_header_list_size_;
    uint32_t old_value = max_header_list_size_;
    max_header_list_size_ = max_header_list_size;
    has_max_header_list_size_ = true;
    if (opt_listener != nullptr) {
      opt_listener->OnChangeMaxHeaderListSize(had_max_header_list_size,
                                              old_value, max_header_list_size_);
    }
  }
}

}  // namespace gfe_quic
