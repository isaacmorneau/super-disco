#include <stdio.h>
#include "cross_platform.h"
#include "queue.h"
#include <string.h>

int main(int argc, char** argv) {
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

    if (argc > 1) {
        char buff[256];
        memset(buff, 0, 256);
        read_stc(q, buff, 256);
        printf("read: '%s'", buff);
    }
    else
    {
        char buff[] = "This is a really long message to make sure that the packetizing doesnt actually loose any of the text 123456789";
        write_stc(q, buff, strlen(buff));
    }
    getchar();
    cleanup_shared_memory(&mem);
}