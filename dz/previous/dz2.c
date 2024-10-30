#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct List List;
typedef struct dictElement dictElement;

struct List {
    int key;
    List* next;
};

struct dictElement{
    char* word;
    int count;
    dictElement* right;
    dictElement* left;
} ;

//interface

List* input() {
    List* l = (List*) malloc(sizeof(List));
    List* cur = l;
    int new = 0;
    int c = 0;
    int pred = 0;
    while (1) {
        c = getchar();
        if (c == EOF) {
            if (!isspace(pred)) {
                cur -> key = new;
                cur -> next = NULL;
            }
            return l;
        }
        if (isspace(c)) {
            cur -> key = new;
            cur -> next = (List*) malloc(sizeof(List));
            cur = cur->next;
            new = 0;
        }
        else {
            new = new*10 + (c-'0');
        }
        pred = c;
    }
}

void printList(List* l) {
    List* cur = NULL;
    cur = l;
    while (cur != NULL) {
        printf("%d ", cur->key);
        cur = cur->next; 
    }
    printf("\n");
}

void freeMemory(List* l) {
    List* cur = l;
    while (l != NULL) {
        cur = l->next;
        free(l);
        l = cur;
    }
}

/////////////////////////////////////////////

//#6
int heightOfTree(dictElement* tree) {
    if (tree == NULL) {
        return 0;
    }
    int lheight = 1 + heightOfTree(tree -> left);
    int rheight = 1 + heightOfTree(tree -> right);
    if (lheight > rheight) {
        return lheight;
    }
    return rheight;
}

////////////////////////////////

//#7
List* pushVal(List* l, int val) {
    List* newVal = (List*) malloc(sizeof(List));
    newVal -> key = val;
    newVal -> next = l;
    l = newVal;
    return l;
}

int popVal(List* l) {
    int res = l->key;
    List* last = l;
    l = l->next;
    free(last);
    return res;
}

int getVal(List* l) {
    return l->key;
}

int sizeStack(List* l) {
    if (l == NULL) {
        return 0;
    }
    return 1 + sizeStack(l -> next);
}

///////////////////////////////

//#1
List* changeLastAndFirst(List* l) {
    List* first = l;
    List* cur = l;
    List* temp = l;
    if (l->next == NULL) {
        printf("Nothing to change\n");
        return l;
    }
    while ((cur -> next)->next != NULL) {
        cur = cur -> next;
    }
    temp = cur -> next;
    temp -> next = l->next;
    first -> next = NULL;
    cur -> next = first;
    l = temp;
    return l;
}

////////////////////////////////////////////////

//#2
List* doubleValue(List* l, int val) {
    List* cur = l;
    while (cur != NULL) {
        if (cur -> key == val) {
            List* buf = (List*) malloc(sizeof(List));
            buf -> next = cur->next;
            buf -> key = val;
            cur -> next = buf;
            cur = buf -> next;
        }
        else {
            cur = cur->next;
        }
    }
    return l;
}


int main() {
    List* l = input();
    printf("Was before changing: ");
    printList(l);
    l = changeLastAndFirst(l);
    printf("Now: ");
    printList(l);

    printf("Was before doubling: ");
    printList(l);
    l = doubleValue(l, 90);
    printf("Now: ");
    printList(l);

    freeMemory(l);
    return 0;
}