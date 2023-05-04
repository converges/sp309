#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

static void sig_catcher(int);

int alarmed = 0;

int main(void){
    
    int pid;
    signal(SIGALRM, sig_catcher);
    alarm(3);
    /* Do something */
    sleep(1);
    /* Done */
    while(alarmed == 0){
        printf("After alarm in main\n");
    }
}

void sig_catcher(int signo){
    alarmed = 1;
    alarm(0);
}