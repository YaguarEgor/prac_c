#include <stdio.h>
#include <stdlib.h>

char* arrInput();

char** masInput(long long int* n);

int first_bigger(char* c1, char* c2);

void arrSort(char* mas[], long long int n);

void arrPrint(char* mas[], long long int n );

int main() {
    long long int n = 0;
    char** mas = masInput(&n);
    arrSort(mas, n);
    arrPrint(mas, n);
    for (long long int i = 0; i < n; i ++) {
        free(mas[i]);
    }
    free(mas);
    return 0;
}

char* arrInput() {
    long long int len = 1;
    char* c = NULL;
    c = malloc(len);
    c[0] = '\0';
    if (c == NULL) {
        return '\0';
    }
    int buf;
    while (1) {

        buf = getchar();
        if (buf == EOF) {
            free(c);
            return NULL;
        }
        if (buf == '\n') {
            break;
        }
        c[len-1] = buf;
        c = realloc(c, ++len);
        if (c == NULL) {
            return '\0';
        }
        c[len-1]= '\0';
    } 
    return c;

}

char** masInput(long long int* n) {
    long long int len = 1;
    char** mas = NULL;
    mas = (char**)malloc(len*sizeof(char*));
    mas[0] = '\0';
    printf("Input strings by enter until EOF:\n");
    while (1) {
        mas[len-1] = arrInput();
        if (mas[len-1] == NULL) {
            free(mas[len-1]);
            len--;
            *n = len;
            return mas;
        } 
        len++;
        mas = (char**)realloc(mas, len*sizeof(char*));
        if (mas == NULL) {
            printf("Error");
            exit(1);
        }
        mas[len-1] = NULL;
    }
}

int first_bigger(char* c1, char* c2) {
    char* p1 = c1;
    char* p2 = c2; 
    while (*p1 != '\0' && *p2 != '\0') {
        if (*p1 != *p2) {
            return *p1 > *p2;
        }
        p1 ++;
        p2 ++;
    }
    return *p1 != '\0';
}

void arrSort(char* mas[], long long int n) {
    char* buf;
    for (long long int i = 0; i < n-1; i ++) {
        for (long long int j = 0; j < n-1-i; j++) {
            if (first_bigger(mas[j], mas[j+1])) {
                buf = mas[j];
                mas[j] = mas[j+1];
                mas[j+1] = buf;
            }
        }
    }
}

void arrPrint(char* mas[], long long int n) {
    char* p = NULL;
    if (n == 0) {
        printf("There are no strings in mas\n");
        return;
    }
    printf("All strings:\n");
    for (long long int i = 0; i < n; i++) {
        printf("String nunber %d: '", i+1);
        p = *(mas+i);
        while (*p != '\0') {
            printf("%c", *p);
            p++;
        }
        printf("'\n");
    }
    return;
}