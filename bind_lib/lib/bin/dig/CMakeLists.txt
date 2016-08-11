cmake_minimum_required(VERSION 3.5)
project(dig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

set(SOURCE_FILES
    include/dig/dig.h
    include/dig/myDig.c
    dig.c
    dighost.c
    host.c
    myDig.c
    myDig.h
    nslookup.c)

add_executable(dig ${SOURCE_FILES})