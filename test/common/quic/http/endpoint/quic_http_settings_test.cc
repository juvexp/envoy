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

// Tests of QuicHttpSettings.

#include "test/common/quic/http/tools/quic_http_gunit_helpers.h"
#include "testing/base/public/gmock.h"
#include "testing/base/public/gunit.h"

using ::testing::AssertionResult;
using ::testing::AssertionSuccess;
using ::testing::StrictMock;

namespace gfe_quic {
namespace test {
namespace {

class MockQuicHttpSettingsListener : public QuicHttpSettingsListener {
 public:
  MOCK_METHOD2(OnSettingsError,
               void(QuicHttpSettingsParameter parameter, uint32_t value));
  MOCK_METHOD2(OnChangeHeaderTableSize,
               uint32_t(uint32_t old_value, uint32_t new_value));
  MOCK_METHOD3(OnChangeMaxConcurrentStreams,
               uint32_t(bool has_old_value,
                        uint32_t old_value,
                        uint32_t new_value));
  MOCK_METHOD2(OnChangeInitialWindowSize,
               uint32_t(uint32_t old_value, uint32_t new_value));
  MOCK_METHOD2(OnChangeMaxFrameSize,
               uint32_t(uint32_t old_value, uint32_t new_value));
  MOCK_METHOD3(OnChangeMaxHeaderListSize,
               uint32_t(bool has_old_value,
                        uint32_t old_value,
                        uint32_t new_value));
  MOCK_METHOD1(OnChangeEnablePush, bool(bool new_value));
};

AssertionResult VerifyRejected(QuicHttpSettingsParameter param,
                               uint32_t value,
                               QuicHttpSettings* settings) {
  // Without listener, without QuicHttpSettingFields.
  VERIFY_FALSE(settings->set_parameter(param, value, nullptr));
  {
    // With listener, without QuicHttpSettingFields.
    StrictMock<MockQuicHttpSettingsListener> mock_listener;
    EXPECT_CALL(mock_listener, OnSettingsError(param, value));
    VERIFY_FALSE(settings->set_parameter(param, value, &mock_listener));
  }

  // Without listener, with QuicHttpSettingFields.
  QuicHttpSettingFields sf{param, value};
  VERIFY_FALSE(settings->set_parameter(sf, nullptr));
  {
    // With listener, with QuicHttpSettingFields.
    StrictMock<MockQuicHttpSettingsListener> mock_listener;
    EXPECT_CALL(mock_listener, OnSettingsError(param, value));
    VERIFY_FALSE(settings->set_parameter(sf, &mock_listener));
  }
  return AssertionSuccess();
}

// Apply a setting several ways confirming that all of them modify the
// QuicHttpSettings instance (or a copy) as expected. We need the copy because
// we expect that multiple calls will make no change.
// The caller should specify:
//     EXPECT_CALL(*listener, OnChangeFooBar(...)).Times(2).
AssertionResult VerifyAccepted(QuicHttpSettingsParameter param,
                               uint32_t value,
                               QuicHttpSettings* settings,
                               QuicHttpSettingsListener* listener) {
  QuicHttpSettings copy_a(*settings), copy_b(*settings), copy_c(*settings);

  // Without listener, without QuicHttpSettingFields.
  VERIFY_TRUE(settings->set_parameter(param, value, nullptr));

  // With listener, without QuicHttpSettingFields.
  VERIFY_TRUE(copy_a.set_parameter(param, value, listener));

  // Without listener, with QuicHttpSettingFields.
  QuicHttpSettingFields sf{param, value};
  VERIFY_TRUE(copy_b.set_parameter(sf, nullptr));

  // With listener, with QuicHttpSettingFields.
  VERIFY_TRUE(copy_c.set_parameter(sf, listener));

  // Confirm that there is no difference between the copies (the strict
  // listener will complain if it gets an callbacks, which would indicate
  // a difference).
  StrictMock<MockQuicHttpSettingsListener> strict_listener;
  settings->AnnounceChanges(copy_a, &strict_listener);
  settings->AnnounceChanges(copy_b, &strict_listener);
  settings->AnnounceChanges(copy_c, &strict_listener);

  return AssertionSuccess();
}

AssertionResult VerifyHasDefaults(const QuicHttpSettings& settings) {
  // Values are from the RFC: http://httpwg.org/specs/rfc7540.html#SETTINGS
  VERIFY_EQ(settings.header_table_size(), 4096);
  VERIFY_EQ(settings.enable_push(), true);
  VERIFY_EQ(settings.has_max_concurrent_streams(), false);
  VERIFY_EQ(settings.initial_window_size(), 65535);
  VERIFY_EQ(settings.max_frame_size(), 16384);
  VERIFY_EQ(settings.has_max_header_list_size(), false);
  return AssertionSuccess();
}

TEST(QuicHttpSettingsTest, HasExpectedDefaultValues) {
  QuicHttpSettings a;
  EXPECT_TRUE(VerifyHasDefaults(a));

  // a should be the same as itself, so no differences should be reported.
  StrictMock<MockQuicHttpSettingsListener> mock_listener;
  a.AnnounceChanges(a, &mock_listener);

  // A copy of a should also have the default values.
  QuicHttpSettings copy(a);
  EXPECT_TRUE(VerifyHasDefaults(copy));
  a.AnnounceChanges(copy, &mock_listener);

  // Another default constructed settings object should the same as a, so no
  // differences should be reported.
  QuicHttpSettings b;
  EXPECT_TRUE(VerifyHasDefaults(b));
  a.AnnounceChanges(b, &mock_listener);
}

TEST(QuicHttpSettingsTest, RejectInvalidValues) {
  QuicHttpSettings a;

  EXPECT_TRUE(VerifyRejected(QuicHttpSettingsParameter::ENABLE_PUSH, 2, &a));

  EXPECT_TRUE(VerifyRejected(QuicHttpSettingsParameter::INITIAL_WINDOW_SIZE,
                             0x80000000, &a));

  // Max Frame Size has both low and high limits.
  EXPECT_TRUE(
      VerifyRejected(QuicHttpSettingsParameter::MAX_FRAME_SIZE, 16383, &a));
  EXPECT_TRUE(
      VerifyRejected(QuicHttpSettingsParameter::MAX_FRAME_SIZE, 16777216, &a));
}

TEST(QuicHttpSettingsTest, AcceptsValidValues) {
  const QuicHttpSettings with_defaults;
  QuicHttpSettings a;

  // Change HEADER_TABLE_SIZE.
  const uint32_t new_header_table_size = 1000;
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size))
        .Times(2);
    EXPECT_TRUE(VerifyAccepted(QuicHttpSettingsParameter::HEADER_TABLE_SIZE,
                               new_header_table_size, &a, &listener));
  }
  // Confirm HEADER_TABLE_SIZE.
  EXPECT_EQ(new_header_table_size, a.header_table_size());
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size));
    a.AnnounceChanges(with_defaults, &listener);
  }

  // Change ENABLE_PUSH.
  const bool new_enable_push = false;
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener, OnChangeEnablePush(new_enable_push)).Times(2);
    EXPECT_TRUE(VerifyAccepted(QuicHttpSettingsParameter::ENABLE_PUSH,
                               new_enable_push ? 1 : 0, &a, &listener));
  }
  // Confirm ENABLE_PUSH.
  EXPECT_EQ(new_enable_push, a.enable_push());
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    // Expect all OnChange* callbacks that are appropriate.
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size));
    EXPECT_CALL(listener, OnChangeEnablePush(new_enable_push));
    a.AnnounceChanges(with_defaults, &listener);
  }

  // Change MAX_CONCURRENT_STREAMS from not limited to 0.
  const uint32_t new_max_concurrent_streams = 0;
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener, OnChangeMaxConcurrentStreams(
                              false, 0, new_max_concurrent_streams))
        .Times(2);
    EXPECT_TRUE(
        VerifyAccepted(QuicHttpSettingsParameter::MAX_CONCURRENT_STREAMS,
                       new_max_concurrent_streams, &a, &listener));
  }
  // Confirm MAX_CONCURRENT_STREAMS.
  EXPECT_EQ(new_max_concurrent_streams, a.max_concurrent_streams());
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    // Expect all OnChange* callbacks that are appropriate.
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size));
    EXPECT_CALL(listener, OnChangeEnablePush(new_enable_push));
    EXPECT_CALL(listener, OnChangeMaxConcurrentStreams(
                              false, 0, new_max_concurrent_streams));
    a.AnnounceChanges(with_defaults, &listener);
  }

  // Change MAX_CONCURRENT_STREAMS from 0 to 10.
  const uint32_t newest_max_concurrent_streams = 10;
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener,
                OnChangeMaxConcurrentStreams(true, new_max_concurrent_streams,
                                             newest_max_concurrent_streams))
        .Times(2);
    EXPECT_TRUE(
        VerifyAccepted(QuicHttpSettingsParameter::MAX_CONCURRENT_STREAMS,
                       newest_max_concurrent_streams, &a, &listener));
  }
  // Confirm MAX_CONCURRENT_STREAMS.
  EXPECT_EQ(newest_max_concurrent_streams, a.max_concurrent_streams());
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    // Expect all OnChange* callbacks that are appropriate.
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size));
    EXPECT_CALL(listener, OnChangeEnablePush(new_enable_push));
    EXPECT_CALL(listener, OnChangeMaxConcurrentStreams(
                              false, 0, newest_max_concurrent_streams));
    a.AnnounceChanges(with_defaults, &listener);
  }

  // Change INITIAL_WINDOW_SIZE.
  const uint32_t new_initial_window_size = 10000;
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener,
                OnChangeInitialWindowSize(with_defaults.initial_window_size(),
                                          new_initial_window_size))
        .Times(2);
    EXPECT_TRUE(VerifyAccepted(QuicHttpSettingsParameter::INITIAL_WINDOW_SIZE,
                               new_initial_window_size, &a, &listener));
  }
  // Confirm INITIAL_WINDOW_SIZE.
  EXPECT_EQ(new_initial_window_size, a.initial_window_size());
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    // Expect all OnChange* callbacks that are appropriate.
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size));
    EXPECT_CALL(listener, OnChangeEnablePush(new_enable_push));
    EXPECT_CALL(listener, OnChangeMaxConcurrentStreams(
                              false, 0, newest_max_concurrent_streams));
    EXPECT_CALL(listener,
                OnChangeInitialWindowSize(with_defaults.initial_window_size(),
                                          new_initial_window_size));
    a.AnnounceChanges(with_defaults, &listener);
  }

  // Change MAX_FRAME_SIZE.
  const uint32_t new_max_frame_size = 100000;
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener, OnChangeMaxFrameSize(with_defaults.max_frame_size(),
                                               new_max_frame_size))
        .Times(2);
    EXPECT_TRUE(VerifyAccepted(QuicHttpSettingsParameter::MAX_FRAME_SIZE,
                               new_max_frame_size, &a, &listener));
  }
  // Confirm MAX_FRAME_SIZE.
  EXPECT_EQ(new_max_frame_size, a.max_frame_size());
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    // Expect all OnChange* callbacks that are appropriate.
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size));
    EXPECT_CALL(listener, OnChangeEnablePush(new_enable_push));
    EXPECT_CALL(listener, OnChangeMaxConcurrentStreams(
                              false, 0, newest_max_concurrent_streams));
    EXPECT_CALL(listener,
                OnChangeInitialWindowSize(with_defaults.initial_window_size(),
                                          new_initial_window_size));
    EXPECT_CALL(listener, OnChangeMaxFrameSize(with_defaults.max_frame_size(),
                                               new_max_frame_size));
    a.AnnounceChanges(with_defaults, &listener);
  }

  // Change MAX_HEADER_LIST_SIZE from not limited to 10000.
  const uint32_t new_max_header_list_size = 10000;
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener,
                OnChangeMaxHeaderListSize(false, 0, new_max_header_list_size))
        .Times(2);
    EXPECT_TRUE(VerifyAccepted(QuicHttpSettingsParameter::MAX_HEADER_LIST_SIZE,
                               new_max_header_list_size, &a, &listener));
  }
  // Confirm MAX_HEADER_LIST_SIZE.
  EXPECT_EQ(new_max_header_list_size, a.max_header_list_size());
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    // Expect all OnChange* callbacks that are appropriate.
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size));
    EXPECT_CALL(listener, OnChangeEnablePush(new_enable_push));
    EXPECT_CALL(listener, OnChangeMaxConcurrentStreams(
                              false, 0, newest_max_concurrent_streams));
    EXPECT_CALL(listener,
                OnChangeInitialWindowSize(with_defaults.initial_window_size(),
                                          new_initial_window_size));
    EXPECT_CALL(listener, OnChangeMaxFrameSize(with_defaults.max_frame_size(),
                                               new_max_frame_size));
    EXPECT_CALL(listener,
                OnChangeMaxHeaderListSize(false, 0, new_max_header_list_size));
    a.AnnounceChanges(with_defaults, &listener);
  }

  // Change MAX_HEADER_LIST_SIZE from 10000 to 0.
  const uint32_t newest_max_header_list_size = 0;
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    EXPECT_CALL(listener,
                OnChangeMaxHeaderListSize(true, new_max_header_list_size,
                                          newest_max_header_list_size))
        .Times(2);
    EXPECT_TRUE(VerifyAccepted(QuicHttpSettingsParameter::MAX_HEADER_LIST_SIZE,
                               newest_max_header_list_size, &a, &listener));
  }
  // Confirm MAX_HEADER_LIST_SIZE.
  EXPECT_EQ(newest_max_header_list_size, a.max_header_list_size());
  {
    StrictMock<MockQuicHttpSettingsListener> listener;
    // Expect all OnChange* callbacks that are appropriate.
    EXPECT_CALL(listener,
                OnChangeHeaderTableSize(with_defaults.header_table_size(),
                                        new_header_table_size));
    EXPECT_CALL(listener, OnChangeEnablePush(new_enable_push));
    EXPECT_CALL(listener, OnChangeMaxConcurrentStreams(
                              false, 0, newest_max_concurrent_streams));
    EXPECT_CALL(listener,
                OnChangeInitialWindowSize(with_defaults.initial_window_size(),
                                          new_initial_window_size));
    EXPECT_CALL(listener, OnChangeMaxFrameSize(with_defaults.max_frame_size(),
                                               new_max_frame_size));
    EXPECT_CALL(listener, OnChangeMaxHeaderListSize(
                              false, 0, newest_max_header_list_size));
    a.AnnounceChanges(with_defaults, &listener);
  }
}

}  // namespace
}  // namespace test
}  // namespace gfe_quic
