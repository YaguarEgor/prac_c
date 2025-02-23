#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <wait.h>

int iter = 1;

void hlr() {
    printf("pid: %d, iteration: %d\n", getpid(), iter++);
}

void hlr2() {
    while (1);
}

int main() {
    int a = 0;
    int fd12[2], fd21[2];
    pipe(fd12);
    pipe(fd21);
    __pid_t pid1, pid2;
    if ((pid1 = fork())) {
        if ((pid2 = fork())) { //father
            sleep(10);
            kill(pid1, SIGKILL);
            kill(pid2, SIGKILL);
            while ((wait(NULL) != -1));
        }
        else { //son 2
            close(fd12[1]);
            close(fd21[0]);
            while (read(fd12[0], &a, 4)) {
                hlr();
                write(fd21[1], &a, 4);
            }
        }
    }
    else { //son 1
        close(fd12[0]);
        close(fd21[1]);
        write(fd12[1], &a, 4);
        while (read(fd21[0], &a, 4)) {
            hlr();
            write(fd12[1], &a, 4);
        }
    }
    return 0;
}