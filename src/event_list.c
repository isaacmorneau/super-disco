#include "event_list.h"
#include "cross_platform.h"
#include <stdio.h>
#include <errno.h>

void notify_event(int id, int type, event_list * list) {
    list->events[id] = type;
    post_sem(list->event_sem);
}

int wait_event(event_list * list) {
    wait_sem(list->event_sem);
    int i = 0;
    while(i < TOTAL_SERVICES)
        if(list->events[i])
            return i;
    return -1;
}

void init_event_list(event_list * list, shared_memory * mem) {
    list->event_sem = get_sem(EVENT_SEM_NAME);
    if(list->event_sem == 0){
        printf("bad %d", errno);
    }
    list->events = (int *)mem->memory;
}
void cleanup_event_list(event_list * list) {
    close_sem(EVENT_SEM_NAME);
    list->event_sem = 0;
    list->events = 0;
}
