#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    int i;
    int pid;
    int child;
    char **child_argv;

    child = fork();
    if(child < 0) {
	printf("fork failed\n");
	exit(1);
    }

    if(child == 0) {
	printf("child calling exec on argstack\n");
	child_argv = (char **)malloc(6*sizeof(char *));
	if(child_argv == NULL) {
		printf("child malloc failed for exeve\n");
		exit(1);
	}
	child_argv[0] = "argstack";
	child_argv[1] = "I";
	child_argv[2] = "am";
	child_argv[3] = "the";
	child_argv[4] = "child";
	child_argv[5] = NULL;
	execve("argstack",child_argv,NULL);
	printf("child execve failed\n");
	exit(1);
    } else {
	pid = wait();
    	for(i=0; i < 100; i++) {
       		printf("parent i: %d\n",i);
    	}
    }

    return(0);
}


