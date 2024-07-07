#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int num_passengers = 0;
int car_capacity;
sem_t car_full;
sem_t all_aboard;
sem_t car_empty;
sem_t tour_done;


void* car(void* args);
void* passenger(void* args);
void load();
void unload();
void board();
void offboard();

int main() {
    sem_init(&car_full, 0, 0);
    sem_init(&all_aboard, 0, 0);
    sem_init(&car_empty, 0, 1);
    sem_init(&tour_done,0,0);


    printf("Enter the car capacity: ");
    scanf("%d", &car_capacity);

    int MAX_PASSENGERS;
    printf("Enter the total numbers of passangers:");
    scanf("%d",&MAX_PASSENGERS);

    if (MAX_PASSENGERS<car_capacity){
        printf("Total number of passangers cannot be less than car capacity.");
        return 0;
    }

    pthread_t car_thread;
    pthread_create(&car_thread, NULL, car, NULL);

    pthread_t passenger_threads[MAX_PASSENGERS];
    for (int i = 0; i < MAX_PASSENGERS; i++) {
        pthread_create(&passenger_threads[i], NULL, passenger, (void*)(intptr_t)i);
    }

    pthread_join(car_thread, NULL);

    for (int i = 0; i < MAX_PASSENGERS; i++) {
        pthread_join(passenger_threads[i], NULL);
    }

    sem_destroy(&car_full);
    sem_destroy(&all_aboard);
    sem_destroy(&car_empty);
    sem_destroy(&tour_done);

    return 0;
}

void* car(void* args) {
    while (1) {
        load();
        sleep(1);
        printf("Car is running with %d passengers.\n", num_passengers);
        sleep(1);
        sem_post(&tour_done);
        unload();
    }
}

void* passenger(void* args) {
    int passenger_id = (intptr_t)args;

    while (1) {
        board();
        printf("Passenger %d is boarding the car.\n",passenger_id);
        sleep(1);
        offboard();
        printf("Passenger %d is getting off the car.\n",passenger_id);
    }
}

void load() {
    sem_wait(&all_aboard);
    sleep(1);
    printf("Car is loading passengers.\n");
}

void unload() {
    sem_wait(&tour_done);
    printf("Car is unloading passengers.\n");
    sleep(1);

    sem_post(&car_full);
}

void board() {
    sem_wait(&car_empty);
    sleep(1);
    num_passengers++;

    if (num_passengers == car_capacity) {
        sem_post(&all_aboard);
    }
    else{sem_post(&car_empty);}
}

void offboard() {
    sem_wait(&car_full);
    sleep(1);
    num_passengers--;

    if (num_passengers == 0) {
        sem_post(&car_empty);
    }
    else{sem_post(&car_full);}
}
