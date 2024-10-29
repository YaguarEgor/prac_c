#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct List1 List1;
typedef struct List2 List2;

struct List1 {
    int key;
    List1* next;
};

struct List2 {
    int key;
    List2* previous;
    List2* next;
};

List2* doubleValue(List2* l) {
    List2* cur = l;
    while (cur != NULL) {
        List2* buf = (List2*) malloc(sizeof(List2));
        buf -> next = cur->next;
        buf -> previous = cur;
        buf -> key = cur -> key;
        cur -> next = buf;
        cur = buf -> next;
        if (cur != NULL)
            cur -> previous = buf;
    }
    return l;
}

List2* pushVal(List2* l, int val) {
    List2* newVal = (List2*) malloc(sizeof(List2));
    if (l != NULL) {
        l -> previous = newVal;
        newVal -> next = l;
    }
    newVal -> key = val;
    newVal -> previous = NULL;
    l = newVal;
    return l;
}

void printList(List2* l) {
    List2* cur = NULL;
    cur = l;
    while (cur != NULL) {
        printf("%d ", cur->key);
        cur = cur->next; 
    }
    printf("\n");
}

int main() {
    List2* l = NULL;
    l = pushVal(l, 10);
    l = pushVal(l, 15);
    l = pushVal(l, -15);
    l = pushVal(l, 13);
    doubleValue(l);
    printList(l);
    return 0;
}