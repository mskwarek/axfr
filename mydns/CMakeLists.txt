cmake_minimum_required(VERSION 2.8.9)
project(axfrScanner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wno-long-long -pedantic -rdynamic -std=c++1z") # -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall -Wno-long-long -pedantic -rdynamic -std=gnu11") # -fprofile-arcs -ftest-coverage")

add_subdirectory (src)
add_subdirectory (DnsQuery)
add_subdirectory (SystemFunctionProxy)

