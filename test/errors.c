/*
 * errors.c -- A lot of bad system calls
 */

main()
{
	char ch[80];
	read(-1, ch, 12);
	perror("First error");
	read(0, -1, 12);
	perror("Second error");
	read(0, ch, -1);
	perror("Third error");

	write(-1, ch, 12);
	perror("Fourth error");
	write(1, 1048570, 12);
	perror("Fifth error");
	write(-1, ch, -2);
	perror("Sixth error");
	write(1, "If six errors appear, test successful.\n", 39);
}
