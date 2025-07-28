#pragma once
#include "log_output.h"
#include <atomic>
#include <string>
#include <memory>
enum class LogLevel : int {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

class Logger {
public:
    Logger(const std::string& target, LogLevel defaultLevel);
    void log(LogLevel level, const std::string& message);
    void setDefaultLevel(LogLevel level);
private:
    std::unique_ptr<LogOutput> logOutput;
    std::atomic<LogLevel> defaultLevel;
    std::string getTimestamp();
};
