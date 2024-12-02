#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

int main() {
    int iter = 1;
    int fd12[2], fd21[2];
    pipe(fd12);
    pipe(fd21);
    __pid_t pid1, pid2;
    if ((pid1 = fork())) {
        if ((pid2 = fork())) { //father
            sleep(5);
            kill(pid1, SIGKILL);
            kill(pid2, SIGKILL);
            while ((wait(NULL) != -1));
        }
        else { //son 2
            close(fd12[1]);
            close(fd21[0]);
            while (read(fd12[0], &iter, 4)) {
                printf("pid: %d, iteration: %d\n", getpid(), iter++);
                write(fd21[1], &iter, 4);
            }
        }
    }
    else { //son 1
        close(fd12[0]);
        close(fd21[1]);
        write(fd12[1], &iter, 4);
        while (read(fd21[0], &iter, 4)) {
            printf("pid: %d, iteration: %d\n", getpid(), iter++);
            write(fd12[1], &iter, 4);
        }
    }
    return 0;
}