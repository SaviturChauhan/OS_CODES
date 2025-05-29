#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define N 5  // Number of philosophers
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define LEFT (phnum + N - 1) % N
#define RIGHT (phnum + 1) % N

int state[N];  // Track philosopher states
int phil[N] = {0, 1, 2, 3, 4};

sem_t mutex;  // Mutual exclusion semaphore
sem_t S[N];   // Semaphores for philosophers

void test(int phnum) {
    // Check if philosopher can start eating
    if (state[phnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) {
        state[phnum] = EATING;

        printf("Philosopher %d takes forks %d & %d\n", phnum + 1, LEFT + 1, phnum + 1);
        printf("Philosopher %d is EATING\n", phnum + 1);
        
        usleep(rand() % 2000);  // Simulate eating time
        
        sem_post(&S[phnum]);  // Allow eating
    }
}

// Pick up chopsticks
void take_fork(int phnum) {
    sem_wait(&mutex);  // Lock mutex for synchronization

    state[phnum] = HUNGRY;
    printf("Philosopher %d is HUNGRY\n", phnum + 1);

    test(phnum);  // Try to eat

    sem_post(&mutex);  // Unlock mutex
    sem_wait(&S[phnum]);  // Wait until eating is allowed
}

// Put down chopsticks
void put_fork(int phnum) {
    sem_wait(&mutex);  // Lock mutex

    state[phnum] = THINKING;
    printf("Philosopher %d putting down forks %d & %d\n", phnum + 1, LEFT + 1, phnum + 1);
    printf("Philosopher %d is THINKING\n", phnum + 1);

    // Check if neighbors can eat now
    test(LEFT);
    test(RIGHT);

    sem_post(&mutex);  // Unlock mutex
}

void* philosopher(void* num) {
    int* i = num;

    while (1) {
        usleep(rand() % 3000);  // Simulate thinking time
        take_fork(*i);
        usleep(rand() % 1000);  // Eating phase
        put_fork(*i);
    }
}

int main() {
    pthread_t thread_id[N];

    // Initialize semaphores
    sem_init(&mutex, 0, 1);
    for (int i = 0; i < N; i++) {
        sem_init(&S[i], 0, 0);
    }

    // Create philosopher threads
    for (int i = 0; i < N; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &phil[i]);
        printf("Philosopher %d is THINKING\n", i + 1);
    }

    // Join philosopher threads
    for (int i = 0; i < N; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // Clean up semaphores
    sem_destroy(&mutex);
    for (int i = 0; i < N; i++) {
        sem_destroy(&S[i]);
    }

    return;
}
