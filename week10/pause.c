#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


int alarmed = 0;

void sig_catch(int signo){
    alarmed = 1;
    alarm(0);
}

int main(void){
    int pid;
    int status;

    signal(SIGUSR1, sig_catch);
    if((pid=fork()) == 0){
        /* Child */
        pause(); // It may block forever: why?
        printf("Child wakes up\n");
        exit(0);
    }
    else{
        /* Parent */
        sleep(1);
        kill(pid, SIGUSR1); // send SIGUSR1 to the child process
        wait(&status);
    }

    return 0;
}