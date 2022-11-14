/*
 * exit.c -- check whether exit closes empty file descriptors
 */

#define Syscall(x) exit(x)

main(int argc, char *argv[])
{
	int i, j, rc, num_pipes, *fd;
	char wr_buf[100];
	char rd_buf[100];
	char c;

	if (argc < 2 || (num_pipes=atoi(argv[1])) <= 0) {
		printf("Usage: ./exit <num_pipes>\n");
		Syscall(0);
	}

	fd = (int *)malloc(num_pipes * 2 * sizeof(int));

	for (i=0; i<num_pipes; ++i) {
		rc = pipe(&fd[i*2]);
		if (rc != 0) {
			perror("pipe() failed");
			Syscall(0);
		}
	}

	rc = fork();

	if (rc < 0) {
		perror("fork() failed");
		Syscall(0);
	} else if (rc == 0) {
		printf("Child exiting...\n");
		exit(0);
	} else {
		for (i=0; i<num_pipes; ++i)
			close(fd[2*i+1]);
		rc = read(fd[2*(num_pipes-1)+0], rd_buf, 1);
		printf("read() from fd %d returned %d\n", fd[2*(num_pipes-1)+0], rc);
	}

//	for(i=0; i < 10000; i++);
	Syscall(0);
}
