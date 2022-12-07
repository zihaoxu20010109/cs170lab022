#ifndef CONSOLE_BUF_H
#define CONSOLE_BUF_H

#include "kt.h"

extern struct console_buf* buffer;

struct console_buf {
    int* buff;
    int head;
    int tail;
    int size;
};

struct console_buf* init_buff();
void initialize_cons_sema();
void cons_to_buff();

extern kt_sem nelem;
extern kt_sem nslots;
extern kt_sem consoleWait;

#endif