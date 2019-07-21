cmake_minimum_required(VERSION 3.13)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 14)

add_executable(NetworkProgramming main.cpp)
add_executable(echosrv echosrv.cpp)
add_executable(echocli echocli.cpp)
add_executable(echosrv2 echosrv2.cpp)
add_executable(echocli2 echocli2.cpp)
add_executable(echosrv3 echosrv3.cpp)
add_executable(echocli3 echocli3.cpp)
add_executable(p2pcli p2pcli.cpp)
add_executable(p2pserv p2pserv.cpp)