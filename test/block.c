/*
 * block.c -- check blocking behavior of KOS pipes
 *
 * read() should block when the pipe is empty when the read is initiated and
 * there are open writers
 *
 * write() should block when the pipe is full and there are open readers
 *
 */

#ifdef FORLINUX
#define Syscall(x) exit(x)
#endif

main(int argc, char *argv[])
{
	int i, rc, fd[2];
	char wr_buf[100];
	char rd_buf[100];

	if (argc < 2 || !strlen(argv[1])) {
		printf("Usage: ./block <r|w>\n");
		exit(1);
	}

	rc = pipe(fd);
	if (rc != 0) {
		perror("pipe() failed");
		exit(1);
	}

	switch (argv[1][0]) {
		case 'r':
		case 'R':
			printf("About to read from an empty pipe...\n");
			read(fd[0], rd_buf, 10);
			break;
		case 'w':
		case 'W':
			printf("About to write to full pipe...\n");
			for (i=0; 1; i+=100) {
				printf("Writing 100 more chars...\n");
				write(fd[1], wr_buf, 100);
			}
			break;
		default:
			printf("Invalid argument '%c'.\n", argv[1][0]);
	}

	printf("Hey! Why didn't we block?\n");

	exit(1);
}
