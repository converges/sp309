#include <stdio.h>
#include <stdlib.h>

struct {
	short count;
	char sample;
	long total;
	float numeric[4];
} object;

FILE *stream;

int main(int argc, char *argv[]) {

	object.count = 1;
	object.sample = 's';
	object.total = 10000000000;
	object.numeric = {1.5, 2.1, 7.3, 9.9};

	if (fwrite(&new_obj, sizeof(object), 1, stream) != 1) {
		perror("fwrite error");
	}

	return 0;
}
