#include "queue.h"
#include <Windows.h>
#include <string.h>

int wait_while_cts(queue * q, int type, int interval) {
    if (interval == -1) {
        //spinlock
        while (q->_cts_signal == type);
        return (q->_cts_signal == Q_END);
    }
    while (q->_cts_signal == type) {
        yeild(interval);
    }
    return (q->_cts_signal == Q_END);
}

int wait_while_stc(queue * q, int type, int interval) {
    if (interval == -1) {
        //spinlock
        while (q->_stc_signal == type);
        return (q->_stc_signal == Q_END);
    }
    while (q->_stc_signal == type) {
        yeild(interval);
    }
    return (q->_stc_signal == Q_END);
}

int wait_until_cts(queue * q, int type, int interval) {
    if (interval == -1) {
        //spinlock
        while (q->_cts_signal != type);
        return (q->_cts_signal == Q_END);
    }
    while (q->_cts_signal != type) {
        yeild(interval);
    }
    return (q->_cts_signal == Q_END);
}

int wait_until_stc(queue * q, int type, int interval) {
    if (interval == -1) {
        //spinlock
        while (q->_stc_signal != type);
        return (q->_stc_signal == Q_END);
    }
    while (q->_stc_signal != type) {
        yeild(interval);
    }
    return (q->_stc_signal == Q_END);
}

int write_stc(queue * q, char * text, int len) {
    q->_stc_signal = Q_SYNC;
    if (wait_until_stc(q, Q_ACK, 0))
        return -1;
    int progress = len;
    while (progress > 0) {
        memcpy(q->client, text + (len - progress), Q_SIZE);
        q->_stc_signal = Q_WRITE;
        if (wait_until_stc(q, Q_DONE, 0))
            return (len - progress);
        progress -= Q_SIZE;
    }
    q->_stc_signal = Q_END;
    return 0;
}

int write_cts(queue * q, char * text, int len) {
    int progress = len;
    q->_cts_signal = Q_SYNC;
    if (wait_until_cts(q, Q_ACK, 0))
        return -1;
    while (progress > 0) {
        memcpy(q->server, text + (len - progress), Q_SIZE);
        q->_cts_signal = Q_WRITE;
        if (wait_until_cts(q, Q_DONE, 0))
            return (len - progress);
        progress -= Q_SIZE;
    }
    q->_cts_signal = Q_END;
    return 0;
}

int read_cts(queue * q, char * text, int len) {
    int progress = 0;
    if (wait_until_cts(q, Q_SYNC, 0))
        return -1;
    q->_cts_signal = Q_ACK;
    if (wait_until_cts(q, Q_WRITE, 0))
        return -1;
    while (progress < len) {
        memcpy(text + progress, q->client, min(len - progress, Q_SIZE));
        progress += Q_SIZE;
        q->_cts_signal = Q_DONE;
        if (wait_while_cts(q, Q_DONE, 0))
            return progress;
    }
    q->_cts_signal = Q_END;
    return 0;
}

int read_stc(queue * q, char * text, int len) {
    int progress = 0;
    if (wait_until_stc(q, Q_SYNC, 0))
        return -1;
    q->_stc_signal = Q_ACK;
    if (wait_until_stc(q, Q_WRITE, 0))
        return -1;
    while (progress < len) {
        memcpy(text + progress, q->client, min(len - progress, Q_SIZE));
        progress += Q_SIZE;
        q->_stc_signal = Q_DONE;
        if (wait_while_stc(q, Q_DONE, 0))
            return progress;
    }
    q->_stc_signal = Q_END;
    return 0;
}

void init_queue(queue ** q, shared_memory * mem, unsigned int pos) {
    *q = (queue *)((*mem).memory) + (pos*sizeof(queue));
    //dont want to zero it and wipe signals but do want to restrict it to valid signals
    (*q)->_cts_signal &= Q_RANGE;
    (*q)->_stc_signal &= Q_RANGE;
    //could set buffer but you should do that yourself
}
