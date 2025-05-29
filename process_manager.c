#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

void addition(int a, int b) {
    printf("Process PID: %d\n%d + %d = %d\n", getpid(), a, b, a + b);
}

void print_files() {
    char *argument_list[] = {"cat", "file1.txt", "file2.txt", NULL};
    execvp(argument_list[0], argument_list);
    perror("execvp failed"); // Error handling in case execvp fails
    exit(EXIT_FAILURE);
}

int main() {
    int a = 5, b = 7;
    pid_t one, two;

    one = fork();
    if (one < 0) {
        perror("Fork Failed");
        return EXIT_FAILURE;
    } else if (one == 0) {
        addition(a, b);
        exit(EXIT_SUCCESS);
    }

    two = fork();
    if (two < 0) {
        perror("Fork Failed");
        return EXIT_FAILURE;
    } else if (two == 0) {
        print_files();
    }

    waitpid(one, NULL, 0);
    waitpid(two, NULL, 0);

    addition(a, b);
    
    return EXIT_SUCCESS;
}
