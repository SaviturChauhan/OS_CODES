#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5
#define MAX_ITEMS 10

int buffer[BUFFER_SIZE];
int in = 0, out = 0;
int produced_count = 0, consumed_count = 0;

sem_t mutex;     // Controls access to buffer
sem_t full;      // Counts full slots
sem_t empty;     // Counts empty slots

void* producer(void* arg) {
    for (int item = 1; item <= MAX_ITEMS; item++) {
        sem_wait(&empty); // Wait for an empty slot
        sem_wait(&mutex); // Lock access to buffer

        buffer[in] = item;
        printf("Produced: %d | Buffer Index: %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        produced_count++;

        sem_post(&mutex); // Unlock buffer access
        sem_post(&full);  // Signal that a full slot is available
    }

    pthread_exit(NULL);
}

void* consumer(void* arg) {
    for (int i = 0; i < MAX_ITEMS; i++) {
        sem_wait(&full);  // Wait for a full slot
        sem_wait(&mutex); // Lock access to buffer

        int item = buffer[out];
        printf("Consumed: %d | Buffer Index: %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        consumed_count++;

        sem_post(&mutex); // Unlock buffer access
        sem_post(&empty); // Signal that an empty slot is available
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t producerThread, consumerThread;

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    sem_init(&full, 0, 0);
    sem_init(&empty, 0, BUFFER_SIZE);

    // Create threads for producer and consumer
    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    // Wait for threads to complete execution
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    // Clean up semaphores
    sem_destroy(&mutex);
    sem_destroy(&full);
    sem_destroy(&empty);

    return EXIT_SUCCESS;
}
