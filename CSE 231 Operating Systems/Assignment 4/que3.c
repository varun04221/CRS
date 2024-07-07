#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wpointer-to-int-cast"


sem_t sem_bridge;
sem_t sem_left, sem_right;
pthread_mutex_t mutex;

int lcount = 0, rcount = 0;

void* left(void* args);
void* right(void* args);
void print(int dir, int carid);


int main() {
    int carleft, carright;

    printf("Enter the number of cars from the left: ");
    scanf("%d", &carleft);
    printf("Enter the number of cars from the right: ");
    scanf("%d", &carright);

    sem_init(&sem_bridge, 0, 5);
    sem_init(&sem_left, 0, 5);
    sem_init(&sem_right, 0, 5);
    pthread_mutex_init(&mutex, NULL);

    pthread_t threads[carleft+carright];
    int carids[carleft+carright];

    for (int i = 0; i < carleft; ++i) {
        carids[i] = i + 1;
        pthread_create(&threads[i], NULL, left, carids[i]);
    }

    for (int i = 0; i < carright; ++i) {
        carids[i + carleft] = i + 1;
        pthread_create(&threads[i + carleft], NULL, right, carids[i + carleft]);
    }

    for (int i = 0; i < carleft + carright; ++i) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem_bridge);
    sem_destroy(&sem_left);
    sem_destroy(&sem_right);
    pthread_mutex_destroy(&mutex);

    return 0;
}

void* right(void* args) {

    int carid = ((int)args);

    sem_wait(&sem_right);  
    pthread_mutex_lock(&mutex);
    sem_wait(&sem_bridge);  
    print(1, carid);
    sem_post(&sem_bridge);  
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_right);
    pthread_exit(NULL);
}


void print(int dir, int carid) {
    printf("Car %d from %s side is traveling on the bridge.\n", carid, (dir == 0) ? "Left" : "Right");
    sleep(1);  
}

void* left(void* args) {
 
    int carid = ((int)args);

    sem_wait(&sem_left);  
    pthread_mutex_lock(&mutex);
    sem_wait(&sem_bridge);  
    print(0, carid);
    sem_post(&sem_bridge);  
    pthread_mutex_unlock(&mutex);
    sem_post(&sem_left);
    pthread_exit(NULL);
}

