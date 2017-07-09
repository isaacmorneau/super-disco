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

    *(mem.running) = 1;

    //evl
    init_event_list(&list, &mem);

    //bf
    init_buffer(&q, &mem, sizeof(int)*TOTAL_SERVICES);

    if (enter_background_mode()) {
        return 1;
    }

    char buff[1024]; 
    memset(buff,0,1024);
    int ev_id;
    while (1) {
        if(!fgets(buff,1024,stdin))
            break;
        if(!strcmp(buff,"exit\n"))
            break;
        if((ev_id = wait_event(&list)) == -1)
            return -1;
        printf("Event Triggered %d\n", ev_id);
        write_buffer(q, buff, strlen(buff));
    }
    
    *(mem.running) = 0;

    cleanup_event_list(&list);
    cleanup_shared_memory(&mem);
}
