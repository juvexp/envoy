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

#include "common/quic/platform/api/quic_mutex.h"

namespace gfe_quic {

void QuicMutex::WriterLock() {
  impl_.WriterLock();
}

void QuicMutex::WriterUnlock() {
  impl_.WriterUnlock();
}

void QuicMutex::ReaderLock() {
  impl_.ReaderLock();
}

void QuicMutex::ReaderUnlock() {
  impl_.ReaderUnlock();
}

void QuicMutex::AssertReaderHeld() const {
  impl_.AssertReaderHeld();
}

QuicReaderMutexLock::QuicReaderMutexLock(QuicMutex* lock) : lock_(lock) {
  lock->ReaderLock();
}

QuicReaderMutexLock::~QuicReaderMutexLock() {
  lock_->ReaderUnlock();
}

QuicWriterMutexLock::QuicWriterMutexLock(QuicMutex* lock) : lock_(lock) {
  lock->WriterLock();
}

QuicWriterMutexLock::~QuicWriterMutexLock() {
  lock_->WriterUnlock();
}

}  // namespace gfe_quic
