include_directories(../inc)
include_directories(inc)
include_directories(../SystemFunctionProxy/inc)

FILE(GLOB C_SOURCES src/*.c)
add_library(axfr SHARED ${C_SOURCES})

target_link_libraries(axfr systemFunc)
#add_subdirectory(test)
