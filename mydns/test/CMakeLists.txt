
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(../inc/)
include_directories(mocks/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++1z")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wno-long-long -pedantic -std=gnu11")

FILE(GLOB c_src *.c)
add_library(foolib SHARED ${c_src})

FILE(GLOB src *.cc *.cpp)
add_executable(runUnitTests ${src})

# Link test executable against gtest & gtest_main
add_test(runUnitTests runUnitTests)
target_link_libraries(runUnitTests axfr foolib gmock_main gmock gtest gtest_main)
