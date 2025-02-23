#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    //printf("pid: %d, ppid: %d\n", getpid(), getppid());
    pid_t ret_pid = fork();
    int status;
    if (ret_pid > 0) {
        ret_pid = wait(&status);
        printf("1\n");
    }
    else if (ret_pid == 0) {
        ret_pid = fork();
        if (ret_pid > 0) {
            int status;
            wait(&status);
            printf("2 ");
        }
        else if (ret_pid == 0) {
            printf("3 ");
        }
        else {
            printf("Error\n");
        }
    }
    else {
        printf("Error\n");
    }
    return 0;
}