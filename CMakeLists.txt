cmake_minimum_required(VERSION 3.6)
project(zadanie1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES echo-client.c echo-server.c err.h err.c)
add_executable(zadanie1 ${SOURCE_FILES})