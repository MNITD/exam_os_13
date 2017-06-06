cmake_minimum_required(VERSION 3.6)
project(exam_os_13)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES socket_server.cpp)
add_executable(exam_os_13 ${SOURCE_FILES})