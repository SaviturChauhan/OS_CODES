#include <stdio.h>
#include <string.h>

#define MAX_PROCESSES 5
#define TIME_QUANTUM 2

// Define structure for a process
typedef struct {
    char id[3];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

// Function to implement FCFS Scheduling
void fcfs_scheduling(Process processes[], int n) {
    int total_tat = 0, total_wt = 0;

    // Sort processes by arrival time
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (processes[j].arrival_time > processes[j + 1].arrival_time) {
                Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }

    // Calculate Completion Time
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        current_time += processes[i].burst_time;
        processes[i].completion_time = current_time;

        // Calculate TAT and WT
        processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
        processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

        total_tat += processes[i].turnaround_time;
        total_wt += processes[i].waiting_time;
    }

    printf("\nFCFS Scheduling:\n");
    printf("Process\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%d\n", processes[i].id,
               processes[i].arrival_time, processes[i].burst_time,
               processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time);
    }

    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);
    printf("Average Waiting Time: %.2f\n", (float)total_wt / n);
}

// Function to implement Round Robin Scheduling
void round_robin_scheduling(Process processes[], int n, int time_quantum) {
    int total_tat = 0, total_wt = 0;
    int current_time = 0;
    int completed = 0;

    // Initialize remaining time
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].remaining_time > 0) {
                if (current_time < processes[i].arrival_time) {
                    current_time = processes[i].arrival_time;
                }

                int execute_time = (processes[i].remaining_time > time_quantum) ? time_quantum : processes[i].remaining_time;
                processes[i].remaining_time -= execute_time;
                current_time += execute_time;

                if (processes[i].remaining_time == 0) {
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;

                    total_tat += processes[i].turnaround_time;
                    total_wt += processes[i].waiting_time;

                    completed++;
                }
            }
        }
    }

    printf("\nRound Robin Scheduling (Time Quantum = %d):\n", time_quantum);
    printf("Process\tAT\tBT\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%d\n", processes[i].id,
               processes[i].arrival_time, processes[i].burst_time,
               processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time);
    }

    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);
    printf("Average Waiting Time: %.2f\n", (float)total_wt / n);
}

int main() {
    Process processes[MAX_PROCESSES] = {
        {"P1", 0, 4, 0},
        {"P2", 1, 3, 0},
        {"P3", 2, 1, 0},
        {"P4", 3, 2, 0},
        {"P5", 4, 5, 0}
    };

    int choice;
    printf("Choose Scheduling Algorithm:\n1. FCFS\n2. Round Robin\nEnter choice: ");
    scanf("%d", &choice);

    if (choice == 1) {
        fcfs_scheduling(processes, MAX_PROCESSES);
    } else if (choice == 2) {
        round_robin_scheduling(processes, MAX_PROCESSES, TIME_QUANTUM);
    } else {
        printf("Invalid choice!\n");
    }

    return 0;
}
