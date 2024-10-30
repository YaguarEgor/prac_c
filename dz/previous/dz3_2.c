#include <stdio.h>
#include <stdlib.h>

#define MAX 256


void reverse(FILE* f1, FILE* f2) {
    char buf[MAX];
    if (fgets(buf, MAX, f1) != NULL) {
        reverse(f1, f2);
        printf("%s", buf);
    }
    return;
}

void doubleLines(FILE* f1, FILE* f2) {
    char buf[MAX];

    while (fgets(buf, MAX, f1) != NULL) {
        fputs(buf, f2);
        fputs(buf, f2);
    }
}




int main(int argc, char** argv) {
    FILE* f1 = fopen(argv[1], "r");
    FILE* f2 = fopen(argv[2], "w");

    //doubleLines(f1, f2);
    reverse(f1, f2);

    fclose(f1);
    fclose(f2);

    return 0;
}