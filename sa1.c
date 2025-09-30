//Required libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

//Declaration of FIFO_NAME
#define FIFO_NAME "/tmp/my_fifo"

int main() {
    int fd;
    pid_t pid;
    char parent_msg[] = "We will strive for the fame and the glory";
    char child_msg[] = "Of Mapua University!";
    char buffer[100];

    // Attempt to remove any leftover file first.
    unlink(FIFO_NAME);

    //Pipe Creation
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    //Highlight the part in your code where the parent forks a child.
    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process
        //Using O_RDONLY
        fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        printf("Child (PID %d) received: '%s'\n", getpid(), buffer);
        close(fd);

        printf("Child (PID %d) is sending a message.\n", getpid());

        //Using O_WRONLY
        fd = open(FIFO_NAME, O_WRONLY);
        write(fd, child_msg, strlen(child_msg) + 1);
        
        //Part where the FIFO is closed (close).
        close(fd);
        
        exit(EXIT_SUCCESS);

    } else { // Parent Process
        printf("Parent (PID %d) is sending a message.\n", getpid());

        fd = open(FIFO_NAME, O_WRONLY);
        write(fd, parent_msg, strlen(parent_msg) + 1);
        close(fd);

        // This is the correct place to read. It will wait for the child.
        fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        printf("Parent (PID %d) received: '%s'\n", getpid(), buffer);
        close(fd);
        wait(NULL);
        
        unlink(FIFO_NAME);
    }

    return 0;
}