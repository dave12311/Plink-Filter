cmake_minimum_required(VERSION 3.0)

project(plinkfilter)

add_executable(plinkfilter main.cpp)

#target_link_libraries(plinkfilter PUBLIC "-static")

install(TARGETS plinkfilter RUNTIME DESTINATION bin)
