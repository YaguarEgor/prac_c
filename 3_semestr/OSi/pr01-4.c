#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
    struct Node *prev, *next;
    char *elem;
};

struct List {
    struct Node *first, *last; 
};

void lappend(struct List* l, struct Node* item) {
    if (!l || !item) {
        return;
    }

    if (!l->first) {
        l->first = l->last = item;
        return;
    }

    l->last->next = item;
    item->prev = l->last;
    l->last = item;
    return;
}

void lcut(struct List* l, struct Node* item) {
    if (!l || !item) {
        return;
    }

    if (l->first == item && l->last == item) {
        l->first = l->last = NULL;
    } else if (l->first == item) {
        l->first = item->next;
        l->first->prev = NULL;
    } else if (l->last == item) {
        l->last = item->prev;
        l->last->next = NULL;
    } else {
        item->prev->next = item->next;
        item->next->prev = item->prev;
    }
    
    item->next = item->prev = NULL;
    return;
}

void lremove(struct List* l, struct Node* item) {
    lcut(l, item);    
    free(item->elem);
    free(item);
    return;
}

void process(struct List *pl, const char *str) {
    if (!pl || !str) {
        return;
    }

    struct Node* end = NULL;
    struct Node* cur = pl->first;

    while (cur && cur != end) {
        if (strcmp(cur->elem, str) > 0) {
            struct Node* tmp = cur;
            cur = cur->next;

            if (!end) {
                end = tmp;
            }

            lcut(pl, tmp);
            lappend(pl, tmp);
            continue;
        }

        if (!strcmp(cur->elem, str)) {
            struct Node* tmp = cur;
            cur = cur->next;

            lremove(pl, tmp);
            continue;
        }

        cur = cur->next;
    }
}

