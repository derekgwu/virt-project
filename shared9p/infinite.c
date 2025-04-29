#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid1, pid2;

   
    pid1 = fork();

    if (pid1 < 0) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) {
        printf("First child (PID: %d) spinning...\n", getpid());
        while (1);  
    }

 
    pid2 = fork();

    if (pid2 < 0) {
        perror("fork() failed");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) {
        printf("Second child (PID: %d) spinning...\n", getpid());
        while (1); 
    }

 
    printf("Parent (PID: %d) created children with PIDs: %d and %d\n", getpid(), pid1, pid2);


    while (1) {
        sleep(10);
    }

    return 0;
}
