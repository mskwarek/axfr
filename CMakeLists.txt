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
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O2 -Wall -fPIC -DVERSION=\"9.11.0b3\" -D_REENTRANT -D_GNU_SOURCE")



include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/digUtils
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/bin/dig/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/dns/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/irs/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/bind9/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/unix/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/pthreads/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/x86_32/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccc/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/unix/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/include
	${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccfg/include

		-I/home/marcin/ClionProjects/myDig/bind_lib
		bind_lib
		-I/home/marcin/ClionProjects/myDig/bind_lib/lib/dns/include
		bind_lib/lib/dns/include
		-I/home/marcin/ClionProjects/myDig/bind_lib/lib/bind9/include
		bind_lib/lib/bind9/include
		-I/home/marcin/ClionProjects/myDig/bind_lib/lib/isc/include
		bind_lib/lib/isc
		bind_lib/lib/isc/include
		bind_lib/lib/isc/unix/include
		bind_lib/lib/isc/pthreads/include
		bind_lib/lib/isc/x86_32/include
		-I/home/marcin/ClionProjects/myDig/bind_lib/lib/lwres/include
		bind_lib/lib/lwres/unix/include
		bind_lib/lib/lwres/include
		-I/home/marcin/ClionProjects/myDig/bind_lib/lib/isccfg/include
		bind_lib/lib/isccfg/include
		-I/usr/include/libxml2
	)

# - Try to find LibXml2
# Once done this will define
#  LIBXML2_FOUND - System has LibXml2
#  LIBXML2_INCLUDE_DIRS - The LibXml2 include directories
#  LIBXML2_LIBRARIES - The libraries needed to use LibXml2
#  LIBXML2_DEFINITIONS - Compiler switches required for using LibXml2

find_package(PkgConfig)
pkg_check_modules(PC_LIBXML QUIET libxml-2.0)
set(LIBXML2_DEFINITIONS ${PC_LIBXML_CFLAGS_OTHER})

find_path(LIBXML2_INCLUDE_DIR libxml/xpath.h
		HINTS ${PC_LIBXML_INCLUDEDIR} ${PC_LIBXML_INCLUDE_DIRS}
		PATH_SUFFIXES libxml2 )

find_library(LIBXML2_LIBRARY NAMES xml2 libxml2
		HINTS ${PC_LIBXML_LIBDIR} ${PC_LIBXML_LIBRARY_DIRS} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibXml2  DEFAULT_MSG
		LIBXML2_LIBRARY LIBXML2_INCLUDE_DIR)

mark_as_advanced(LIBXML2_INCLUDE_DIR LIBXML2_LIBRARY )

set(LIBXML2_LIBRARIES ${LIBXML2_LIBRARY} )
set(LIBXML2_INCLUDE_DIRS ${LIBXML2_INCLUDE_DIR} )



find_package (Threads)
find_library(DNS NAMES libdns.a PATHS "${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/dns/libdns.a")

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
FIND_PACKAGE(Krb5 REQUIRED gssapi)
find_package(Krb5 REQUIRED krb5)

find_package(DIG_LIBS
		NAMES dl z pthread xml2 gssapi_krb5 krb5 k5crypto com_err dns readline termcap crypto isc
		PATHS
		/usr
		/usr/lib
		/usr/lib64
		/usr/local/lib
		/usr/local/lib64)
find_path(GssApiIncludes gssapi_krb5 PATHS
		/usr/lib
		/usr/lib64
		/usr/local/lib
		/usr/local/lib64)
if(NOT GssApiIncludes OR NOT DIG_LIBS )
	message(FATAL_ERROR "Can't find folder containing gssapi.h")
endif()
message(${DIG_LIBS})
link_libraries(${DNS_LIBS})



set(common_src digUtils/digQuery.c
		digUtils/dighost.c
		Scanner/axfrLookup.cpp
		Scanner/axfrLookup.h
		digUtils/digQuery.h
		digUtils/dig_parser.h
		digUtils/dig_parser.c
		Scanner/AxfrDatabase.hpp
		Scanner/InputReader.cpp
		Scanner/InputReader.h
		GeneralUtils/ms_response_list.h
		GeneralUtils/ms_response_list.c)

add_library(AxfrLib SHARED ${common_src} axfrlib.cpp)

target_link_libraries(AxfrLib
        ${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/dns/libdns.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/libisc.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/libisc-nosymtbl.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/irs/libirs.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccc/libisccc.a
        ${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccfg/libisccfg.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/liblwres.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/bind9/libbind9.a
		${DIG_LIBS})


add_executable(myDig ${common_src} main.cpp)

target_link_libraries(myDig
		${CMAKE_THREAD_LIBS_INIT}
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/dns/libdns.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/libisc.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isc/libisc-nosymtbl.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/irs/libirs.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccc/libisccc.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/isccfg/libisccfg.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/lwres/liblwres.a
		${CMAKE_CURRENT_SOURCE_DIR}/bind_lib/lib/bind9/libbind9.a
		${DIG_LIBS}
		${LIBXML2_LIBRARIES}
		)
