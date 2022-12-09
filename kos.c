/*
 * kos.c -- starting point for student's os.
 *
 */
#include <stdlib.h>

#include "kt.h"
#include "simulator_lab2.h"
#include "simulator.h"
#include "scheduler.h"
#include "syscall.h"
#include "console_buf.h"

void KOS()
{
	initialize_sema();
	initialize_cons_sema();

	
	initialize_scheduler();

	kt_fork(initialize_user_process, (void*)kos_argv);

	buffer = init_buff();
	kt_fork((void*)(cons_to_buff), (void*)(buffer));
	scheduler();
}
