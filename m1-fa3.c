#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int pipefd[2];
    char message[] = "MAPUA";
    char buffer[50];
    pid_t pid;
    
    if(pipe(pipefdd) == -1){
        perror("Pipe Failed");
        exit(1);
    }

    pid = fork();

    if (pid < 0){
        perror("Fork Failed");
        exit(1);
    }

    if (pid > 0){
        close(pipefd[0]);
        printf("Parent PID: %d\n", getpid());
        printf("Child PID: %d\n", pid);
        write(pipefd[1], message, strlen(message) + 1);
        close(pipefd[1]);
        exit(1);
    } else {
        close(pipefd[1]);
        read(pipefd[0], buffer, sizeof(buffer));
        printf("Parent received message: %s\n", buffer);
        wait(NULL);
    }

    return 0;
}