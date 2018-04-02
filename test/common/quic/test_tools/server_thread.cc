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

#include "test/common/quic/test_tools/server_thread.h"

#include "common/quic/core/quic_dispatcher.h"
#include "common/quic/platform/api/quic_containers.h"
#include "test/common/quic/test_tools/crypto_test_utils.h"
#include "test/common/quic/test_tools/quic_server_peer.h"

namespace gfe_quic {
namespace test {

ServerThread::ServerThread(QuicServer* server, const QuicSocketAddress& address)
    : server_(server), address_(address), port_(0), initialized_(false) {
  SetJoinable(true);
}

ServerThread::~ServerThread() {}

void ServerThread::Initialize() {
  if (initialized_) {
    return;
  }

  server_->CreateUDPSocketAndListen(address_);

  QuicWriterMutexLock lock(&port_lock_);
  port_ = server_->port();

  initialized_ = true;
}

void ServerThread::Run() {
  if (!initialized_) {
    Initialize();
  }

  while (!quit_.HasBeenNotified()) {
    if (pause_.HasBeenNotified() && !resume_.HasBeenNotified()) {
      paused_.Notify();
      resume_.WaitForNotification();
    }
    server_->WaitForEvents();
    ExecuteScheduledActions();
    MaybeNotifyOfHandshakeConfirmation();
  }

  server_->Shutdown();
}

int ServerThread::GetPort() {
  QuicReaderMutexLock lock(&port_lock_);
  int rc = port_;
  return rc;
}

void ServerThread::Schedule(std::function<void()> action) {
  DCHECK(!quit_.HasBeenNotified());
  QuicWriterMutexLock lock(&scheduled_actions_lock_);
  scheduled_actions_.push_back(std::move(action));
}

void ServerThread::WaitForCryptoHandshakeConfirmed() {
  confirmed_.WaitForNotification();
}

void ServerThread::Pause() {
  DCHECK(!pause_.HasBeenNotified());
  pause_.Notify();
  paused_.WaitForNotification();
}

void ServerThread::Resume() {
  DCHECK(!resume_.HasBeenNotified());
  DCHECK(pause_.HasBeenNotified());
  resume_.Notify();
}

void ServerThread::Quit() {
  if (pause_.HasBeenNotified() && !resume_.HasBeenNotified()) {
    resume_.Notify();
  }
  if (!quit_.HasBeenNotified()) {
    quit_.Notify();
  }
}

void ServerThread::MaybeNotifyOfHandshakeConfirmation() {
  if (confirmed_.HasBeenNotified()) {
    // Only notify once.
    return;
  }
  QuicDispatcher* dispatcher = QuicServerPeer::GetDispatcher(server());
  if (dispatcher->session_map().empty()) {
    // Wait for a session to be created.
    return;
  }
  QuicSession* session = dispatcher->session_map().begin()->second.get();
  if (session->IsCryptoHandshakeConfirmed()) {
    confirmed_.Notify();
  }
}

void ServerThread::ExecuteScheduledActions() {
  QuicDeque<std::function<void()>> actions;
  {
    QuicWriterMutexLock lock(&scheduled_actions_lock_);
    actions.swap(scheduled_actions_);
  }
  while (!actions.empty()) {
    actions.front()();
    actions.pop_front();
  }
}

}  // namespace test
}  // namespace gfe_quic
