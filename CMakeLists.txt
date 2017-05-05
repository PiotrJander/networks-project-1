cmake_minimum_required(VERSION 3.6)
project(zadanie1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES src/echo-client.c src/echo-server.c src/err.h src/err.c)
add_executable(zadanie1 ${SOURCE_FILES})