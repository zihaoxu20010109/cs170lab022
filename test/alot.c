#include <stdio.h>

#define ROWS	20
#define COLS	41

main()
{
	int i, j;
	char data[ROWS * (COLS + 1)];

	for (i=0; i<ROWS; i++) {
		for (j=0; j<COLS - 1; j++)
			data[i * COLS + j] = '0' + j;
		data[i * COLS + COLS - 1] = '\n';
	}
	return write(1, data, strlen(data));
}
