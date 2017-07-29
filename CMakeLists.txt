cmake_minimum_required (VERSION 2.6)
project (super-disco)

set (super-disco_VERSION_MAJOR 1)
set (super-disco_VERSION_MINOR 0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if (UNIX)
    add_definitions(-D_POSIX_C_SOURCE=200809L)
    add_definitions(-D_DEFAULT_SOURCE)
endif (UNIX)

include_directories(src mbedtls)

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

set(HTTP
    src/http_client.c
    )

#i used https://github.com/isaacmorneau/relative-remover to scope all the source files into something more useable
file(GLOB HTTPS_TLS mbedtls/*.c)

set(HTTPS
    src/https_client.c
    )

add_executable(server ${SERVER} ${SHARED})
add_executable(cat_client ${CAT} ${SHARED})
add_executable(http_client ${HTTP} ${SHARED})
add_executable(https_client ${HTTPS} ${HTTPS_TLS} ${SHARED})


if (UNIX)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    target_link_libraries(server rt Threads::Threads)
    target_link_libraries(cat_client rt Threads::Threads)
    target_link_libraries(http_client rt Threads::Threads)
    target_link_libraries(https_client rt Threads::Threads)
else()
endif (UNIX)
