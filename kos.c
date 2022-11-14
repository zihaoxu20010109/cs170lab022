/*
 * kos.c -- starting point for student's os.
 *
 */
#include <stdlib.h>

#include "kt.h"
#include "simulator.h"
#include "scheduler.h"
#include "syscall.h"
#include "console_buf.h"
struct PCB* processes[8] = {NULL};
void KOS()
{
	initialize_sema();
	initialize_cons_sema();
	initialize_scheduler();
	rbtree = make_jrb();
	kt_fork(initialize_user_process, (void*)kos_argv);

	buffer = init_buff();
	kt_fork((void*)(cons_to_buff), (void*)(buffer));

	scheduler();
}
