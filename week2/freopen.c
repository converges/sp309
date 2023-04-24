#include <stdio.h>

int main(void) {

	freopen("myfile.txt", "w", stdout);
	printf("This sentence is redirected to a file.");
	fclose(stdout);

	return 0;
}
