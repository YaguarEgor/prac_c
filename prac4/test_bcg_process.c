#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    int i = 0;
    for (; i < 10; i++) {
        printf("i = %d\n", i);
        sleep(2);
    }
    return 0;
}