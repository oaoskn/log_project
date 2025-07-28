#include "logger.h"
#include "file_log_output.h"
#include "socket_log_output.h"
#include <sstream>
#include <chrono>
#include <iomanip>
#include <iostream>

Logger::Logger(const std::string& target, LogLevel defaultLevel)
    : defaultLevel(defaultLevel) {
    if (target.substr(0, 7) == "file://") {
        std::string filename = target.substr(7);
        logOutput = std::make_unique<FileLogOutput>(filename);
    } else if (target.substr(0, 9) == "socket://") {
        size_t colonPos = target.find(':', 9);
        if (colonPos != std::string::npos) {
            std::string host = target.substr(9, colonPos - 9);
            int port = std::stoi(target.substr(colonPos + 1));
            logOutput = std::make_unique<SocketLogOutput>(host, port);
        } else {
            std::cerr << "Ошибка: неверный формат адреса сокета" << std::endl;
        }
    } else {
        std::cerr << "Ошибка: неверный формат цели" << std::endl;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    if (static_cast<int>(level) < static_cast<int>(defaultLevel.load())) return;
    std::string formatted = getTimestamp() + " [" + std::to_string(static_cast<int>(level)) + "] " + message;
    if (logOutput) {
        logOutput->write(formatted);
    }
}

void Logger::setDefaultLevel(LogLevel level) {
    defaultLevel.store(level);
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
