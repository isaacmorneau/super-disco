#ifndef BUFFER_H
#define BUFFER_H
#include "cross_platform.h"

#define B_SIZE 64

#define B_DONE 1
#define B_NEXT 2

typedef struct buffer {
    //signal set to Q_DONE when handled
    int _signal;
    int total;
    int data[B_SIZE];
} buffer;

//0 is kind wait -1 is spinlock >0 is in msec for checks
//returns when signal != type
void wait_while(buffer * q, int type, int interval);
//returns when signal == type
void wait_until(buffer * q, int type, int interval);
//keep writing until length met
int write_buffer(buffer * q, char * text, int len);
//read until length or end found
int read_buffer(buffer * q, char * text, int len);

void init_buffer(buffer ** q, shared_memory * mem, unsigned int pos);

#endif
