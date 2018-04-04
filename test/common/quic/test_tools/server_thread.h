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

#ifndef GFE_QUIC_TEST_TOOLS_SERVER_THREAD_H_
#define GFE_QUIC_TEST_TOOLS_SERVER_THREAD_H_

#include <memory>

#include "base/macros.h"
#include "base/notification.h"
#include "common/quic/core/quic_config.h"
#include "common/quic/platform/api/quic_containers.h"
#include "common/quic/platform/api/quic_mutex.h"
#include "common/quic/platform/api/quic_socket_address.h"
#include "common/quic/tools/quic_server.h"
#include "thread/thread.h"

namespace gfe_quic {
namespace test {

// Simple wrapper class to run QuicServer in a dedicated thread.
class ServerThread : public Thread {
 public:
  ServerThread(QuicServer* server, const QuicSocketAddress& address);

  ~ServerThread() override;

  // Prepares the server, but does not start accepting connections. Useful for
  // injecting mocks.
  void Initialize();

  // Runs the event loop. Will initialize if necessary.
  void Run() override;

  // Schedules the given action for execution in the event loop.
  void Schedule(std::function<void()> action);

  // Waits for the handshake to be confirmed for the first session created.
  void WaitForCryptoHandshakeConfirmed();

  // Pauses execution of the server until Resume() is called.  May only be
  // called once.
  void Pause();

  // Resumes execution of the server after Pause() has been called.  May only
  // be called once.
  void Resume();

  // Stops the server from executing and shuts it down, destroying all
  // server objects.
  void Quit();

  // Returns the underlying server.  Care must be taken to avoid data races
  // when accessing the server.  It is always safe to access the server
  // after calling Pause() and before calling Resume().
  QuicServer* server() { return server_.get(); }

  // Returns the port that the server is listening on.
  int GetPort();

 private:
  void MaybeNotifyOfHandshakeConfirmation();
  void ExecuteScheduledActions();

  absl::Notification
      confirmed_;             // Notified when the first handshake is confirmed.
  absl::Notification pause_;  // Notified when the server should pause.
  absl::Notification paused_;       // Notitied when the server has paused
  absl::Notification resume_;       // Notified when the server should resume.
  absl::Notification quit_;         // Notified when the server should quit.

  std::unique_ptr<QuicServer> server_;
  QuicSocketAddress address_;
  mutable QuicMutex port_lock_;
  int port_ GUARDED_BY(port_lock_);

  bool initialized_;

  QuicMutex scheduled_actions_lock_;
  QuicDeque<std::function<void()>> scheduled_actions_
      GUARDED_BY(scheduled_actions_lock_);

  DISALLOW_COPY_AND_ASSIGN(ServerThread);
};

}  // namespace test
}  // namespace gfe_quic

#endif  // GFE_QUIC_TEST_TOOLS_SERVER_THREAD_H_
