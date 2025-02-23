#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

enum
{
    BYTE_SIZE = 8,
    BYTE_SHIFT = 3,
    START_SIZE = 100
};

int main(int argc, char** argv) {
    if (argc == 1) {
        return 0;
    }

    int fd;
    if ((fd = open(argv[1], O_RDWR)) < 0) {
        return 1;
    }

    unsigned long long size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    unsigned long long byte_num = 0;
    unsigned char byte;

    unsigned long long zeros = 0, max_zeros = 0;
    unsigned long long* max_pos = calloc(START_SIZE, sizeof(max_pos));
    unsigned long long len = START_SIZE, cur = 0;

    while (read(fd, &byte, sizeof(byte)) == sizeof(byte)) {
        for (unsigned bit_num = 0; bit_num < BYTE_SIZE; bit_num++) {
            if (!((byte >> (BYTE_SIZE - 1 - bit_num)) & 1)) {
                zeros++;
            } else {
                if (max_zeros && zeros == max_zeros) {
                    max_pos[cur++] = ((byte_num << BYTE_SHIFT) + bit_num) - zeros;
                    max_pos[cur++] = ((byte_num << BYTE_SHIFT) + bit_num);

                    if (cur == len) {
                        len *= 2;
                        max_pos = realloc(max_pos, len * sizeof(*max_pos));
                    }
                } else if (zeros > max_zeros) {
                    max_zeros = zeros;
                    free(max_pos);
                    max_pos = calloc(len, sizeof(*max_pos));

                    cur = 0;
                    max_pos[cur++] = ((byte_num << BYTE_SHIFT) + bit_num) - zeros;
                    max_pos[cur++] = ((byte_num << BYTE_SHIFT) + bit_num);
                }
                zeros = 0;
            }
        }
        byte_num++;
    }

    if (max_zeros && zeros == max_zeros) {
        max_pos[cur++] = (byte_num << BYTE_SHIFT) - zeros;
        max_pos[cur++] = (byte_num << BYTE_SHIFT);

        if (cur == len) {
            len *= 2;
            max_pos = realloc(max_pos, len * sizeof(*max_pos));
        }
    } else if (zeros > max_zeros) {
        max_zeros = zeros;
        free(max_pos);
        max_pos = calloc(len, sizeof(*max_pos));

        cur = 0;
        max_pos[cur++] = (byte_num << BYTE_SHIFT) - zeros;
        max_pos[cur++] = (byte_num << BYTE_SHIFT);
    }

    // for (int i = 0; i < cur; i++) {
    //     printf("%lld ", max_pos[i]);
    // }
    // printf("\n");
    // len = cur;

    for (cur = 0; cur < len; cur += 2) {
        if (max_pos[cur] == max_pos[cur + 1]) {
            continue;
        }

        unsigned long long start_byte = max_pos[cur] >> BYTE_SHIFT;
        unsigned long long finish_byte = max_pos[cur + 1] >> BYTE_SHIFT;
        unsigned start_bit = max_pos[cur] & (BYTE_SIZE - 1);
        unsigned finish_bit = max_pos[cur + 1] & (BYTE_SIZE - 1);

        if (start_byte != finish_byte) {
            max_pos[cur] = (start_byte + 1) << BYTE_SHIFT;
            finish_byte = start_byte + 1;
            finish_bit = 0;
            cur -= 2;
        }

        unsigned char mask = 0;
        do {
            mask |= 1 << (BYTE_SIZE - 1 - start_bit);
            start_bit = (start_bit + 1) % BYTE_SIZE;
        } while (start_bit != finish_bit);

        lseek(fd, start_byte, SEEK_SET);
        read(fd, &byte, sizeof(byte));
        byte |= mask;
        lseek(fd, -1, SEEK_CUR);
        write(fd, &byte, sizeof(byte));
    }

    free(max_pos);
    close(fd);
    return 0;
}
