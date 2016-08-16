cmake_minimum_required(VERSION 2.8)
project(myDig)

set(CMAKE_VERBOSE_MAKEFILE=1)
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


SET(CMAKE_C_COMPILER gcc)
SET(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -fPIC -DVERSION=\"9.11.0b3\" -D_GNU_SOURCE")

find_library(DNS NAMES libdns.a PATHS "${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/dns/libdns.a")
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/digUtils
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/bin/dig/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/dns/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/bind9/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/unix/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/pthreads/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/x86_32/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/unix/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccfg/include
	)


      
set(common_src digUtils/digQuery.c digUtils/dighost.c Scanner/axfrLookup.cpp Scanner/axfrLookup.h digUtils/digQuery.h digUtils/dig_parser.h digUtils/dig_parser.c Scanner/AxfrDatabase.hpp Scanner/InputReader.cpp Scanner/InputReader.h GeneralUtils/ms_response_list.h GeneralUtils/ms_response_list.c)

add_executable(myDig ${common_src} main.cpp)
add_library(AxfrLib SHARED ${common_src} axfrlib.cpp)

target_link_libraries(myDig
        ${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/dns/libdns.a
        ${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccfg/libisccfg.a
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/libisc-nosymtbl.a
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/liblwres.a
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/bind9/libbind9.a)

target_link_libraries(AxfrLib
        ${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/dns/libdns.a
        ${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccfg/libisccfg.a
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/libisc-nosymtbl.a
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/liblwres.a
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/bind9/libbind9.a)
