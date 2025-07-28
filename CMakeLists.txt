cmake_minimum_required(VERSION 3.10)
project(LoggingProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_library(LoggerLib STATIC src/logger.cpp src/file_log_output.cpp src/socket_log_output.cpp)
add_library(LoggerLibShared SHARED src/logger.cpp src/file_log_output.cpp src/socket_log_output.cpp)

add_executable(LoggerApp src/main.cpp)
target_link_libraries(LoggerApp LoggerLibShared)
