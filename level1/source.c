#include <stdio.h>
#include <stdlib.h>

int run(void)
{
	fwrite("Good... Wait what?\n", 19, 1, stdout);
	system("/bin/sh");
	return(0);
}

int main (void)
{
	char str[64];

	gets(str);
	return(0);
}
