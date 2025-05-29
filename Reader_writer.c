// Implement the solution for the “Readers-writers Problem” in OS using multi-threading concept
// with m no. of readers and n no. of writers, and m>n.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define M 5   // Number of readers (m > n)
#define N 3   // Number of writers

pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t reader_count_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t writer_condition = PTHREAD_COND_INITIALIZER;

int reader_count = 0;

void* reader(void* arg) {
    int id = *(int*)arg;
    
    while (1) {
        usleep(rand() % 1000); // Simulate varied read times
        
        pthread_mutex_lock(&reader_count_mutex);
        reader_count++;
        if (reader_count == 1) { // First reader locks resource
            pthread_mutex_lock(&resource_mutex);
        }
        pthread_mutex_unlock(&reader_count_mutex);
        
        // Reading section
        printf("Reader %d is reading...\n", id);
        usleep(rand() % 1000); // Simulate reading time

        pthread_mutex_lock(&reader_count_mutex);
        reader_count--;
        if (reader_count == 0) { // Last reader unlocks resource
            pthread_mutex_unlock(&resource_mutex);
        }
        pthread_mutex_unlock(&reader_count_mutex);
    }
}

void* writer(void* arg) {
    int id = *(int*)arg;

    while (1) {
        usleep(rand() % 2000); // Simulate varied write times
        
        pthread_mutex_lock(&resource_mutex);

        // Writing section
        printf("Writer %d is writing...\n", id);
        usleep(rand() % 1000); // Simulate writing time
        
        pthread_mutex_unlock(&resource_mutex);
    }
}

int main() {
    srand(time(NULL));

    pthread_t readers[M], writers[N];
    int reader_ids[M], writer_ids[N];

    // Creating reader threads
    for (int i = 0; i < M; i++) {
        reader_ids[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
    }

    // Creating writer threads
    for (int i = 0; i < N; i++) {
        writer_ids[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
    }

    // Joining threads
    for (int i = 0; i < M; i++) {
        pthread_join(readers[i], NULL);
    }
    for (int i = 0; i < N; i++) {
        pthread_join(writers[i], NULL);
    }

    pthread_mutex_destroy(&resource_mutex);
    pthread_mutex_destroy(&reader_count_mutex);
    pthread_cond_destroy(&writer_condition);

    return EXIT_SUCCESS;
}
