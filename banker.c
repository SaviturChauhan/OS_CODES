#include <stdio.h>
#include <stdlib.h>

#define NUM_PROCESSES 5
#define NUM_RESOURCES 3

// Function to calculate the Need matrix
void calculate_need(int need[NUM_PROCESSES][NUM_RESOURCES], int max[NUM_PROCESSES][NUM_RESOURCES], int alloc[NUM_PROCESSES][NUM_RESOURCES]) {
    for (int i = 0; i < NUM_PROCESSES; i++) {
        for (int j = 0; j < NUM_RESOURCES; j++) {
            need[i][j] = max[i][j] - alloc[i][j];
        }
    }
}

// Function to find and print the safe sequence
void find_safe_sequence(int alloc[NUM_PROCESSES][NUM_RESOURCES], int max[NUM_PROCESSES][NUM_RESOURCES], int avail[NUM_RESOURCES]) {
    int need[NUM_PROCESSES][NUM_RESOURCES];
    int done[NUM_PROCESSES] = {0}; // Tracks completed processes
    int safe_seq[NUM_PROCESSES];   // Stores the safe sequence
    int count = 0;

    // Calculate Need matrix
    calculate_need(need, max, alloc);

    printf("\nNeed Matrix:\n");
    printf("Process\tA\tB\tC\n");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("P%d\t", i);
        for (int j = 0; j < NUM_RESOURCES; j++) {
            printf("%d\t", need[i][j]);
        }
        printf("\n");
    }

    // Find Safe Sequence
    while (count < NUM_PROCESSES) {
        int found = 0; // Flag to check if allocation was made

        for (int i = 0; i < NUM_PROCESSES; i++) {
            if (!done[i]) { // Process not completed
                int can_allocate = 1;
                for (int j = 0; j < NUM_RESOURCES; j++) {
                    if (need[i][j] > avail[j]) {
                        can_allocate = 0;
                        break;
                    }
                }
                if (can_allocate) {
                    // Allocate resources and update availability
                    for (int j = 0; j < NUM_RESOURCES; j++) {
                        avail[j] += alloc[i][j];
                    }

                    safe_seq[count++] = i;
                    done[i] = 1;
                    found = 1;
                }
            }
        }

        // If no allocation was possible, system is not in a safe state
        if (!found) {
            printf("\nSystem is not in a safe state! Deadlock might occur.\n");
            return;
        }
    }

    // Display Safe Sequence
    printf("\nSafe Sequence: ");
    for (int i = 0; i < NUM_PROCESSES; i++) {
        printf("P%d", safe_seq[i]);
        if (i != NUM_PROCESSES - 1) {
            printf(" -> ");
        }
    }
    printf("\n");
}

int main() {
    int alloc[NUM_PROCESSES][NUM_RESOURCES] = {
        {0,1,0},
        {2,0,0},
        {3,0,2},
        {2,1,1},
        {0,0,2}
    };

    int max[NUM_PROCESSES][NUM_RESOURCES] = {
        {7,5,3},
        {3,2,2},
        {9,0,2},
        {2,2,2},
        {4,3,3}
    };

    int avail[NUM_RESOURCES] = {3,3,2};

    // Run Banker's Algorithm
    find_safe_sequence(alloc, max, avail);

    return 0;
}
