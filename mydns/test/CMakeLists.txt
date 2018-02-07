
enable_testing()
find_package(GTest REQUIRED)
#find_package(GMock REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(cmock)
include_directories(mocks/)
set(GLOB MOCK_SRC "mocks/*.c")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -stdlib=libc++ -std=c++1z -v")

add_executable(runUnitTests dns_ut.cpp mocks/testMock.c cmock-function-mockers_test.cc)

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gtest gtest_main gmock gmock_main)
add_test(runUnitTests runUnitTests)
