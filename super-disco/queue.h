#ifndef QUEUE_H
#define QUEUE_H
#include "cross_platform.h"
//bigger less cpu
//smaller less mem footprint
#define Q_SIZE 128


#define Q_DONE 0
#define Q_READ 1
#define Q_WRITE 2
#define Q_END 3

typedef struct queue {
    //signal set to Q_DONE when handled
    int _cts_signal;
    int _stc_signal;

    int client[Q_SIZE];
    int server[Q_SIZE];
} queue;

//0 is kind wait -1 is spinlock >0 is in msec for checks
//returns when signal != type
//returns 1 if Q_END hit
int wait_for_cts(queue * q, int type, int interval);
int wait_for_stc(queue * q, int type, int interval);

//keep writing until length met
//returns amout wrote if Q_END hit
int write_stc(queue * q, char * text, int len);
int write_cts(queue * q, char * text, int len);

//read until length or end found
//returns amount read if Q_END hit
int read_cts(queue * q, char * text, int len);
int read_stc(queue * q, char * text, int len);

void init_queue(queue ** q, shared_memory * mem, unsigned int pos);
#endif