#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

time_t start = 0;

void hlr() {
    signal(SIGINT, hlr);
    time_t diff = (clock() - start)/ CLOCKS_PER_SEC;
    if (diff >= 1) {
        printf("Hi\n");
        start = clock();
    }
    else {
        printf("By-by!\n");
        _exit(0);
    }
    
}

int main() {
    signal(SIGINT, hlr);
    start = clock();
    while (1);
    return 0;
}