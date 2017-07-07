cmake_minimum_required (VERSION 2.6)
project (super-disco)

set (super-disco_VERSION_MAJOR 1)
set (super-disco_VERSION_MINOR 0)

if (UNIX)
    add_definitions(-D_POSIX_C_SOURCE=200809L)
    add_definitions(-D_DEFAULT_SOURCE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif (UNIX)

include_directories(src)

set(SHARED 
    src/cross_platform.c
    src/queue.c
    )

set(SERVER
    src/server.c
    )
set(CAT
    src/cat_client.c
    )

add_executable(server ${SHARED} ${SERVER})
add_executable(cat_client ${SHARED} ${CAT})
if (UNIX)
    target_link_libraries(server rt)
    target_link_libraries(cat_client rt)
endif (UNIX)
