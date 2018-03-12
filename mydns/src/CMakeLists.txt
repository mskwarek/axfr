
#Bring the headers, such as Student.h into the project
include_directories(../inc)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB C_SOURCES "*.c")
file(GLOB CXX_SOURCES "*.cpp")

add_library(axfr SHARED ${C_SOURCES})

add_executable(axfrScanner ${CXX_SOURCES})
target_link_libraries(axfrScanner axfr)

