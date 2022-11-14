/*
 * This program writes out the the strings in the argv array.
 * It uses argc to determine the number of elements. Also if envp
 * is not NULL, the first index of the array is written out.
 */

#include <stdio.h>

main(int argc, char **argv, char **envp)
{
	int i;
	char buf[256];


	sprintf(buf, "&argc is -->%u<--\n", &argc);
	write(1, buf, strlen(buf));
	sprintf(buf, "argc is -->%u<--\n", argc);
	write(1, buf, strlen(buf));
	sprintf(buf, "argv is -->%u<--\n", argv);
	write(1, buf, strlen(buf));
	sprintf(buf, "envp is -->%u<--\n", envp);
	write(1, buf, strlen(buf));
	for (i=0; i<argc; i++) {
		sprintf(buf, "argv[%d] is (%d) -->%s<--\n", i, argv[i], argv[i]);
		write(1, buf, strlen(buf));
	}
        if (envp != NULL) {
  	  sprintf(buf, "envp[0] is -->%s<--\n", envp[0]);
	  write(1, buf, strlen(buf));
        }
	exit(&argc);
}
