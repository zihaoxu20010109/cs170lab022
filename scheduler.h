#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "jrb.h"
#include "memory.h"
#include "kt.h"
#include "kos.h"
#include "dllist.h"
struct PCB{
    int my_registers[NumTotalRegs];
    int base;
    int limit;
    void* sbrk_ptr;
    unsigned short pid;
    int mem_int;
    struct PCB* parent;
    JRB children;
    int exit_value;
    kt_sem waiters_sem;
    Dllist waiters;
};
extern int curpid;
extern JRB rbtree;
extern struct PCB* init;
extern int memory_chunk[8];
void initialize_scheduler();
void* initialize_user_process(void* arg);
int perform_execve(struct PCB* pcb, char* filename, char** argv);
void scheduler();
int get_new_pid();
void destroy_pid(int pid);

#endif
