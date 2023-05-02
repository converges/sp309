#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "linkedlist.h"

void *producer(void*);
void *consumer(void*);

/* Declare a shared linked list */
struct LinkedList buffer;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

int main(void) {

    ListInit(&buffer);

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, producer, NULL);
    pthread_create(&threads[1], NULL, consumer, NULL);

    for(int i=0; i<2; i++) {
        pthread_join(threads[i], NULL);
    }

    /* Clear all the Nodes */
    freeAllNode(&buffer);

    return 0;
}

void *producer(void *v) {

    for(int i=0; i<1000; i++) {
        pthread_mutex_lock(&mutex);
        if (isFull(&buffer)) {
            /* BUFFER FULL ! */
            pthread_cond_wait(&buffer_has_space, &mutex);
        }

        insertItem(&buffer, i);
        usleep(100000); // 0.1 seconds

        pthread_cond_signal(&buffer_has_data);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer (void *v) {

    int data;

    for(int i=0; i<1000; i++) {
        pthread_mutex_lock(&mutex);
        if (isEmpty(&buffer)) {
            /* BUFFER EMPTY ! */
            pthread_cond_wait(&buffer_has_data, &mutex);
        }

        data = getItem(&buffer);
        usleep(100000); // 0.1 seconds 

        pthread_cond_signal(&buffer_has_space);
        pthread_mutex_unlock(&mutex);

        printf("data = %d\n", data);
    }
}