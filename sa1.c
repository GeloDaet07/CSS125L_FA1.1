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

    // 1. Initial Cleanup: Remove the pipe if it was left over from a crash.
    unlink(FIFO_NAME);

    // Create the fresh named pipe for this run.
    if (mkfifo(FIFO_NAME, 0666) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child Process Logic
        // The child first waits to receive the parent's message.
        fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        printf("Child (PID %d) received: '%s'\n", getpid(), buffer);
        close(fd);

        // Now, the child sends its reply.
        printf("Child (PID %d) is sending a message.\n", getpid());
        fd = open(FIFO_NAME, O_WRONLY);
        write(fd, child_msg, strlen(child_msg) + 1);
        close(fd);
        
        exit(EXIT_SUCCESS);

    } else { // Parent Process Logic
        // The parent first sends its message.
        printf("Parent (PID %d) is sending a message.\n", getpid());
        fd = open(FIFO_NAME, O_WRONLY);
        write(fd, parent_msg, strlen(parent_msg) + 1);
        close(fd);

        // Now, the parent waits to receive the child's reply.
        fd = open(FIFO_NAME, O_RDONLY);
        read(fd, buffer, sizeof(buffer));
        printf("Parent (PID %d) received: '%s'\n", getpid(), buffer);
        close(fd);
        
        // 2. Correct `wait()` Placement: Wait for the child only AFTER all communication is done.
        wait(NULL);
        
        // 3. Final Cleanup: Remove the pipe now that we are completely finished.
        unlink(FIFO_NAME);
    }

    return 0;
}