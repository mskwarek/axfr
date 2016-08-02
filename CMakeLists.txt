cmake_minimum_required(VERSION 3.5)
project(myDig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

find_library(DNS NAMES libdns.a PATHS "/home/marcin/mgr/bind-9.2.3/lib/dns/libdns.a")
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/dns/include
        /${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isccc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isccfg/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/unix/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/nothreads/include
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/bin/dig/include
        ${CMAKE_CURRENT_SOURCE_DIR},
        ${CMAKE_CURRENT_SOURCE_DIR}/digUtils)

set(SOURCE_FILES digUtils/digQuery.c digUtils/dighost.c axfrLookup.cpp axfrLookup.h main.cpp digUtils/digQuery.h digUtils/dig_parser.h digUtils/dig_parser.c AxfrDatabase.hpp)
add_executable(myDig ${SOURCE_FILES})

target_link_libraries(myDig
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/dns/libdns.a
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/isc/libisc.a)