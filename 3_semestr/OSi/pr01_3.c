#include <stdio.h>

int main() {
    int a, b;
    unsigned int n;
    scanf("%d%d%u", &a, &b, &n);
    printf("%*s", n, "");
    for (int i = a; i < b; i++)
        printf(" %*d", n, i);
    printf("\n");
    for (int i = a; i < b; i++) {
        for (int j = a-1; j < b; j++) {
            if (j == a-1)
                printf("%*d", n, i);
            else
                printf(" %*lld", n, (long long)i*j);
        }
        printf("\n");
    }
    return 0;
}