#ifndef WATCHER_H
#define WATCHER_H

#include <atomic>
#include <chrono>
#include <functional>
#include <memory>
#include <thread>

#include "httpClient/brixclient.h"
#include "utils/safequeue.h"

class Watcher
{
  public:
    Watcher();
    virtual ~Watcher();
    virtual int eventTypeId() const = 0;
    void start(std::function<int(BrixClient::BrixAWEvent)> callback,
               int pollTimeMsec = 1000);
    void stop();
    bool running();

  private:
    void watcherDaemon(uint64_t threadId);
    void queueWorker(uint64_t threadId);
    virtual std::string pollEvent() = 0;

    std::atomic<bool> stopped_ = true, daemonThreadClear_ = true,
                      watcherThreadClear_ = true;
    int pollTimeMsec {};
    uint64_t threadId_ {};
    SafeQueue<BrixClient::BrixAWEvent> eventQueue_;
    std::function<int(BrixClient::BrixAWEvent)> callback;

    std::unique_ptr<std::thread> daemonThread_ {};
    std::unique_ptr<std::thread> workerThread_ {};
};

#endif  // WATCHER_H
