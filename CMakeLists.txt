cmake_minimum_required(VERSION 3.6)
project(zadanie1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(
        SOURCE_FILES
        src/client.c
        src/server.c
)

add_executable(
        server
        src/server.c
        src/err.h
        src/socket_wrappers.h
        src/circular_queue.h
        src/client_list.h
)

add_executable(
        client
        src/client.c
        src/err.h
        src/socket_wrappers.h
)
