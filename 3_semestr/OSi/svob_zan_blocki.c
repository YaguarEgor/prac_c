#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int getStatusOfBlock(unsigned char* mas, unsigned long long n, unsigned long long maximum) {
    if (n > maximum) {
        return -1;
    }
    unsigned long long num_of_bite = n / 8;
    int num_in_bite = n%8;
    int mask = 1 << (7-num_in_bite);
    char bite = mas[num_of_bite];
    int status = (bite&mask) >> (7-num_in_bite);
    return status;
}

int main() {

    return 0;
}