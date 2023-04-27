#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

	char cmdstr[1024];

	strcpy(cmdstr, "/bin/ls ");

	for (int i=1; i<argc; i++) {
		strcat(cmdstr, argv[i]);
		strcat(cmdstr, " ");
	}

	fprintf(stdout, "cmdstr = \"%s\"\n", cmdstr);

	system(cmdstr);

	exit(0);

	return 0;
}
