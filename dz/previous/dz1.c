#include <stdio.h>

#define MAX 15

void reverse(char* s);

int mystrlen(char* s);

void cdelete(char* s, char c);

void f(char* s, int* n, char* c);

int main() {

    //No 9

    // int x;
    // printf("Input number: ");
    // scanf("%d", &x);
    // printf("before: %d, after: %d\n", x, x^0x70);
    

    //No 10

    // int z;
    // printf("Input number: ");
    // scanf("%d", &z);
    // printf("before: %d, after: %d", z, (z&0x1F)|0x7);

    //No 2

    // char s[MAX];
    // printf("Input str for reverse: ");
    // scanf("%10s", s);
    // reverse(s);
    // printf("%s\n", s);

    //No 3

    // char s[MAX];
    // printf("Input str for reverse: ");
    // scanf("%10s", s);
    // printf("Input char for delete: ");
    // scanf("%c", &c);
    // //cdelete(s, 'c');
    // printf("%s\n", s);


    //No 6

    int max;
    char c;
    char s[MAX];
    printf("Input str for finding element: ");
    scanf("%10s", s);
    f(s, &max, &c);
    
    printf("Amount: %d, character: %c\n", max, c);
    return 0;
}

int mystrlen(char* s) {
    int len = 0;
    while (s[len] != '\0') {
        len ++;
    }
    return len;
}

void reverse(char* s) {
    // char temp[mystrlen(s)];
    // int len = 0;
    // while (s[len] != '\0') {
        
    //     temp[len] = s[len];
    //     len ++;
    // }
    // for (int i = 0; i < len; i++) {
    //     s[len-i-1] = temp[i];
    // }

    
    int len = mystrlen(s);
    char buf;
    for (int i = 0; i < len/2; i++) {
        buf = s[i];
        s[i] = s[len-i-1];
        s[len-i-1] = buf;
    }
    

}

void cdelete(char* s, char c) {
    int count_c = 0;
    for (int i = 0; i < mystrlen(s); i++) {
        if (s[i] == c) {
            count_c ++;
        }
        else {
            s[i-count_c] = s[i];
        } 
    }
    s[mystrlen(s)-count_c] = '\0';
}

void f(char* s, int* n, char* c) {
    int len = mystrlen(s);
    int max = 0;
    char temp[len];
    for (int i = 0; i < len; i++) {
        char new_c = s[i];
        for (int j = 0; j < i+1; j++) {
            if (new_c == temp[j]) {
                int k = j;
                while (k < i) {
                    char buf = temp[k+1];
                    temp[k+1] = new_c;
                    new_c = buf;
                    k++; 
                }
                break;
            }
            else if (j == i) {
                temp[j] = new_c;
            } 
        }
    }
    int count = 1;
    char cur, best;
    for (int i =0; i < len; i++) {
        if (cur == temp[i]) {
            count++;
        }
        else { 
            if (count > max) {
                max = count;
                best = cur;
            }
            count = 1;
            cur = temp[i];
        }
        
    }

    *n = max;
    *c = best;
    
}

