#include <stdio.h>
#include <pthread.h>
#include "linkedlist.h"

void *producer(void*);
void *consumer(void*);

#define MAX_BUF 100

struct LinkedList* buffer = {MAX_BUF, NULL, NULL};

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_has_space = PTHREAD_COND_INITIALIZER;
pthread_cond_t buffer_has_data = PTHREAD_COND_INITIALIZER;

int main(void) {

    pthread_t threads[2];
    pthread_create(&threads[0], NULL, producer, NULL);
    pthread_create(&threads[1], NULL, consumer, NULL);

    for(int i=0; i<2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

void *producer(void *v) {


    for(int i=0; i<1000; i++) {
        pthread_mutex_lock(&mutex);
        if(isFull(buffer)) {
            /* BUFFER FULL ! */
            pthread_cond_wait(&buffer_has_space, &mutex);
        }
        insertItem(buffer, i);

        pthread_cond_signal(&buffer_has_data);
        pthread_mutex_unlock(&mutex);
    }
}

void *consumer (void *v) {

    int data;

    for(int i=0; i<1000; i++) {
        pthread_mutex_lock(&mutex);
        if (isEmpty(buffer)) {
            /* BUFFER EMPTY ! */
            pthread_cond_wait(&buffer_has_data, &mutex);
        }
        data = getItem(buffer);
        pthread_cond_signal(&buffer_has_space);
        pthread_mutex_unlock(&mutex);
        printf("data = %d\n", data);
    }
}