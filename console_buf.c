#include <stdlib.h>
#include "simulator.h"
#include "console_buf.h"
#include "kt.h"
#include "scheduler.h"
#include "jval.h"

kt_sem nelem;
kt_sem nslots;
kt_sem consoleWait;
struct console_buf* buffer;

struct console_buf* init_buff() {
    struct console_buf* b;
    b = malloc(sizeof(struct console_buf));
    b->buff = malloc(256*sizeof(int));
    b->head = 0;
    b->tail = 0;
    b->size = 256;
    return b;
}

void initialize_cons_sema() {
    nelem = make_kt_sem(0);
    nslots = make_kt_sem(256);
    consoleWait = make_kt_sem(0);
}

void cons_to_buff(struct console_buf* buffer) {
    int finish = 0;
    while (finish == 0) {
        P_kt_sem(consoleWait);
        P_kt_sem(nslots);
        char c = (char)console_read();
        V_kt_sem(nelem);
        Jval val = new_jval_i(c);
        dll_append(buffer, val);
        buffer -> buff[(buffer -> tail)%(buffer->size)] = c;
        //increment the tail of buffer by 1
        //The tail and head will be checked by semaphore
        buffer -> tail = (buffer -> tail + 1)%(buffer->size);
    }
    //kt_exit();
}
