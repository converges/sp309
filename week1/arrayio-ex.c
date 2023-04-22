#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

int main(void) {
	
	int i;
	int sample_array[ARRAY_SIZE];
	FILE *stream;

	if ( (stream=fopen(argv[1], "w")) == NULL ) {
		perror(argv[1]);
		return 1;
	}

	if ( fwrite(sample_array, sizeof(int), ARRAY_SIZE, stream) != ARRAY_SIZE) {
		perror("fwrite error");
		return 2;
	}

	return 0;
}
