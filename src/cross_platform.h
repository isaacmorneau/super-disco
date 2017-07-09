#ifndef CROSS_PLATFORM_H
#define CROSS_PLATFORM_H

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

#ifdef __linux
#include <unistd.h>
#include <semaphore.h>
typedef int fd_t;
#define yeild(x) sleep(x)
typedef sem_t nsem_t;
#elif _WIN32
#include <Windows.h>
typedef void* fd_t;
#define yeild(x) Sleep(x)
//?
//typedef sem_t nsem_t

#endif

/* Named Semaphore */

nsem_t * get_sem(char * name);
void close_sem(char * sem);

void wait_sem(nsem_t * sem);
void post_sem(nsem_t * sem);

/* Threading */

int enter_background_mode();
int exit_background_mode();

/* Shared Memory */

#define SHARED_BUFF_SIZE 1024
#define SHARED_NAME "shm"

typedef struct shared_memory {
    void* memory;
    int* running;
    fd_t map;
    int buff_size;
    char* name;
} shared_memory;

//expects name and buff_sze to be filled out
int fillout_shared_memory(shared_memory* buffer);
//cleans and zeros pointers in shared memory region
void cleanup_shared_memory(shared_memory* buffer);
//write to shared mem
void write_shared_memory(shared_memory* buffer,int dest, void* src, int len);
//read from shared mem
void read_shared_memory(shared_memory* buffer,int src, void* dest, int len);



#endif // !CROSS_PLATFORM_H

