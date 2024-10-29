#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char** argv) {
    FILE* bin = fopen(argv[1], "a+b");
    long long mini = 2^(sizeof(long long)*-1);
    long long buf = 0;
    fread(&buf, sizeof(long long), 1, bin);
    while (buf != EOF) {
        if (buf < mini)
            mini = buf;
        fread(&buf, sizeof(long long), 1, bin);
        printf("%lld ", buf);
    }
    fseek(bin, 0, SEEK_SET);
    fread(&buf, sizeof(long long), 1, bin);
    while (buf != mini) {
        fread(&buf, sizeof(long long), 1, bin);
    }
    long long temp = mini;
    while (temp != EOF) {
        fread(&buf, sizeof(long long), 1, bin);
        fseek(bin, -1, SEEK_CUR);
        fwrite(&temp, sizeof(long long), 1, bin);
        temp = buf;
    }
    return 0;
}