#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int fd[2];
    pid_t pid1 = fork();

    if (pid1 == 0) {
        dup2(fd[1], 1);
        close(fd[0]); 
        close(fd[1]);
        execlp(argv[0], argv[0], argv[1], argv[2], NULL);
        perror("Error: execlp\n");
        exit(0);
    }
    else {
        close(fd[1]);
        pid_t pid2 = fork();
        if (pid2 == 0) {
            dup2(fd[0], 0);
            close(fd[0]);
            execlp(argv[3], argv[3], NULL);
            perror("Error: execlp\n");
            exit(0);
        }
        else {
            close(fd[0]);
            while (wait(NULL) != -1); 
            int fd = open(argv[5], O_WRONLY | O_APPEND | O_CREAT);
            if (fd == -1) {
                perror("Error: open f1.dat\n");
                return -1;
            }
            pid_t pid3 = fork();
            if (pid3 == 0) {
                dup2(fd, 1);
                close(fd);
                execlp(argv[4], argv[4], NULL);
                perror("Error: execlp ");
                exit(0);
            } else {
                close(fd); 
                wait(NULL); 
            }
        }
    }
    return 0;
}
