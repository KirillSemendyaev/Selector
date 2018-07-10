cmake_minimum_required(VERSION 3.5)
project(Selector)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES select_server.c tcp_client.c udp_client.c)
add_executable(TCP_CLIENT tcp_client.c)
add_executable(UDP_CLIENT udp_client.c)
add_executable(SELECT_SERVER select_server.c)