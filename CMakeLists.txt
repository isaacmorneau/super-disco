cmake_minimum_required (VERSION 2.6)
project (super-disco)

set (super-disco_VERSION_MAJOR 1)
set (super-disco_VERSION_MINOR 0)

include_directories(src)
file(GLOB SOURCES src/*.c)

if (UNIX)
    add_definitions(-D_POSIX_C_SOURCE=200809L)
    add_definitions(-D_DEFAULT_SOURCE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif (UNIX)

add_executable(super-disco ${SOURCES})
if (UNIX)
    target_link_libraries(super-disco rt)
endif (UNIX)
