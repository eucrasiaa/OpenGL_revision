#pragma once
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include "commandSource/ICommandSource.hpp"


class TerminalInputService : public ICommandSource {
private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::atomic<bool> isRunning_{false};
    std::thread workerThread_;

    void workerLoop() {
        std::string line;
        // std::getline blocks!! 
        // check isRunning after each line read
        while (isRunning_ && std::getline(std::cin, line)) {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.push(line);
        }
    }

public:
    TerminalInputService() = default;
    
    ~TerminalInputService() {
        stop();
    }

    void start() {
        if (isRunning_) return;
        isRunning_ = true;
        workerThread_ = std::thread(&TerminalInputService::workerLoop, this);
    }

    void stop() {
        if (!isRunning_) return;
          isRunning_ = false;
        
        // Note: std::getline is a blocking call. 
        // If stuck on getline, the thread exits once the user presses Enter 
        // or when std::cin reaches EOF (e.g., closing the app console).
        if (workerThread_.joinable()) {
            workerThread_.join();
        }
    }

    bool popCommand(std::string& out_cmd) override {
        std::lock_guard<std::mutex> lock(mutex_);
        if (queue_.empty()) return false;
        out_cmd = queue_.front();
        queue_.pop();
        return true;
    }
};
