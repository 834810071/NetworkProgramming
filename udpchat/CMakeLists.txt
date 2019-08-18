cmake_minimum_required(VERSION 3.5.1)
project(NetworkProgramming)

set(CMAKE_CXX_STANDARD 14)

#set(pub Pub.h)
#
#add_library(pub_src ${pub})

add_executable(UdpChatCli UdpChatCli.cpp)
add_executable(UdpChatSrv UdpChatSrv.cpp)
#target_link_libraries(UdpChatCli pub_src)
#target_link_libraries(UdpChatSrv pub_src)
