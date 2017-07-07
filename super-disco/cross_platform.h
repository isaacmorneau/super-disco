#ifndef CROSS_PLATFORM_H
#define CROSS_PLATFORM_H


#ifdef __linux
typedef int fd_t;
#elif _WIN32
typedef void* fd_t;
#endif

/* Shared Memory */

#define SHARED_BUFF_SIZE 1024

typedef struct shared_memory {
    void* memory;
    fd_t map;
    int buff_size;
    char* name;
} shared_memory;

//expects name and buff_sze to be filled out
int fillout_shared_memory(shared_memory* buffer);
//cleans and zeros pointers in shared memory region
void cleanup_shared_memory(shared_memory* buffer);
//write to shared mem
void write_shared_memory(shared_memory* buffer, void* src, int len);
//read from shared mem
void read_shared_memory(shared_memory* buffer, void* dest, int len);

#endif // !CROSS_PLATFORM_H

