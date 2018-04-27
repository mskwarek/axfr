include_directories(../inc)
include_directories(inc)

FILE(GLOB C_SOURCES src/*.c)
add_library(axfr SHARED ${C_SOURCES})

add_subdirectory(test)