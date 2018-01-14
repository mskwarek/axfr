cmake_minimum_required(VERSION 2.8.9)
project(axfrScanner)
 
#Bring the headers, such as Student.h into the project
include_directories(inc)
 
#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "src/*.c*")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic")
add_executable(axfrScanner ${SOURCES})
