#include "queue.h"
#include <Windows.h>
#include <string.h>

int wait_for_cts(queue * q, int type, int interval) {
    if (interval == -1) {
        //spinlock
        while (q->_cts_signal == type);
        return (q->_cts_signal == Q_END);
    }
    while (q->_cts_signal == type) {
#ifdef __linux
        sleep(interval);
#elif _WIN32
        Sleep(interval);
#endif
    }
    return (q->_cts_signal == Q_END);
}

int wait_for_stc(queue * q, int type, int interval) {
    if (interval == -1) {
        //spinlock
        while (q->_stc_signal == type);
        return (q->_stc_signal == Q_END);
    }
    while (q->_stc_signal == type) {
#ifdef __linux
        sleep(interval);
#elif _WIN32
        Sleep(interval);
#endif
    }
    return (q->_stc_signal == Q_END);
}

int write_stc(queue * q, char * text, int len) {
    int progress = len;
    while (progress > 0) {
        memcpy(q->client, text + (len - progress), Q_SIZE);
        q->_stc_signal = Q_WRITE;
        if (wait_for_stc(q, Q_WRITE, 0))
            return (len - progress);
        progress -= Q_SIZE;
    }
    q->_stc_signal = Q_END;
    return 0;
}

int write_cts(queue * q, char * text, int len) {
    int progress = len;
    while (progress > 0) {
        memcpy(q->server, text + (len - progress), Q_SIZE);
        q->_cts_signal = Q_WRITE;
        if (wait_for_cts(q, Q_WRITE, 0))
            return (len - progress);
        progress -= Q_SIZE;
    }
    q->_cts_signal = Q_END;
    return 0;
}

int read_cts(queue * q, char * text, int len) {
    int progress = 0;
    while (progress < len) {
        if (wait_for_cts(q, Q_DONE, 0))
            return progress;
        memcpy(text + progress, q->server, min(len - progress, Q_SIZE));
        q->_cts_signal = Q_DONE;
        progress += Q_SIZE;
    }
    q->_cts_signal = Q_END;
    return 0;
}

int read_stc(queue * q, char * text, int len) {
    int progress = 0;
    while (progress < len) {
        if (wait_for_stc(q, Q_DONE, 0))
            return progress;
        memcpy(text + progress, q->client, min(len - progress, Q_SIZE));
        q->_stc_signal = Q_DONE;
        progress += Q_SIZE;
    }
    q->_cts_signal = Q_END;
    return 0;
}

void init_queue(queue ** q, shared_memory * mem, unsigned int pos) {
    *q = (queue *)((*mem).memory) + pos;
    (*q)->_cts_signal = Q_DONE;
    (*q)->_stc_signal = Q_DONE;
    //could set buffer but you should do that yourself
}
