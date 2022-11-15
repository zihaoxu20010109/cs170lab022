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
#include "memory.h"
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

extern void initialize_sema();
extern void *do_write(void *arg);
extern void *do_read(void *arg);
extern void syscall_return(struct PCB *pcb, int value);
extern void ioctl(void* arg);
extern void fstat(void* arg);
extern void do_sbrk(void *arg);
extern void getpagesize(void* arg);
extern void getdtablesize(void* arg);
extern bool ValidAddress(void *arg);
extern void getpid(void *arg);
extern void do_execve(void *arg);
extern void mydo_fork(void *arg);
extern void *finish_fork(void *arg);
extern void myown_exit(void *arg);
extern bool empty_memory(int* ptr);
extern void do_wait(void *arg);
extern void do_close(void *arg);
extern void get_ppid(void *arg);
extern kt_sem writeok;
extern Dllist readyq;
#endif
