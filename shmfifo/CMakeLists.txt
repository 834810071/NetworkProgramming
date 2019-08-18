cmake_minimum_required(VERSION 3.5.1)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 14)

set(shmfifo_src
        shmfifo.cpp
        shmfifo.h)

add_library(shmfifo ${shmfifo_src})


add_executable(shmfifo_recv shmfifo_recv.cpp)
add_executable(shmfifo_send shmfifo_send.cpp)
add_executable(shmfifo_destroy shmfifo_destroy.cpp)

target_link_libraries(shmfifo_recv shmfifo)
target_link_libraries(shmfifo_send shmfifo)
target_link_libraries(shmfifo_destroy shmfifo semtool)