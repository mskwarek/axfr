
enable_testing()
find_package(GTest REQUIRED)
#find_package(GMock REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${GMOCK_INCLUDE_DIRS})
include_directories(../inc/)
include_directories(mocks/)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++1z -stdlib=libc++ -v")

add_executable(runUnitTests cmock-function-mockers_test.cc cmock-spec-builders_test.cc mocks/testMock.cc mocks/dnsMock.cpp ../inc/dummyFunc.h)

# Link test executable against gtest & gtest_main
target_link_libraries(runUnitTests gmock_main gmock gtest gtest_main)
add_test(runUnitTests runUnitTests)
