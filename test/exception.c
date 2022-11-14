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
	printf("r5: %d", r5;);
	printf("r6: %d", r6;);
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
			DEBUG('e', "_exit() system call\n");
			printf("Program exited with value %d.\n", r5);
			SYSHalt();
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
		// noop();

		break;
	case ConsoleWriteInt:
		DEBUG('e', "ConsoleWriteInt interrupt\n");
		// noop();
		V_kt_sem(writeok);
		scheduler();
		break;
	default:
		DEBUG('e', "Unknown interrupt\n");
		// noop();
		scheduler();
		break;
	}
}
