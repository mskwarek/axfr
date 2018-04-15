
#Bring the headers, such as Student.h into the project
include_directories(../inc)
include_directories(../inc/DnsQuery)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB C_SOURCES "DnsQuery/*.c")
file(GLOB CXX_SOURCES "DnsQuery/*.cpp")

add_library(axfr SHARED ${C_SOURCES})

add_executable(axfrScanner main.cpp utils.c ${CXX_SOURCES})
target_link_libraries(axfrScanner axfr)

