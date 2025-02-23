#include <stdio.h>

int main(int argc, char** argv) {
    unsigned long long n = argc-1;
    FILE* all_files[n];
    unsigned long long elements[n];
    unsigned long long mini = 0;
    int is_finished[n];
    int contin = n;
    for (int i = 0; i < n; i++) {
        is_finished[i] = 0;
        all_files[i] = fopen(argv[i+1], "r");
        if (!all_files[i]) {
            return 1;
        }       
        if (fscanf(all_files[i], "%lld", &(elements[i])) != 1) {
            is_finished[i] = 1;
            fclose(all_files[i]);
            contin--;
        }    
    }
    while (contin) {
        unsigned long long ind = -1;
        for (unsigned long long i = 0; i < n; i++) {
            if (!is_finished[i] ) {
                if (ind == -1 || elements[i] < mini) {
                    mini = elements[i];
                    ind = i;
                }
            }
        }
        printf("%llu ", mini);
        if (fscanf(all_files[ind], "%lld", &(elements[ind])) != 1) {
            fclose(all_files[ind]);
            is_finished[ind] = 1;
            contin--;
        }
    }
    printf("\n");
    return 0;
}