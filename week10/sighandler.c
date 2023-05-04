#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static void sigcatcher(int);
void (*was)(int);


int main(void){

	if((was=signal(SIGINT,sigcatcher))==SIG_ERR){
		perror("SIGINT");
		exit(1);
	}
	while(1) pause();

	return 0;
}

static void sigcatcher(int signo){

	switch(signo) {
		case SIGINT:
			printf("PID %d caught signal SIGINT.\n", getpid());
			signal(SIGINT, was); // Dependent on Linux, bsd versions
			break;
		default:
			fprintf(stderr, "something wrong\n");
			exit(1);
	}
}
