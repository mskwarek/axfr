include_directories(inc)

FILE(GLOB C_SOURCES src/*.c)
add_library(systemFunc STATIC ${C_SOURCES})
