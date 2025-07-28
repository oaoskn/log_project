#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include "logger.h"

class ThreadSafeQueue {
public:
    void push(const std::pair<LogLevel, std::string>& item) {
        std::lock_guard<std::mutex> lock(mutex);
        queue.push(item);
        condVar.notify_one();
    }

    std::pair<LogLevel, std::string> pop() {
        std::unique_lock<std::mutex> lock(mutex);
        condVar.wait(lock, [this] { return !queue.empty(); });
        auto item = queue.front();
        queue.pop();
        return item;
    }

private:
    std::queue<std::pair<LogLevel, std::string>> queue;
    std::mutex mutex;
    std::condition_variable condVar;
};
