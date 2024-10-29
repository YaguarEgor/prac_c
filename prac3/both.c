#include <stdio.h>
#include "both.h"
#include <string.h>


int getArgumentsFromCMD(char** inp, char** out, int argc, char** argv) {
    int BOM = LE;
    int i = 0;
    while (i < argc) {
        if (strcmp(argv[i], "-be") == 0) {
            for (int j = i+1; j < argc; j++) {
                if (strcmp(argv[j], "-le") == 0) {
                    printf("Error: There can't be -be and -le flags at one time\n");
                    return -1;
                }
            }
            BOM = BE;
        }
        else if (strcmp(argv[i], "-le") == 0) {
            for (int j = i+1; j < argc; j++) {
                if (strcmp(argv[j], "-be") == 0) {
                    printf("Error: There can't be -be and -le flags at one time\n");
                    return -1;
                }
            }
            BOM = LE;
        }
        else if (strcmp(argv[i], "-i") == 0) {
            if (i+1 < argc) {
                i++;
                *inp = argv[i];
            }
            else {
                printf("Error: There isn't name of file to work with\n");
                return -1;
            }
        }
        else if (strcmp(argv[i], "-o") == 0) {
            if (i+1 < argc) {
                i++;
                *out = argv[i];
            }
            else {
                printf("Error: There isn't name of file to work with\n");
                return -1;
            }
        }
        i++;
    }
    return BOM;
}