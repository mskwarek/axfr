cmake_minimum_required(VERSION 2.8.9)
project(axfrScanner)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -rdynamic -std=c++1z")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-long-long -pedantic -rdynamic -std=gnu11")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -ggdb -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -g -ggdb -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O3")

configure_file(CMakeLists.txt.in spdlog-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/spdlog-download )
if(result)
  message(FATAL_ERROR "CMake step for spdlog failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/spdlog-download )
if(result)
  message(FATAL_ERROR "Build step for spdlog failed: ${result}")
endif()
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/spdlog-src
                 ${CMAKE_CURRENT_BINARY_DIR}/spdlog-build
                 EXCLUDE_FROM_ALL)

add_subdirectory (src)
add_subdirectory (DnsQuery)
add_subdirectory (SystemFunctionProxy)

