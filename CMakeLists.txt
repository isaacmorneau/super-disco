cmake_minimum_required (VERSION 2.6)
project (super-disco)

set (super-disco_VERSION_MAJOR 1)
set (super-disco_VERSION_MINOR 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (UNIX)
    add_definitions(-D_POSIX_C_SOURCE=200809L)
    add_definitions(-D_DEFAULT_SOURCE)
endif (UNIX)

include_directories(src)

set(SHARED 
    src/cross_platform.c
    src/queue.c
    src/buffer.c
    src/event_list.c
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
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(server rt Threads::Threads)
    target_link_libraries(cat_client rt Threads::Threads)
endif (UNIX)
