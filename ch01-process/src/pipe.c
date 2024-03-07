#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char* argv[]){
    int ret;
    int fd[2];
    pid_t pid;

    ret = pipe(fd);
    if (ret == -1){
        perror("pipe creation failed");
    }

    pid = fork();
    if (pid == -1){
        perror("fork failed");
    }

    /* parent process */
    if (pid > 0){
        close(fd[0]); /* close read side */
        write(fd[1], "pipe content", strlen("pipe content"));
        printf("parents process %d wrote \"%s\" to pipe\n", getpid(), "pipe content");
        //sleep(3);
        close(fd[1]);
    }

    /* child process */
    if (pid == 0){
        close(fd[1]); /* close write side */
        char buf[1024];
        int i = read(fd[0], buf, 1024);
        if (i == -1){
            perror("child process read failed");
            exit(EXIT_FAILURE);
        }
        printf("child process %d read \"", getpid());
        for (int c = 0; c < i; c++){
            printf("%c",buf[c]);
        }
        printf("\"\n");
        // write(STDOUT_FILENO, buf, i);
        close(fd[0]);
    }

    return 0;
}