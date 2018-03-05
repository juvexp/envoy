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

#include "common/quic/core/crypto/quic_tls_adapter.h"

#include "common/quic/platform/api/quic_logging.h"
#include "common/quic/platform/api/quic_string.h"
#include "common/quic/platform/api/quic_text_utils.h"

namespace gfe_quic {

const BIO_METHOD QuicTlsAdapter::kBIOMethod = {
    0,        // type
    nullptr,  // name
    QuicTlsAdapter::BIOWriteWrapper,
    QuicTlsAdapter::BIOReadWrapper,
    nullptr,  // puts
    nullptr,  // gets
    QuicTlsAdapter::BIOCtrlWrapper,
    nullptr,  // create
    nullptr,  // destroy
    nullptr,  // callback_ctrl
};

// static
QuicTlsAdapter* QuicTlsAdapter::GetAdapter(BIO* bio) {
  DCHECK_EQ(&kBIOMethod, bio->method);
  QuicTlsAdapter* adapter = reinterpret_cast<QuicTlsAdapter*>(bio->ptr);
  if (adapter)
    DCHECK_EQ(bio, adapter->bio());
  return adapter;
}

// static
int QuicTlsAdapter::BIOReadWrapper(BIO* bio, char* out, int len) {
  QuicTlsAdapter* adapter = GetAdapter(bio);
  if (!adapter)
    return -1;
  return adapter->Read(out, len);
}

// static
int QuicTlsAdapter::BIOWriteWrapper(BIO* bio, const char* in, int len) {
  QuicTlsAdapter* adapter = GetAdapter(bio);
  if (!adapter)
    return -1;
  return adapter->Write(in, len);
}

// static
// NOLINTNEXTLINE
long QuicTlsAdapter::BIOCtrlWrapper(BIO* bio, int cmd, long larg, void* parg) {
  QuicTlsAdapter* adapter = GetAdapter(bio);
  if (!adapter)
    return 0;
  switch (cmd) {
    // The only control request sent by the TLS stack is from BIO_flush. Any
    // other values of |cmd| would indicate some sort of programming error.
    case BIO_CTRL_FLUSH:
      adapter->Flush();
      return 1;
  }
  QUIC_NOTREACHED();
  return 0;
}

QuicTlsAdapter::QuicTlsAdapter(Visitor* visitor)
    : visitor_(visitor), bio_(BIO_new(&kBIOMethod)) {
  bio_->ptr = this;
  bio_->init = 1;
}

QuicTlsAdapter::~QuicTlsAdapter() {}

QuicErrorCode QuicTlsAdapter::error() const {
  // QuicTlsAdapter passes messages received from ProcessInput straight through
  // to the TLS stack (via the BIO) and does not parse the messages at all.
  // ProcessInput never fails, so there is never an error to provide.
  return QUIC_NO_ERROR;
}

const QuicString& QuicTlsAdapter::error_detail() const {
  return error_detail_;
}

bool QuicTlsAdapter::ProcessInput(QuicStringPiece input,
                                  Perspective perspective) {
  read_buffer_.append(input.data(), input.length());
  visitor_->OnDataAvailableForBIO();
  return true;
}

size_t QuicTlsAdapter::InputBytesRemaining() const {
  return read_buffer_.length();
}

int QuicTlsAdapter::Read(char* out, int len) {
  if (len < 0) {
    return -1;
  }
  if (read_buffer_.empty()) {
    BIO_set_retry_read(bio());
    return -1;
  }
  if (len >= static_cast<int>(read_buffer_.length())) {
    len = read_buffer_.length();
  }
  memcpy(out, read_buffer_.data(), len);
  read_buffer_.erase(0, len);
  QUIC_LOG(INFO) << "BIO_read: reading " << len << " bytes:\n";
  return len;
}

int QuicTlsAdapter::Write(const char* in, int len) {
  if (len < 0) {
    return -1;
  }
  QUIC_LOG(INFO) << "BIO_write: writing " << len << " bytes:\n";
  write_buffer_.append(in, len);
  return len;
}

void QuicTlsAdapter::Flush() {
  QUIC_LOG(INFO) << "BIO_flush: flushing " << write_buffer_.size() << " bytes";
  visitor_->OnDataReceivedFromBIO(write_buffer_);
  write_buffer_.clear();
}

}  // namespace gfe_quic
