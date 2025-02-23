#include <signal.h>
#include <stdio.h>
#include <unistd.h>


int x = 1;

void hlr() {
    signal(SIGINT, hlr);
    x++;
}

void hlr2() {
    printf("By-by!\n");
    _exit(0);
}

int main() {
    signal(SIGINT, hlr);
    signal(SIGQUIT, hlr2);
    while (1) {
        printf("%d\n", x);
    }
    return 0;
}