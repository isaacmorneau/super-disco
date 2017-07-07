cmake_minimum_required (VERSION 2.6)
project (super-disco)

set (super-disco_VERSION_MAJOR 1)
set (super-disco_VERSION_MINOR 0)

#can do it manually
#set(SOURCES src/mainapp.cpp src/Student.cpp)
include_directories(src)
file(GLOB SOURCES src/*.c)

add_executable(super-disco ${SOURCES})
