#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void) {

    int pid;
    if((pid=fork())==0){
        /* Child */
        while(1);
    }
    else{
        /* Parent */
        kill(pid, SIGKILL); // Kill the child, signal no=9
        printf("send a singal to the child\n");
        wait(&pid);
        printf("Death of child\n");
    }

    return 0;
}