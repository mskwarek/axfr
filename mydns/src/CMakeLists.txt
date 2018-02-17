
#Bring the headers, such as Student.h into the project
include_directories(../inc)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES "*.c*")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic -std=c++1z")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall -Wno-long-long -pedantic -std=gnu11")
add_executable(axfrScanner ${SOURCES} ../test/foo.h ../test/foo.c)
