#pragma once
#include <string>

class LogOutput {
public:
    virtual ~LogOutput() = default;
    virtual void write(const std::string& message) = 0;
};
