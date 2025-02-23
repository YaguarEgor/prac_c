#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc == 1) {
        return 0;
    }

    FILE** seq = calloc(argc-1, sizeof(*seq));
    for (int i = 1; i < argc; i++) {
        seq[i-1] = fopen(argv[i], "r");
        
        if (!seq[i-1]) {
            for (int j = 0; j < i-1; j++) {
                fclose(seq[j]);
            }
        
            free(seq);
            return 1;
        }
    }

    long long* numbers  = calloc(argc-1, sizeof(*numbers));
    unsigned char* done = calloc(argc-1, sizeof(*done));
    int count = argc-1;

    for (int i = 0; i < argc-1; i++) {
        if (fscanf(seq[i], "%lld", &numbers[i]) != 1) {
            fclose(seq[i]);
            done[i] = 1;
            count--;
        }
    }

    while (count) {
        long long minval = 0;
        int pos = -1;

        for (int i = 0; i < argc-1; i++) {
            if (!done[i]) {
                if (pos == -1 || minval > numbers[i]) {
                    pos = i;
                    minval = numbers[i];
                }
            }
        }
        printf("%lld ", minval);
        if (fscanf(seq[pos], "%lld", &numbers[pos]) != 1) {
            fclose(seq[pos]);
            done[pos] = 1;
            count--;
        }
    }

    printf("\n");
    free(seq);
    free(numbers);
    free(done);

    return 0;
}