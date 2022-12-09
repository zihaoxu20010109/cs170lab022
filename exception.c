/*
 * exception.c -- stub to handle user mode exceptions, including system calls
 *
 * Everything else core dumps.
 *
 * Copyright (c) 1992 The Regents of the University of California. All rights
 * reserved.  See copyright.h for copyright notice and limitation of
 * liability and disclaimer of warranty provisions.
 */
#include <stdlib.h>

#include "dllist.h"
#include "simulator.h"
#include "scheduler.h"
#include "kos.h"
#include "kt.h"
#include "syscall.h"
#include "console_buf.h"
extern struct PCB *curr;
extern Dllist readyq;
extern bool is_noop;

void exceptionHandler(ExceptionType which)
{
	int type, r5, r6, r7, newPC;
	//int buf[NumTotalRegs];

	if(is_noop == FALSE){
		examine_registers(curr->my_registers);
	}
	type = curr->my_registers[4];
	r5 = curr->my_registers[5];
	r6 = curr->my_registers[6];
	r7 = curr->my_registers[7];
	newPC = curr->my_registers[NextPCReg];

	/*
	 * for system calls type is in r4, arg1 is in r5, arg2 is in r6, and
	 * arg3 is in r7 put result in r2 and don't forget to increment the
	 * pc before returning!
	 */

	switch (which)
	{
	case SyscallException:
		/* the numbers for system calls is in <sys/syscall.h> */
		switch (type)
		{
		case 0:
			/* 0 is our halt system call number */
			DEBUG('e', "Halt initiated by user program\n");
			SYSHalt();
		case SYS_exit:
			/* this is the _exit() system call */
			kt_fork((void *)do_exit, (void *)curr);
			DEBUG('e', "exit() system call\n");
			//printf("Program exited with value %d.\n", r5);
			break;
		case SYS_write:
			/* this is the write() system call */
			kt_fork((void *)do_write, (void *)curr);
			DEBUG('e', "SYS_write() system call\n");
			break;
		case SYS_read:
			/* this is the read() system call */
			kt_fork((void *)do_read, (void *)curr);
			DEBUG('e', "SYS_read() system call\n");
			break;
		case SYS_ioctl:
			/* this is the ioctl() system call */
			DEBUG('e', "SYS_ioctl() system call\n");
			kt_fork((void*)ioctl, (void *)curr);
			break;
		case SYS_fstat:
			kt_fork((void*)fstat, (void *)curr);
			DEBUG('e', "SYS_fstat() system call\n");
			break;
		case SYS_getpagesize:
			kt_fork((void*)getpagesize, (void *)curr);
			DEBUG('e', "getpagesize system call\n");
			break;
		case SYS_sbrk:
			kt_fork((void*)do_sbrk, (void *)curr);
			DEBUG('e',"sbrk() system call\n");
			break;
		case SYS_execve:
			kt_fork((void *)do_execve, (void *)curr);
			DEBUG('e',"execve() system call\n");
			break;
		case SYS_getpid:
			kt_fork((void *)getpid, (void *)curr);
			DEBUG('e', "get_pid() system call\n");
			break;
		case SYS_fork:
			kt_fork((void *)do_fork, (void *)curr);
			DEBUG('e', "fork() system call\n");
			break;
		case SYS_getdtablesize:
			kt_fork((void*)getdtablesize, (void *)curr);
			DEBUG('e', "getdtablesize system call\n");
			break;
		case SYS_close:
			kt_fork((void*)do_close, (void *)curr);
			DEBUG('e', "close system call\n");
			break;	
		case SYS_wait:
			kt_fork((void*)do_wait, (void *)curr);
			DEBUG('e', "wait system call\n");
			break;
		case SYS_getppid:
			kt_fork((void *)get_ppid, (void *)curr);
			DEBUG('e', "get ppid system call\n");
			break;	
		case SYS_pipe:
			kt_fork((void *)do_pipe, (void *)curr);
			DEBUG('e', "do pipe system call\n");
			break;
		case SYS_dup:
			kt_fork((void *)do_dup, (void *)curr);
			DEBUG('e', "do dup system call\n");
			break;
		case SYS_dup2:
			kt_fork((void *)do_dup2, (void *)curr);
			DEBUG('e', "do dup2 system call\n");
			break;	
		default:
			DEBUG('e', "Unknown system call\n");
			SYSHalt();
			break;
		}
		break;
	case PageFaultException:
		DEBUG('e', "Exception PageFaultException\n");
		break;
	case BusErrorException:
		DEBUG('e', "Exception BusErrorException\n");
		break;
	case AddressErrorException:
		DEBUG('e', "Exception AddressErrorException\n");
		break;
	case OverflowException:
		DEBUG('e', "Exception OverflowException\n");
		break;
	case IllegalInstrException:
		DEBUG('e', "Exception IllegalInstrException\n");
		break;
	default:
		printf("Unexpected user mode exception %d %d\n", which, type);
		exit(1);
	}
	scheduler();
}

void interruptHandler(IntType which)
{
	if (is_noop == FALSE)
	{
		examine_registers(curr->my_registers);
		dll_append(readyq, new_jval_v((void *)curr));
	}

	switch (which)
	{
	case ConsoleReadInt:
		DEBUG('e', "ConsoleReadInt interrupt\n");
		V_kt_sem(consoleWait);
		break;
	case ConsoleWriteInt:
		DEBUG('e', "ConsoleWriteInt interrupt\n");
		V_kt_sem(writeok);
		//scheduler();
		break;
	case TimerInt:
		DEBUG('e', "TimerInt interrupt\n");
		break;
	default:
		DEBUG('e', "Unknown interrupt\n");
		//scheduler();
		break;
	}
	if(curr != NULL){ //user code was just run
		examine_registers(curr->my_registers);
		program_ready(curr); //TODO: duplicates
	}

	kt_fork((void (*)(void*))handle_interrupt,curr);
	scheduler();
}
