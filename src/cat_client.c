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

    buffer * b;
    init_buffer(&b, &mem, 0);

    if (enter_background_mode()) {
        return 1;
    }

    char buff[1024];
    memset(buff, 0, 1024);
    while (1) {
        if(read_buffer(b, buff, 1024) > 0)
            printf("%s", buff);
        else
            break;
        if(!*mem.running)
            break;
    }
    cleanup_shared_memory(&mem);
}
