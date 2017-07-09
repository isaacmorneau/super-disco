#include <stdio.h>
#include <string.h>
#include "cross_platform.h"
#include "buffer.h"
#include "event_list.h"

int main() {
    shared_memory mem;
    event_list list;
    buffer * q;
    
    //sm
    memset(&mem, 0, sizeof(shared_memory));

    mem.name = SHARED_NAME;
    mem.buff_size = SHARED_BUFF_SIZE;

    int err;
    if ((err = fillout_shared_memory(&mem))) {
        printf("error %d", err);
        return err;
    }

    //evl
    init_event_list(&list, &mem);

    //bf
    init_buffer(&q, &mem, sizeof(int)*TOTAL_SERVICES);

    if (enter_background_mode()) {
        return 1;
    }
    char buff[1024];
    memset(buff, 0, 1024);
    while (1) {
        notify_event(CAT, 1, &list);
        if(read_buffer(q, buff, 1024) > 0)
            printf("%s", buff);
        if(!*mem.running)
            break;
    }
    cleanup_shared_memory(&mem);
}
