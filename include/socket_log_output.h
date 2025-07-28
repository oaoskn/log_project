#pragma once
#include "log_output.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <mutex>

class SocketLogOutput : public LogOutput {
private:
    int sockfd;
    struct sockaddr_in serverAddr;
    std::mutex mutex;
public:
    SocketLogOutput(const std::string& host, int port);
    ~SocketLogOutput();
    void write(const std::string& message) override;
};
