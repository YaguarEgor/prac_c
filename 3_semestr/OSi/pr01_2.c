#include <stdio.h>

int main() {
    int buf;
    unsigned long long sumi = 0;
    while ((buf = getchar()) != EOF) {
        if ('0' <= buf && buf <= '9') sumi += buf-'0';
        else if ('a' <= buf && buf <= 'f') sumi += buf - 'a' + 10;
        else if ('A' <= buf && buf <= 'F') sumi += buf - 'A' + 10;
    }
    printf("%llu\n", sumi);
    return 0;
}