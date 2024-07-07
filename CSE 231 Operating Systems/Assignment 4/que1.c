#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

pthread_mutex_t forks[5];
pthread_mutex_t bowls[2];
pthread_cond_t fork_conds[5];
pthread_cond_t bowl_conds[2];

#define NUM_PHILOSOPHERS 5




void* philosopher(void* args) {
    int id = (intptr_t)args;

    while (1) {
        //printf("Philosopher %d is thinking.\n", id);

        pthread_mutex_lock(&forks[id]);
        pthread_mutex_lock(&forks[(id + 1) % NUM_PHILOSOPHERS]);

        if (pthread_mutex_trylock(&bowls[0]) == 0) {
            printf("Philosopher %d is eating.\n", id);
            sleep(2);
            pthread_mutex_unlock(&bowls[0]);
        } else if (pthread_mutex_trylock(&bowls[1]) == 0) {
            printf("Philosopher %d is eating.\n", id);
            sleep(2);
            pthread_mutex_unlock(&bowls[1]);
        }
        pthread_mutex_unlock(&forks[id]);
        pthread_mutex_unlock(&forks[(id + 1) % NUM_PHILOSOPHERS]);
        printf("Philosopher %d is thinking.\n", id);
        sleep(2);
    }
}


int main() {
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
        pthread_cond_init(&fork_conds[i], NULL);
    }

    for (int i = 0; i < 2; i++) {
        pthread_mutex_init(&bowls[i], NULL);
        pthread_cond_init(&bowl_conds[i], NULL);
    }

    pthread_t philosopher_threads[NUM_PHILOSOPHERS];
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&philosopher_threads[i], NULL, philosopher, (void*)(intptr_t)i);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosopher_threads[i], NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
        pthread_cond_destroy(&fork_conds[i]);
    }

    for (int i = 0; i < 2; i++) {
        pthread_mutex_destroy(&bowls[i]);
        pthread_cond_destroy(&bowl_conds[i]);
    }

    return 0;
}