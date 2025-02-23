#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>

int main(int argc, char** argv) {
    if (argc < 6) {
        return -1;
    }
    int fd_pipe[2];
    pipe(fd_pipe);
    if (!fork()) {
        dup2(fd_pipe[1], 1);
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        pid_t pid;
        if ((pid = fork()) < 0) {
            return -1;
        }
        else if (!pid) {
            execlp(argv[1], argv[1], NULL);
            _exit(0);
        }
        wait(NULL);
        if ((pid = fork()) < 0) {
            return -1;
        }
        else if (!pid) {
            execlp(argv[2], argv[2], NULL);
            _exit(0);
        }
    }
    wait(NULL);
    close(fd_pipe[1]);
    if (!fork()) {
        int fd_out = open(argv[5], O_WRONLY | O_CREAT | O_APPEND, 0777);
        dup2(fd_pipe[0], 0);
        dup2(fd_out, 1);
        close(fd_pipe[1]);
        pid_t pid;
        if ((pid = fork()) < 0) {
            return -1;
        }
        else if (!pid) {
            execlp(argv[3], argv[3], NULL);
            _exit(0);
        }
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status)) {
            if ((pid = fork()) < 0) {
                return -1;
            }
            else if (!pid) {
                execlp(argv[4], argv[4], NULL);
                _exit(0);
            }
        }
    
    }
    wait(NULL);
    return 0;
}