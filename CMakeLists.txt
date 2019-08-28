cmake_minimum_required(VERSION 3.0)

project(plinkfilter)

add_executable(plinkfilter main.cpp)

install(TARGETS plinkfilter RUNTIME DESTINATION bin)
