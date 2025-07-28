#include "logger.h"
#include "thread_safe_queue.h"
#include <thread>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string target = "file://log.txt";
    LogLevel defaultLevel = LogLevel::INFO;

    // Парсинг аргументов командной строки
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-f" && i + 1 < argc) {
            target = argv[i + 1];
            ++i;
        } else if (std::string(argv[i]) == "-l" && i + 1 < argc) {
            std::string levelStr = argv[i + 1];
            if (levelStr == "DEBUG") defaultLevel = LogLevel::DEBUG;
            else if (levelStr == "INFO") defaultLevel = LogLevel::INFO;
            else if (levelStr == "WARNING") defaultLevel = LogLevel::WARNING;
            else if (levelStr == "ERROR") defaultLevel = LogLevel::ERROR;
            ++i;
        }
    }

    Logger logger(target, defaultLevel);
    ThreadSafeQueue queue;

    // Поток логирования
    std::thread loggingThread([&logger, &queue]() {
        while (true) {
            auto [level, message] = queue.pop();
            if (message == "quit") break;
            logger.log(level, message);
        }
    });

    while (true) {
        std::string input;
        std::cout << "Введите сообщение (или 'quit' для выхода): ";
        std::getline(std::cin, input);
        if (input == "quit") {
            queue.push({defaultLevel, "quit"});
            break;
        }

    // Парсинг ввода: если указан уровень (например, "DEBUG: Сообщение"), извлекаем его
        size_t colonPos = input.find(':');
        LogLevel level = defaultLevel; // По умолчанию используем уровень из параметров
        std::string message = input;
        if (colonPos != std::string::npos) {
            std::string levelStr = input.substr(0, colonPos);
            if (levelStr == "DEBUG") level = LogLevel::DEBUG;
            else if (levelStr == "INFO") level = LogLevel::INFO;
            else if (levelStr == "WARNING") level = LogLevel::WARNING;
            else if (levelStr == "ERROR") level = LogLevel::ERROR;
            message = input.substr(colonPos + 1);
        }

    // Отправляем в очередь с указанным уровнем
        queue.push({level, message});
    }	

    loggingThread.join();
    return 0;
}
