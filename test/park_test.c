# include <stdio.h>
# include <string.h>

main()
{
  int pipefd[2];
  int pid;
  int i, line;
  char s[1000];


  if (pipe(pipefd) < 0) {
    perror("pipe");
    exit(1);
  }

  pid = fork();

  if (pid > 0) {
    close(pipefd[0]);
    while(fgets(s, 10, stdin) != NULL) {
      write(pipefd[1], s, strlen(s));
    }
    printf("closing pipefd[1]\n");
    close(pipefd[1]);
  } else {
    i = 0;
    line = 1;
    close(pipefd[1]);
    while(read(pipefd[0], s+i, 1) == 1) {
      if (s[i] == '\n') {
        s[i] = '\0';
        printf("%6d  %s\n", line, s);
        line++;
        i = 0;
      } else {
        i++;
      }
    }
    close(pipefd[0]);
  }
}
  


