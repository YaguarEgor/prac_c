#include <unistd.h>
#include <stdlib.h>

enum { BSIZE = 1024 };
enum { PERBLOCK = BSIZE / sizeof(unsigned) };
enum
{
    THRESH1 = 10,
    THRESH2 = THRESH1 + PERBLOCK,
    THRESH3 = THRESH2 + PERBLOCK * PERBLOCK,
    THRESH4 = THRESH3 + PERBLOCK * PERBLOCK * PERBLOCK,
};

void bread(int fdfs, unsigned n, void *buf)
{
    lseek(fdfs, n * BSIZE, SEEK_SET);
    read(fdfs, buf, BSIZE);
}

int check(unsigned blockNum, unsigned *blocks, unsigned long size, int fdfs)
{
    unsigned bcount = (size + BSIZE - 1) / BSIZE;
    for (unsigned i = 0; i < bcount; ++i) {
        if (i < THRESH1) {
            if (blocks[i] == blockNum) {
                return 1;
            }
        } else if (i < THRESH2) {
            unsigned l1[PERBLOCK];
            bread(fdfs, blocks[10], l1);
            if (l1[i - THRESH1] == blockNum) {
                return 1;
            }
        } else if (i < THRESH3) {
            unsigned i1 = i - THRESH2;
            unsigned l1[PERBLOCK];
            bread(fdfs, blocks[11], l1);
            unsigned l2[PERBLOCK];
            bread(fdfs, l1[i1 / PERBLOCK], l2);
            if (l2[i1 % PERBLOCK] == blockNum) {
                return 1;
            }
        } else if (i < THRESH4) {
            unsigned i1 = i - THRESH3;
            unsigned l1[PERBLOCK];
            bread(fdfs, blocks[12], l1);
            unsigned l2[PERBLOCK];
            bread(fdfs, l1[i1 / PERBLOCK / PERBLOCK], l2);
            unsigned l3[PERBLOCK];
            bread(fdfs, l2[(i1 / PERBLOCK) % PERBLOCK], l3);
            if (l3[i1 % PERBLOCK] == blockNum) {
                return 1;
            }
        } else {
            abort();
        }
    }
    return 0;
}
