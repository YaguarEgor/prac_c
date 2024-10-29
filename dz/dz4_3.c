#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int readLine(FILE* f, int* code, char** str) {
    int c = 1;
    char* line = (char*)malloc(c*sizeof(char));
    line[0] = '\0';
    int buf;
    while (1) {
        buf = fgetc(f);
        if (buf == EOF) {
            line[c-1] = '\0';
            *str = line;
            *code = 1;
            break;
        }
        else if (buf == '\n') {
            *str = line;
            *code = 0;
            break;
        }
        else {
            line[c-1] = buf;
            c++;
            line = (char*)realloc(line, c*sizeof(char));
            line[c-1] = '\n';
        }

    }
    return c-1;
}

int main(int argc, char** argv) {
    FILE* f = fopen(argv[1], "r");
    int nol = 1;
    char** lines = (char**)malloc(nol*sizeof(char*));
    char* line;
    int code = 0;
    int len;
    while (code == 0) {
        len = readLine(f, &code, &line);
        if (len <= 20) {
            lines[nol-1] = line;
            nol++;
            lines = (char**)realloc(lines, nol*sizeof(char*));
        }
    }
    fclose(f);
    f = fopen(argv[1], "w");
    for (int i = 0; i < nol-1; i++) {
        fputs((lines[i]), f);
    }


    return 0;
}