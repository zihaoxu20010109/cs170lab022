/*
 * This program is a cpu bound program that adds up the first 2999
 * numbers and returns the result to the system through the exit
 * system call.
 */

main()
{
    int i, count;

    for (count = i = 0; i < 50000; i++)
	count += i; 
    return count;
}
