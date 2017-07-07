#ifndef QUEUE_H
#define QUEUE_H
#include "cross_platform.h"
//bigger less cpu
//smaller less mem footprint
#define Q_SIZE 64


#define Q_DONE 1
#define Q_READ 2
#define Q_WRITE 4

#define Q_END 8

#define Q_SYNC 16
#define Q_ACK 32

#define Q_RANGE (1 | 2 | 4 | 8 | 16 | 32)

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
int wait_while_cts(queue * q, int type, int interval);
int wait_while_stc(queue * q, int type, int interval);
//returns when signal == type
int wait_until_cts(queue * q, int type, int interval);
int wait_until_stc(queue * q, int type, int interval);

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