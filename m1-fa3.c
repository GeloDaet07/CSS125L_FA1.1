#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int fd[2]; //The array that holds the file descriptors for the pipe
    char message[] = "MAPUA";
    char buffer[50];
    pid_t jpt;
    
    if(pipe(fd) == -1){
        perror("Pipe Failed");
        exit(1);
    }

    jpt = fork();

    if (jpt < 0){
        perror("Fork Failed");
        exit(1);
    }

    if (jpt > 0){
        close(fd[1]);
        read(fd[0], buffer, sizeof(buffer));
	printf("Parent jpt: %d\n", getpid());
        printf("Child jpt: %d\n", jpt);
        printf("Parent received message: %s\n", buffer);
        close(fd[0]);
        wait(NULL);
    } else {
        close(fd[0]);
        write(fd[1], message, strlen(message) + 1);
        close(fd[1]);
        exit(0);
    }

    return 0;
}