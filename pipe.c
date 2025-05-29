// Write a program to implement the concept of IPC between two processes using pipe(). The
// communication must be bi-directional and continuous within the processes, i.e., if parent
// process says “Hi”, child will be able to read “Hi” and then child process will say “Hello”,
// parent will read “Hello” and so on. Please make note that your code must be able to distinguish
// between the processes while communicating, through their unique IDs.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100
#define ITERATIONS 5

int main() {
    int p1[2], p2[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];

    // Creating Pipes
    if (pipe(p1) == -1 || pipe(p2) == -1) {
        perror("Pipe initialization failed");
        exit(EXIT_FAILURE);
    }

    // Forking process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed to create a child process");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {  // Parent Process
        close(p1[0]); // Close read end of pipe 1
        close(p2[1]); // Close write end of pipe 2

        for (int i = 0; i < ITERATIONS; i++) {
            char parent_msg[BUFFER_SIZE];
            sprintf(parent_msg, "Parent (PID: %d) says: Hi %d", getpid(), i+1);
            
            // Send message to child
            write(p1[1], parent_msg, strlen(parent_msg) + 1);
            printf("[Parent] Sent: %s\n", parent_msg);
            
            // Receive message from child
            read(p2[0], buffer, BUFFER_SIZE);
            printf("[Parent] Received: %s\n\n", buffer);

            sleep(1);
        }

        close(p1[1]); // Close write end of pipe 1
        close(p2[0]); // Close read end of pipe 2
    } 
    else {  // Child Process
        close(p1[1]); // Close write end of pipe 1
        close(p2[0]); // Close read end of pipe 2

        for (int i = 0; i < ITERATIONS; i++) {
            // Receive message from parent
            read(p1[0], buffer, BUFFER_SIZE);
            printf("[Child] Received: %s\n", buffer);
            
            char child_msg[BUFFER_SIZE];
            sprintf(child_msg, "Child (PID: %d) replies: Hello %d", getpid(), i+1);
            
            // Send response to parent
            write(p2[1], child_msg, strlen(child_msg) + 1);
            printf("[Child] Sent: %s\n\n", child_msg);

            sleep(1);
        }

        close(p1[0]); // Close read end of pipe 1
        close(p2[1]); // Close write end of pipe 2
    }

    return 0;
}
