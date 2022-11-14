/*
 * errors2.c -- A lot of bad system calls
 */

main()
{
	char ch[80];
	int i, pipefds[2];

	printf("Yo!\n");

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

	if (fork() == 0) {
		exit(127);
	}
	else {
		wait(&i);
		printf("Child returned %d.\n", i);
		wait(0);
	}
	perror("Seventh Error");

	for (i=0; i<200; i++) {
		pipe(pipefds);
	}
	perror("Eighth error");

	write(pipefds[0], &i, 4);
	perror("Ninth error");

	sbrk(100000000);
	perror("Tenth error");

	printf("If ten errors appear, test successful.\n");
}
