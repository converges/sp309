#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

int cnt=0;
static sem_t hsem;

void *Thread1(void *arg);
void *Thread2(void *arg);

int main(int argc, char *argv[]) {

    pthread_t thread1;
    pthread_t thread2;

    if (sem_init(&hsem, 0, 1) < 0) {
        perror("Semaphore Initilization Error\n");
        return 1;
    }
    sem_wait(&hsem);
    pthread_create(&thread1, NULL, Thread1, NULL);
    pthread_join(thread1, NULL);
    sem_post(&hsem);

    sem_wait(&hsem);
    pthread_create(&thread2, NULL, Thread2, NULL);
    pthread_join(thread2, NULL);
    sem_post(&hsem);

    printf("%d\n", cnt);
    sem_destroy(&hsem);

    return 0;
}

void *Thread1(void *arg) {

    int tmp;

    for(int i=0; i<1000; i++) {
        tmp=cnt;
        usleep(1000);
        cnt=tmp+1;
	printf("Thread 1: %d\n", cnt);
    }
    printf("Thread1 End\n");

    return NULL;
}

void *Thread2(void *arg) {

    int tmp;

    for(int i=0; i<1000;i++) {
        tmp=cnt;
        usleep(1000);
        cnt=tmp+1;
	printf("Thread 2: %d\n", cnt);

    }
    printf("Thread2 End\n");

    return NULL;
}
