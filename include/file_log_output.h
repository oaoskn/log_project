#pragma once
#include "log_output.h"
#include <fstream>
#include <mutex>

class FileLogOutput : public LogOutput {
private:
    std::ofstream file;
    std::mutex mutex;
public:
    FileLogOutput(const std::string& filename);
    ~FileLogOutput();
    void write(const std::string& message) override;
};
