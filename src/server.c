#include <stdio.h>
#include "cross_platform.h"
#include "buffer.h"
#include <string.h>

int main() {
    shared_memory mem;
    char* shared_name = "shmem";
    memset(&mem, 0, sizeof(shared_memory));

    mem.name = shared_name;
    mem.buff_size = SHARED_BUFF_SIZE;

    int err;
    if ((err = fillout_shared_memory(&mem))) {
        printf("error %d", err);
        return err;
    }

    *(mem.running) = 1;

    buffer * q;
    init_buffer(&q, &mem, 0);

    if (enter_background_mode()) {
        return 1;
    }

    char buff[1024]; 
    memset(buff,0,1024);
    while (1) {
        if(!fgets(buff,1024,stdin))
            break;
        if(!strcmp(buff,"exit\n"))
            break;
        write_buffer(q, buff, strlen(buff));
    }
    
    *(mem.running) = 0;

    cleanup_shared_memory(&mem);
}
