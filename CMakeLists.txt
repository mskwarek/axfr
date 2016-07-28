cmake_minimum_required(VERSION 3.5)
project(myDig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

find_library(DNS NAMES libdns.a PATHS "/home/marcin/mgr/bind-9.2.3/lib/dns/libdns.a")
include_directories(/home/marcin/mgr/bind-9.2.3/lib/dns/include
        /home/marcin/mgr/bind-9.2.3/lib/isc/include
        /home/marcin/mgr/bind-9.2.3/lib/isccc/include
        /home/marcin/mgr/bind-9.2.3/lib/isccfg/include
        /home/marcin/mgr/bind-9.2.3/lib/isc
        /home/marcin/mgr/bind-9.2.3/lib/isc/include
        /home/marcin/mgr/bind-9.2.3/lib/isc/unix/include
        /home/marcin/mgr/bind-9.2.3/lib/isc/nothreads/include
        /home/marcin/mgr/bind-9.2.3/bin/dig/include
        ${CMAKE_CURRENT_SOURCE_DIR})

set(SOURCE_FILES digQuery.c dighost.c axfrLookup.cpp axfrLookup.h main.cpp digQuery.h digUtils/dig_parser.h digUtils/dig_parser.c)
add_executable(myDig ${SOURCE_FILES})

target_link_libraries(myDig
        /home/marcin/mgr/bind-9.2.3/lib/dns/libdns.a
        /home/marcin/mgr/bind-9.2.3/lib/isc/libisc.a)