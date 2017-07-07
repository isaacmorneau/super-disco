#include <stdio.h>
#include "cross_platform.h"
#include "queue.h"
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

    queue * q;
    init_queue(&q, &mem, 0);

    if (enter_background_mode()) {
        return 1;
    }

    char buff[1024];
    memset(buff, 0, 1024);
    read_stc(q, buff, 1024);
    printf("%s", buff);
    cleanup_shared_memory(&mem);
}
