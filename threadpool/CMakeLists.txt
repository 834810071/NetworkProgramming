cmake_minimum_required(VERSION 3.5.1)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 14)

set(threadpool
        condition.cpp
        condition.h
        threadpool.cpp
        threadpool.h)

add_library(thread_src ${threadpool})

add_executable(test01 test.cpp)
target_link_libraries(test01 thread_src pthread rt)