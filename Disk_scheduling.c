#include <stdio.h>
#include <stdlib.h>

#define NUM_REQUESTS 8
#define DISK_SIZE 200

// Function to print disk requests
void print_requests(const int arr[], int size) {
    printf("Disk Requests: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// Function to perform Bubble Sort
void bubble_sort(int arr[], int size) {
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// FCFS Disk Scheduling Algorithm
void FCFS(int requests[], int start) {
    int total_seek = 0;

    printf("\n--- FCFS Disk Scheduling ---\n");
    printf("Initial Head Position: %d\n", start);

    for (int i = 0; i < NUM_REQUESTS; i++) {
        total_seek += abs(start - requests[i]);
        printf("Moving from %d -> %d (Seek Time: %d)\n", start, requests[i], abs(start - requests[i]));
        start = requests[i];
    }

    printf("Total Seek Time: %d\n", total_seek);
}

// SCAN Disk Scheduling Algorithm
void SCAN(int requests[], int start) {
    int total_seek = 0, mid = 0;
    
    printf("\n--- SCAN Disk Scheduling ---\n");
    printf("Initial Head Position: %d\n", start);
    
    bubble_sort(requests, NUM_REQUESTS);

    // Find the index where scanning should reverse direction
    while (mid < NUM_REQUESTS && requests[mid] < start) {
        mid++;
    }
    
    // Moving towards 0
    for (int i = mid - 1; i >= 0; i--) {
        total_seek += abs(start - requests[i]);
        printf("Moving from %d -> %d (Seek Time: %d)\n", start, requests[i], abs(start - requests[i]));
        start = requests[i];
    }

    total_seek += abs(start - 0); 
    printf("Moving from %d -> %d (Seek Time: %d)\n", start, 0, abs(start - 0));
    start = 0;

    // Moving towards end of the disk
    for (int i = mid; i < NUM_REQUESTS; i++) {
        total_seek += abs(start - requests[i]);
        printf("Moving from %d -> %d (Seek Time: %d)\n", start, requests[i], abs(start - requests[i]));
        start = requests[i];
    }

    printf("Total Seek Time: %d\n", total_seek);
}

// CSCAN Disk Scheduling Algorithm
void CSCAN(int requests[], int start) {
    int total_seek = 0, mid = 0;
    
    printf("\n--- CSCAN Disk Scheduling ---\n");
    printf("Initial Head Position: %d\n", start);
    
    bubble_sort(requests, NUM_REQUESTS);

    while (mid < NUM_REQUESTS && requests[mid] < start) {
        mid++;
    }

    // Moving to the end of the disk
    for (int i = mid; i < NUM_REQUESTS; i++) {
        total_seek += abs(start - requests[i]);
        printf("Moving from %d -> %d (Seek Time: %d)\n", start, requests[i], abs(start - requests[i]));
        start = requests[i];
    }

    total_seek += abs(start - DISK_SIZE);
    printf("Moving from %d -> %d (Seek Time: %d)\n", start, DISK_SIZE, abs(start - DISK_SIZE));
    start = DISK_SIZE;

    total_seek += abs(start - 0);
    printf("Moving from %d -> %d (Seek Time: %d)\n", start, 0, abs(start - 0));
    start = 0;

    for (int i = 0; i < mid; i++) {
        total_seek += abs(start - requests[i]);
        printf("Moving from %d -> %d (Seek Time: %d)\n", start, requests[i], abs(start - requests[i]));
        start = requests[i];
    }

    printf("Total Seek Time: %d\n", total_seek);
}

int main() {
    int requests[] = {100, 185, 43, 124, 16, 126, 67, 69};
    int start = 55;

    print_requests(requests, NUM_REQUESTS);

    FCFS(requests, start);
    SCAN(requests, start);
    CSCAN(requests, start);

    return 0;
}
