#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <signal.h>

__uint32_t count = 0;

void hlr1() {
    printf("%u %u\n", SIGUSR1, count);
    count+=2;
}

void hlr2() {
    printf("%u %u\n", SIGUSR2, count);
    count-=3;
    if (count < 0) {
        exit(0);
    }
}

int main() {
    signal(SIGUSR1, hlr1);
    signal(SIGUSR2, hlr2);
    printf("%u\n", getpid());
    while(1) {
        pause();
    }
    return 0;
}