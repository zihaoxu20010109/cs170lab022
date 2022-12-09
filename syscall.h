#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdlib.h>

#include "console_buf.h"
#include "simulator.h"
#include "scheduler.h"
#include "kt.h"
#include "dllist.h"
#include "syscall.h"
#include <errno.h>

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

extern void initialize_sema();
extern void *do_write(void *arg);
extern void *do_read(void *arg);
extern void syscall_return(struct PCB *pcb, int value);
extern void ioctl(void *arg);
extern void fstat(void *arg);
extern void getpagesize(void *arg);
extern void do_sbrk(void *arg);
extern bool ValidAddress(void *arg);
extern void do_execve(void *arg);
extern void execve_return(struct PCB *pcb, int value);

extern void getpid(void *arg);
extern void do_fork(void *arg);
void finish_fork(void *arg);
extern void do_exit(void *arg);
extern void getdtablesize(void *arg);
extern void do_close(void *arg);
extern void do_wait(void * arg);
extern void get_ppid(void * arg);

extern void do_dup(void * arg);
extern void do_dup2(void * arg);
extern void do_pipe(void *arg);
extern void handle_interrupt(struct PCB *pcb);
extern void program_ready(struct PCB *pcb);
extern kt_sem writeok;
extern Dllist readyq;
#endif
