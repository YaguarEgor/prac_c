#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>

enum
{
    ADDR_BIT = 16, 
    PAGE_BIT = 9,
    ADDR_SIZE = 2
};

int main(int argc, char** argv)
{
    if (argc < 3) {
        return -1;
    }
    int memfd = open(argv[1], O_RDONLY);
    if (memfd < 0) {
        return -1;
    }
    char* ptr = NULL;
    errno = 0;

    long pt = strtol(argv[2], &ptr, 16);
    if(errno || !ptr || ptr == argv[2] || (uint16_t) pt != pt) {
        return -1;
    }
    uint16_t page_table = pt;
    uint16_t va = 0, fa = 0, vp = 0, fp = 0;
    uint16_t vp_data_addr = 0, offset = 0, data = 0;
    while (scanf("%x", &va) == 1) {
        offset = va & ((1 << PAGE_BIT) - 1);
        vp = va >> PAGE_BIT;
        vp_data_addr = page_table + ADDR_SIZE * vp;
        lseek(memfd, vp_data_addr, SEEK_SET);
        if (read(memfd, &fp, sizeof(fp)) != sizeof(fp)) {
            return -1;
        }
        fa = fp | offset;
        lseek(memfd, fa, SEEK_SET);
        if (read(memfd, &data, sizeof(data)) != sizeof(data)) {
            return -1;
        }
        printf("%u\n", data);
    }
    return 0;
}
