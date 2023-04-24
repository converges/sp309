#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {

	char **ptr;
	char *nptr;
	extern char **environ;

		for (ptr = environ; *ptr != 0; ptr++) {
			/* print all environments; */
			printf("%s \n", *ptr);
		}

	nptr = getenv("HOME");
	printf("HOME = %s \n", nptr);

	nptr = getenv("SHELL");
	printf("SHELL = %s \n", nptr);
	
	nptr = getenv("PATH");
	printf("PATH = %s \n", nptr);

	exit(0);

	return 0;
}

