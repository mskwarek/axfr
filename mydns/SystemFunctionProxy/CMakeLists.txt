include_directories(inc)

FILE(GLOB C_SOURCES src/*.c)
add_library(systemFunc SHARED ${C_SOURCES})
