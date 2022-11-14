/*
 * This program writes out everybody's favorite phrase for a first
 * program. Afterwards it writes out the value returned by
 * the write system call.
 */

main()
{
  int i;
  char s[100];

  i = write(1, "Hello world\n", strlen("Hello world\n"));
  sprintf(s, "the write statement just returned %d\n", i);
  write(1, s, strlen(s));
  return i;

}
