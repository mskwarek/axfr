
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(../inc/)
include_directories(mocks/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++1z -v")

add_library(foolib SHARED foo.c)
add_executable(runUnitTests cmock-function-mockers_test.cc cmock-spec-builders_test.cc foo_mock.cc
        mocks/dnsMock.cpp)

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests /Users/mskwarek/Documents/myDig/mydns/test/libfoolib.dylib gmock_main gmock gtest gtest_main)
add_test(runUnitTests runUnitTests)
