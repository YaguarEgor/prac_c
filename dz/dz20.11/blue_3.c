#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int sigints = 0;
int sigtraps = 0;

void shl_sigint() {
    signal(SIGINT, shl_sigint);
    if (sigints++ == 7) {
        printf("Number of SIGTRAP's: %d\n", sigtraps);
        _exit(0);
    }
}

void shl_sigtrap() {
    signal(SIGTRAP, shl_sigtrap);
    if (sigints > 2 && sigints < 4) {
        sigtraps++;
    }
}

int main() {
    signal(SIGINT, shl_sigint);
    signal(SIGTRAP, shl_sigtrap);
    while(1);
    return 0;
}