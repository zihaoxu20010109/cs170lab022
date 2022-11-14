/*
 * This program is a cpu bound program that adds up the first 2999
 * numbers and returns the result to the system through the exit
 * system call.
 */

#include <stdio.h>
main()
{
    int i, count;
    int pid;

    pid = getpid();

    for (count = i = 0; i < 5; i++) {
	count += i; 
	printf("pid: %d, i: %d\n",pid,i);
    }
    exit(0);
}
