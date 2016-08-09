cmake_minimum_required(VERSION 2.8)
project(myDig)

set(CMAKE_VERBOSE_MAKEFILE)
set(dir ${CMAKE_CURRENT_SOURCE_DIR}/build/)
set(EXECUTABLE_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(LIBRARY_OUTPUT_PATH ${dir} CACHE PATH "Build directory" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${dir})
set(CMAKE_BUILD_FILES_DIRECTORY ${dir})
set(CMAKE_BUILD_DIRECTORY ${dir})
set(CMAKE_BINARY_DIR  ${dir})
set(EXECUTABLE_OUTPUT_PATH ${dir})
set(LIBRARY_OUTPUT_PATH ${dir}lib)
set(CMAKE_CACHEFILE_DIR ${dir})


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

find_library(DNS NAMES libdns.a PATHS "/home/marcin/mgr/bind-9.2.3/lib/dns/libdns.a")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/dns/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isccc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isccfg/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/unix/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/nothreads/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/bin/dig/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/digUtils)

set(SOURCE_FILES digUtils/digQuery.c digUtils/dighost.c Scanner/axfrLookup.cpp Scanner/axfrLookup.h main.cpp digUtils/digQuery.h digUtils/dig_parser.h digUtils/dig_parser.c Scanner/AxfrDatabase.hpp Scanner/InputReader.cpp Scanner/InputReader.h)
add_executable(myDig ${SOURCE_FILES})

target_link_libraries(myDig
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dns/libdns.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/libisc.a)