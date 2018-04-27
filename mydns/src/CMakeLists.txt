
#Bring the headers, such as Student.h into the project
include_directories(../inc)
include_directories(../DnsQuery/inc)

add_executable(axfrScanner main.cpp)
target_link_libraries(axfrScanner axfr)

