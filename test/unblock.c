/*
 * unblock.c --
 */
#include <stdio.h>

#define Syscall(x) exit(x)

#define BURN 1000
#define FULL 8*1024

main(int argc, char *argv[])
{
	int i, rc, num_procs, fd[2];
	char wr_buf[FULL];
	char rd_buf[FULL];

	if (argc < 3 || !strlen(argv[1]) || (num_procs=atoi(argv[2])) <= 0) {
		printf("Usage: ./unblock <r|w> <num_procs>\n");
		Syscall(0);
	}

	rc = pipe(fd);
	if (rc != 0) {
		perror("pipe() failed");
		Syscall(0);
	}

	switch (argv[1][0]) {
		case 'r':
		case 'R':
			for (i=0; i<num_procs; ++i) {
				rc = fork();
				if (rc < 0) {
					perror("fork() failed");
					Syscall(0);
				} else if (rc == 0) {
					close(fd[1]);
					printf("(%d) trying to read from pipe...\n", getpid());

					/* this read should block */
					rc = read(fd[0], rd_buf, 1);

					printf("(%d) read returned %d\n", getpid(), rc);
					exit(0);
				}
			}

			/* burn some cycles */
			for (i=0; i<BURN; ++i)
				close(100);

			/* unblock by closing pipe */
			if (argv[1][0] == 'r') {
				printf("Closing pipe...\n");
				close(fd[1]);
			/* unblock by writing to pipe */
			} else {
				printf("Writing to pipe...\n");
				for (i=0; i<num_procs; ++i)
					write(fd[1], wr_buf, 1);
			}

			for (i=0; i<num_procs; ++i)
				wait();
			break;
		case 'w':
		case 'W':
			/* fill up pipe */
			write(fd[1], wr_buf, FULL);

			for (i=0; i<num_procs; ++i) {
				rc = fork();
				if (rc < 0) {
					perror("fork() failed");
					Syscall(0);
				} else if (rc == 0) {
					close(fd[0]);
					printf("(%d) trying to write to pipe...\n", getpid());

					/* this write should block */
					rc = write(fd[1], wr_buf, 1);

					if (rc < 0)
						perror("write to pipe");

					printf("(%d) write returned %d\n", getpid(), rc);
					exit(0);
				}
			}

			/* burn some cycles */
			for (i=0; i<BURN; ++i)
				close(100);

			/* unblock by closing pipe */
			if (argv[1][0] == 'w') {
				printf("Closing pipe...\n");
				close(fd[0]);
			/* unblock by reading from pipe */
			} else {
				printf("Reading from pipe...\n");
				for (i=0; i<num_procs; ++i)
					read(fd[0], rd_buf, 1);
			}

			for (i=0; i<num_procs; ++i)
				wait();
			break;

			break;
		default:
			printf("Invalid argument '%c'.\n", argv[1][0]);
	}

	Syscall(0);
}
