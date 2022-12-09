#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "jrb.h"
#include "memory.h"
#include "kt.h"
#include "kos.h"
#include "dllist.h"

struct PCB{
    int my_registers[NumTotalRegs];
    void* sbrk_ptr;
    //control the memory limit of the pcb
    int limit;
    int base;

    unsigned short pid;
    int exit_value;
    int memory_num;

    struct PCB* parent;
    
    kt_sem waiters_sem;
    Dllist waiters;
    JRB children;

    struct Fd *fd[64];
};

//pid stuff
extern int curpid;
extern JRB rbtree;

extern struct PCB* init;
extern struct PCB *curr;

void initialize_scheduler();

void* initialize_user_process(void* arg);

void scheduler();

int perform_execve(struct PCB *pcb, char *filename, char **pcb_argv);

int get_new_pid();
void destroy_pid(int pid);

struct Pipe{
    char* buffer;

    int read_count;
    int write_count;

    kt_sem read;
    kt_sem write;
    kt_sem nelement;
    kt_sem space_available;

    int write_head;
    int read_head;

    int writer_in_use;
};

struct Fd{
    bool console;
    bool open;
    //default as FALSE
    bool isread;
    int reference_count;
    struct Pipe* my_pipe;
};

#endif
