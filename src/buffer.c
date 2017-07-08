#include "buffer.h"
#include <string.h>

void wait_while(buffer * q, int type, int interval) {
    if (interval == -1)
        while (q->_signal == type);
    else
        while (q->_signal == type)
            yeild(interval);
}
void wait_until(buffer * q, int type, int interval) {
    if (interval == -1)
        while (q->_signal != type);
    else
        while (q->_signal != type)
            yeild(interval);
}

int write_buffer(buffer * q, char * text, int len) {
    int progress = len;
    q->total = len;
    while (progress > 0) {
        memcpy(q->data, text + (len - progress), B_SIZE);
        q->_signal = B_NEXT;
        wait_while(q, B_NEXT, 0);
        progress -= B_SIZE;
    }
    q->_signal = B_NEXT;
    return 0;
}
int read_buffer(buffer * q, char * text, int len) {
    wait_until(q, B_NEXT, 0);
    int progress = 0, total = MIN(q->total, len);
    while (progress < total) {
        memcpy(text + progress, q->data, MIN(len - progress, B_SIZE));
        q->_signal = B_DONE;
        wait_while(q, B_DONE, 0);
        progress += B_SIZE;
    }
    return progress;
}
void init_buffer(buffer ** q, shared_memory * mem, unsigned int pos) {
    *q = (buffer *)((*mem).memory + pos);
}
