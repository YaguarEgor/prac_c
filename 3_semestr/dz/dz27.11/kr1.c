#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>



int main(int argc, char** argv) {
    if (argc < 3) {
        return -1;
    }
    __uint64_t sum = 0, cur = 0, offset;
    char *ptr = NULL;
    __uint64_t N = strtoull(argv[2], &ptr, 10);
    int fd = open(argv[1], O_RDWR);
    off_t len = lseek(fd, 0, SEEK_END)/8;
    lseek(fd, 0, SEEK_SET);
    if (N < len) {
        offset = 0;
        while(read(fd, &cur, 8) == 8) {
            sum += cur;
        }
    }
    else {
        offset = (len-N)%2==0 ? (len-N)/2 : (len-N)/2 + 1;
        lseek(fd, offset, SEEK_SET);
        for (int i = 0; i < N; i++) {
            read(fd, &cur, 8);
            sum += cur;
        }
    }
    lseek(fd, offset, SEEK_SET);
    write(fd, &sum, 8);
    return 0;
}