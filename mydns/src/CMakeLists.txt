
#Bring the headers, such as Student.h into the project
include_directories(../inc)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++1z")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wno-long-long -pedantic -std=gnu11")

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB C_SOURCES "*.c")
file(GLOB CXX_SOURCES "*.cpp")

add_library(axfr SHARED ${C_SOURCES})

add_executable(axfrScanner ${CXX_SOURCES} ${C_SOURCES})
target_link_libraries(axfrScanner axfr)

