#include <memory>

#include "watcher.h"

time_t timestampMs()
{
    using namespace std::chrono;
    auto ms =
        duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    return ms.count();
}

Watcher::Watcher()
    : stopped_(true)
{
}

Watcher::~Watcher()
{
    stop();
}

void Watcher::start(std::function<int(BrixClient::BrixAWEvent)> callback,
                    int pollTimeMsec)
{
    stop();
    callback = callback;
    pollTimeMsec = pollTimeMsec;
    stopped_ = false;
    ++threadId_;
    daemonThread_ =
        std::make_unique<std::thread>(&Watcher::watcherDaemon, this, threadId_);
    workerThread_ =
        std::make_unique<std::thread>(&Watcher::queueWorker, this, threadId_);
}

void Watcher::stop()
{
    stopped_ = true;
    if (daemonThread_) {
        daemonThread_->join();
        daemonThread_.reset(nullptr);
    }
    if (workerThread_) {
        workerThread_->join();
        workerThread_.reset(nullptr);
    }
}

bool Watcher::running()
{
    return !(daemonThreadClear_ && watcherThreadClear_);
}

void Watcher::watcherDaemon(uint64_t threadId)
{
    while (!daemonThreadClear_) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(pollTimeMsec / 10));
    }

    daemonThreadClear_ = false;
    BrixClient::BrixAWEvent last_event;
    last_event.startTime = 0;
    time_t last_report = timestampMs();
    do {
        if (threadId != threadId_) {
            daemonThreadClear_ = true;
            return;
        }
        std::string cur_content = pollEvent();
        auto curTime = static_cast<uint64_t>(timestampMs());
        if (cur_content != last_event.eventContent) {
            if (last_event.startTime != 0) {
                last_event.endTime = curTime;
                last_report = timestampMs();
                eventQueue_.enqueue(last_event);
            }
            last_event.eventId = -1;
            last_event.eventType = eventTypeId();
            last_event.eventContent = cur_content;
            last_event.startTime = curTime;
            last_event.endTime = curTime;
        } else {
            last_event.endTime = curTime;
            if (timestampMs() - last_report > 60000) {
                last_report = timestampMs();
                eventQueue_.enqueue(last_event);
                last_event.eventId = -2;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(pollTimeMsec));
    } while (!stopped_);
    if (last_event.startTime != 0) {
        last_event.endTime = timestampMs();
        eventQueue_.enqueue(last_event);
    }
    daemonThreadClear_ = true;
}

void Watcher::queueWorker(uint64_t threadId)
{
    while (!watcherThreadClear_) {
        std::this_thread::sleep_for(
            std::chrono::milliseconds(pollTimeMsec / 10));
    }
    watcherThreadClear_ = false;
    int lastId = -1;
    while (!stopped_ || eventQueue_.size()) {
        if (threadId != threadId_)
            break;
        BrixClient::BrixAWEvent event = eventQueue_.dequeue();
        if (event.eventId == -2)
            event.eventId = lastId;
        lastId = callback(event);
    }
    watcherThreadClear_ = true;
}
