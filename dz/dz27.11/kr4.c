#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <wait.h>
#include <signal.h>
#define __USE_MISC
#include <math.h>

//not full

int main(int argc, char** argv) {
    if (argc < 6) {
        return -1;
    }
    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if (fd == -1) {
        return -1;
    }
    char* ptr = NULL;
    __uint128_t p = strtoull(argv[2], &ptr, 10);
    long double x0 = strtold(argv[3], &ptr);
    long double s = strtold(argv[4], &ptr);
    __uint128_t k = strtoull(argv[5], &ptr, 10);
    for (__uint128_t i = 0; i < p; i ++) {
        for (__uint128_t j =0; j < k+1; j++) {
            long double cur = cos((x0+j*s)*(i+1)/(p*M_PI));
            write(fd, &cur, sizeof(long double));
        }
        char el = '\n';
        write(fd, &el, sizeof(char));
    }
    return 0;
}