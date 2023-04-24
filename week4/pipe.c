#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {

	int fd[2];
	int input, output;
	int n;
	char buf[10];

	pipe(fd); // fd[0] for reading, fd[1] for writing
	
	if (fork() == 0) { // child process executes
		input = open("input.dat", 0);
		close(fd[0]);

		while ( (n=read(input, buf, 10)) != 0) {
			write(fd[1], buf, n);
		}

		close(input);
		close(fd[1]);
		exit(0);
	}
	else { // parent process executes
		close(fd[1]);
		output = creat("output.dat", 0666);

		while ( (n=read(fd[0], buf, 10)) != 0) {
			write(output, buf, n);
		}

		close(output);
		close(fd[0]);

		int status;
		wait(&status);
	}

	return 0;
}
