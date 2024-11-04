#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    for (int i = 1; i < argc; i++) {
        pid_t pid;
        if ((pid = fork()) == 0) {
            execlp(argv[i], argv[i], (char*)0);
            return -1;
        }
    }
    return 0;
}
