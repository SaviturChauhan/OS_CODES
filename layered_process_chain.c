#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void create_processes(int level, int max_levels, int a, int b) {
    if (level > max_levels) {
        return;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork Failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {  // Child process
        printf("Process PID: %d, Parent PID: %d, Level: %d\n", getpid(), getppid(), level);
        if (level == max_levels) {
            // Last child executes the addition function
            printf("Final Process performing addition:\n");
            printf("%d + %d = %d\n\n", a, b, a + b);
        } else {
            create_processes(level + 1, max_levels, a, b);
        }
        exit(EXIT_SUCCESS);
    } else {  // Parent process
        waitpid(pid, NULL, 0);
        printf("Parent PID %d continuing after child completes.\n", getpid());
    }
}

int main() {
    int a = 5, b = 7;
    int max_levels = 3;  // Defines the hierarchy depth

    printf("Starting Process Hierarchy:\n");
    create_processes(1, max_levels, a, b);

    return EXIT_SUCCESS;
}
