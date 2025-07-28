#include "socket_log_output.h"
#include <unistd.h>
#include <iostream>

SocketLogOutput::SocketLogOutput(const std::string& host, int port) : sockfd(-1) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Ошибка: не удалось создать сокет" << std::endl;
        return;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Ошибка: не удалось разрешить адрес " << host << std::endl;
        close(sockfd);
        sockfd = -1;
        return;
    }
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Ошибка: не удалось подключиться к серверу" << std::endl;
        close(sockfd);
        sockfd = -1;
    }
}

SocketLogOutput::~SocketLogOutput() {
    if (sockfd >= 0) {
        close(sockfd);
    }
}

void SocketLogOutput::write(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex);
    if (sockfd >= 0) {
        send(sockfd, message.c_str(), message.size(), 0);
        send(sockfd, "\n", 1, 0);
    }
}
