#ifndef EVENT_LIST_H
#define EVENT_LIST_H

#include "cross_platform.h"

#define EVENT_SEM_NAME "evsem"
enum service {
    CAT,
    TOTAL_SERVICES
};

typedef struct event_list {
    nsem_t * event_sem;
    //index is service enum value is type
    int * events;// [TOTAL_SERVICES];
} event_list;

//tell the server you have an event to be processed
void notify_event(int id, int type, event_list * list);
//find the index of the next event to be handled
int wait_event(event_list * list);
//no off set as it will always be the first item
void init_event_list(event_list * list, shared_memory * mem);
void cleanup_event_list(event_list * list);
#endif
