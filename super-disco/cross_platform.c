#include "cross_platform.h"

#include <stdio.h>
#ifdef __linux
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int fillout_shared_memory(shared_memory* buffer) {
    buffer->map = shm_open(
        buffer->name,
        O_CREAT | O_RDWR,
        S_IRUSR | S_IWUSR);
    if (buffer->map == -1) {
        //failed to create file map object
        return 1;
    }
    if (ftruncate(buffer->map, buffer->buff_size) == -1) {
        //failed to truncate
        return 3;
    }
    buffer->memory = mmap(0, buffer->buff_size,
        PROT_READ | PROT_WRITE, MAP_SHARED, buffer->map, 0);

    if (buffer->memory == MAP_FAILED) {
        //failed to map view of file
        shm_unlink(buffer->name);
        buffer->map = 0;
        buffer->memory = 0;
        return 2;
    }

    return 0;
}

void cleanup_shared_memory(shared_memory* buffer) {
    munmap(buffer->memory, buffer->buff_size);
    buffer->memory = 0;
    shm_unlink(buffer->name);
    buffer->map = 0;
}


void write_shared_memory(shared_memory* buffer, void* src, int len) {
    memcpy(buffer->memory, src, len);
}
void read_shared_memory(shared_memory* buffer, void* dest, int len) {
    memcpy(dest, buffer->memory, len);
}
#elif _WIN32
#include <windows.h>
#include <conio.h>


int fillout_shared_memory(shared_memory* buffer) {
    buffer->map = CreateFileMapping(
        INVALID_HANDLE_VALUE,    // use paging file
        NULL,                    // default security
        PAGE_READWRITE,          // read/write access
        0,                       // maximum object size (high-order DWORD)
        buffer->buff_size,       // maximum object size (low-order DWORD)
        buffer->name);           // name of mapping object
    if (buffer->map == 0) {
        //failed to create file map object
        return 1;
    }
    buffer->memory = MapViewOfFile(buffer->map,   // handle to map object
        FILE_MAP_ALL_ACCESS, // read/write permission
        0,
        0,
        buffer->buff_size);

    if (buffer->memory == 0) {
        //failed to map view of file
        CloseHandle(buffer->map);
        buffer->map = 0;
        return 2;
    }

    return 0;
}

void cleanup_shared_memory(shared_memory* buffer) {
    UnmapViewOfFile(buffer->memory);
    buffer->memory = 0;
    CloseHandle(buffer->map);
    buffer->map = 0;
}

void write_shared_memory(shared_memory* buffer, void* src, int len) {
    memcpy(buffer->memory, src, len);
}

void read_shared_memory(shared_memory* buffer, void* dest, int len) {
    memcpy(dest, buffer->memory, len);
}

#endif
