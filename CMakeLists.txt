cmake_minimum_required(VERSION 3.0.0)
project(Chat VERSION 0.1.0)

add_executable(Server Server.cpp server_start.cpp command.cpp)
add_executable(Client Client.cpp client_start.cpp command.cpp)

target_link_libraries(Server -static ws2_32)
target_link_libraries(Client -static ws2_32)
