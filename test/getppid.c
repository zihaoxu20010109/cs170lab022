/* getppid.c -- this program prints out a pid and a ppid, and then forks
   and repeats.  The child calls printf twice, which means taht by the
   time it finishes, it should be an orphan.
 */

main()
{
  int i;

  printf("1 My pid is %d.  My parents pid is %d\n", getpid(), getppid());
  i = fork();
  if (i == 0) {
  printf("2 My pid is %d.  My parents pid is %d.  Fork returned %d\n", 
      getpid(), getppid(), i);
  }
  printf("3 My pid is %d.  My parents pid is %d.  Fork returned %d\n", 
      getpid(), getppid(), i);
}
