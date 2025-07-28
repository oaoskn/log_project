#include "file_log_output.h"
#include <iostream>

FileLogOutput::FileLogOutput(const std::string& filename) {
    file.open(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
    }
}

FileLogOutput::~FileLogOutput() {
    if (file.is_open()) {
        file.close();
    }
}

void FileLogOutput::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex);
    if (file.is_open()) {
        file << message << std::endl;
        file.flush();
    }
}
