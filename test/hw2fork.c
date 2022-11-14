/*
 * This program writes out everybody's favorite phrase for a first
 * program. Afterwards it writes out the value returned by
 * the write system call.
 */

main()
{
  int i;
  char s[100];

  i = fork();

  if(i == 0)
  	printf("Hello\n");
  else
	printf("world\n");
}
