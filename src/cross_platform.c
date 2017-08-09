#include "cross_platform.h"
#include <stdio.h>
#include <string.h>

void write_shared_memory(shared_memory* buffer,int dest, void* src, int len) {
    memcpy(buffer->memory + dest, src, len);
}

void read_shared_memory(shared_memory* buffer,int src, void* dest, int len) {
    memcpy(dest, buffer->memory + src, len);
}

#ifdef __linux
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>

nsem_t * get_sem(char * name) {
    nsem_t * id = sem_open(name, O_CREAT, 0755, 0);
    if (id == SEM_FAILED)
        return 0;
    return id;
}

void wait_sem(nsem_t * sem) {
    sem_wait(sem);
}

void post_sem(nsem_t * sem) {
    sem_post(sem);
}

void close_sem(char * sem) {
    sem_unlink(sem);
}

int enter_background_mode() {
    if (nice(19) == -1) {
        return 1;
    }
    return 0;
}

int exit_background_mode() {
    if (nice(0) == -1) {
        return 1;
    }
    return 0;
}

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
    buffer->running = (int*)buffer->memory;
    return 0;
}

void cleanup_shared_memory(shared_memory* buffer) {
    munmap(buffer->memory, buffer->buff_size);
    buffer->memory = 0;
    shm_unlink(buffer->name);
    buffer->map = 0;
}


#elif _WIN32
#include <windows.h>
#include <conio.h>

nsem_t * get_sem(char * name) {
    nsem_t * id = CreateSemaphore(0,0,256, name);
    if (id == 0)
        return 0;
    return id;
}

void wait_sem(nsem_t * sem) {
    WaitForSingleObject(sem, 0);
}

void post_sem(nsem_t * sem) {
    ReleaseSemaphore(sem, 1, 0);
}

void close_sem(char * sem) {
    CloseHandle(sem);
}


int enter_background_mode() {
    if (!SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN)) {
        return 1;
    }
    return 0;
}

int exit_background_mode() {
    if (!SetThreadPriority(GetCurrentThread(), THREAD_MODE_BACKGROUND_END)) {
        return 1;
    }
    return 0;
}

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
#endif
