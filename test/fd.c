/*
 * fd.c -- test file descriptor allocation/deallocation
 */

#define Syscall(x) exit(x)

main()
{
	int i, rc, pipe1[2], pipe2[2], pipe3[2];

	rc = pipe(pipe1);
	printf("pipe1: status = %d, fd[0] = %d, fd[1] = %d\n", rc, pipe1[0], pipe1[1]);

	rc = pipe(pipe2);
	printf("pipe2: status = %d, fd[0] = %d, fd[1] = %d\n", rc, pipe2[0], pipe2[1]);

	rc = close(pipe1[1]);
	printf("closed fd %d returned %d\n", pipe1[1], rc);

	rc = close(pipe2[0]);
	printf("closed fd %d returned %d\n", pipe2[0], rc);

	rc = pipe(pipe3);
	printf("pipe3: status = %d, fd[0] = %d, fd[1] = %d\n", rc, pipe3[0], pipe3[1]);

//	for(i=0; i < 10000; i++);

	Syscall(0);
}
