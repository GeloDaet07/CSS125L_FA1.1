#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd;
    pid_t pid;
    char parent_msg[] = "We will strive for the fame and the glory";
    char child_msg[] = "Of Mapua University!";
    char buffer[100];

    // Remove any leftover pipe file from a previous run
    unlink(FIFO_NAME);

    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Parent Process
        printf("Parent (PID %d) is sending a message.\n", getpid());

        fd = open(FIFO_NAME, O_WRONLY);
        write(fd, parent_msg, strlen(parent_msg) + 1);
        close(fd);

        // Read the child's reply; this will wait until the child writes
        fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        printf("Parent (PID %d) received: '%s'\n", getpid(), buffer);
        close(fd);
        
        // Wait for the child process to terminate before cleaning up
        wait(NULL);
        
        unlink(FIFO_NAME);

    } else { // Child Process
        // Read the parent's message; this will wait until the parent writes
        fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        printf("Child (PID %d) received: '%s'\n", getpid(), buffer);
        close(fd);

        printf("Child (PID %d) is sending a message.\n", getpid());

        fd = open(FIFO_NAME, O_WRONLY);
        write(fd, child_msg, strlen(child_msg) + 1);
        close(fd);
        
        exit(EXIT_SUCCESS);
    }

    return 0;
}