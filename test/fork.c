/*
 * fork.c -- forks a process and has parent and child print something
 */

#include <stdio.h>

main()
{
  int i;
  char s[100];

  i = fork();

  sprintf(s, "mypid = %d.  fork returned %d\n", getpid(), i);
  write(1, s, strlen(s));
}
