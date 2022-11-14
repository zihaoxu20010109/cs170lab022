#include <stdio.h>

char *words[] = {"dog", "rat", "bat", "ant", "cow", "pig", "fox", "hen", "bee", "emu", "eel", "owl"};

#define Syscall(x) exit(x)

main(int argc, char *argv[])
{
	int i, num_dups, rc, pipe1[2], *rd_fd, *wr_fd;
	char rd_buf[256], wr_buf[256];

	if (argc < 2 || (num_dups=atoi(argv[1])) <= 0) {
		printf("Usage: ./dup <num_dups>\n");
		Syscall(0);
	}

	rd_fd = (int *)malloc(num_dups * sizeof(int));
	wr_fd = (int *)malloc(num_dups * sizeof(int));

	rc = pipe(pipe1);
	if (rc != 0) {
		perror("pipe() failed");
		Syscall(0);
	}

	for (i=0; i<num_dups; ++i) {
		rd_fd[i] = dup(pipe1[0]);
		wr_fd[i] = dup(pipe1[1]);
	}

	strcpy(wr_buf, "cat");
	rc = write(pipe1[1], wr_buf, 4);
	printf("wrote '%s' into fd %d (original pipe), returned %d\n", wr_buf, pipe1[1], rc);

	for (i=0; i<num_dups; ++i) {
		strcpy(wr_buf, words[i]);
		rc = write(wr_fd[i], wr_buf, 4);
		printf("wrote '%s' into fd %d, returned %d\n", wr_buf, wr_fd[i], rc);
	}

	for (i=num_dups-1; i>=0; --i) {
		rc = read(rd_fd[i], rd_buf, 4);
		printf("read '%s' from fd %d, returned %d\n", rd_buf, rd_fd[i], rc);
	}

	rc = read(pipe1[0], rd_buf, 4);
	printf("read '%s' from fd %d (original pipe), returned %d\n", rd_buf, pipe1[0], rc);

//	for(i=0; i < 10000; i++);
	Syscall(0);

}
