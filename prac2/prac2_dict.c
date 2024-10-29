#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

typedef struct dictElement dictElement;

struct dictElement{
    char* word;
    int count;
    dictElement* right;
    dictElement* left;
} ;

dictElement* rewriteNode(dictElement* oldNode) {
    dictElement* node = (dictElement*)malloc(sizeof(dictElement));
    node -> word = oldNode -> word;
    node -> count = oldNode -> count;
    node -> left = node -> right = NULL;
    return node;
}

dictElement* insertCountNode(dictElement* oldTree, dictElement* newNode) {
    
    if (oldTree == NULL) {
        return rewriteNode(newNode);
    }
    if ((newNode->count) >= (oldTree->count)) {
        oldTree->right = insertCountNode(oldTree->right, newNode);
    }
    else {
        oldTree->left = insertCountNode(oldTree->left, newNode);
    }
    return oldTree;
}



dictElement* getNodes(dictElement* oldTree, dictElement* newTree) {
    if (oldTree -> right != NULL) {
        newTree = getNodes(oldTree->right, newTree);
    }
    if (oldTree -> left != NULL) {
        newTree = getNodes(oldTree->left, newTree);
    }
    newTree = insertCountNode(newTree, oldTree);
    return newTree;
}



dictElement* createTree(char* word) {
    dictElement* newNode = (dictElement*)malloc(sizeof(dictElement));
    newNode->word = word;
    newNode->right = newNode->left = NULL;
    newNode->count = 1;
    return newNode;
}

dictElement* insertNode(dictElement* oldTree, char* newWord) {
    if (oldTree == NULL) {
        return createTree(newWord);
    }
    if (strcmp(newWord, oldTree->word) > 0) {
        oldTree->right = insertNode(oldTree->right, newWord);
    }
    else if (strcmp(newWord, oldTree->word) < 0) {
        oldTree->left = insertNode(oldTree->left, newWord);
    }
    else {
        oldTree->count ++;
        free(newWord);
    }
    return oldTree;
}

void printTree(dictElement* Tree, int allCount) {
    if (Tree == NULL) {
        return;
    }
    printTree(Tree->right, allCount);
    printf("%s %d %f\n", Tree->word, Tree->count, (float)Tree->count / allCount);
    printTree(Tree->left, allCount);
}

dictElement* inputText(int* allCount) {
    dictElement* root = NULL;
    int buf;
    long int len = 1;
    char* c = NULL;
    c = malloc(len);
    int wasSpace = 0;
    c[0] = '\0';

    while (1)
    {
        buf = getchar();
        if (buf == EOF) {
            if (len == 1) {
                free(c);
                return root;
            }
            (*allCount) ++;
            return insertNode(root, c);
        }
        if (isspace(buf)) {
            if (!wasSpace) {
                wasSpace = 1;
                (*allCount) ++;
                root = insertNode(root, c);
                len = 1;
                c = malloc(len);
                c[0] = '\0';
            }
        }
        else if (ispunct(buf)) {
            if (len > 1 && !ispunct(c[len-2])) {
                (*allCount) ++;
                root = insertNode(root, c);
                len = 2;
                c = malloc(len);
                c[0] = buf;
                c[1] = '\0';
            }
            else {
                c[len-1] = buf;
                len++;
                c = realloc(c, len);
                c[len-1] = '\0';
            }
            wasSpace = 0;            
        }
        else if (isprint(buf)) {
            if (len >= 2 && ispunct(c[len-2])) {
                (*allCount) ++;
                root = insertNode(root, c);
                len = 2;
                c = malloc(len);
                c[0] = buf;
                c[1] = '\0';
            }
            else {
                c[len-1] = buf;
                len++;
                c = realloc(c, len);
                c[len-1] = '\0';
            }
            wasSpace = 0;
            
        }

    }
    
}

void freeMemory(dictElement* tree, int ifWordFree) {
    if (tree -> right != NULL) {
        freeMemory(tree->right, ifWordFree);
    }
    if (tree -> left != NULL) {
        freeMemory(tree->left, ifWordFree);
    }
    if (ifWordFree) {
        free(tree->word);
    }
    free(tree);
}


int main() {
    int allCount = 0;
    dictElement* Tree = inputText(&allCount);
    //printTree(Tree);

    dictElement* CountTree = NULL;
    CountTree = getNodes(Tree, CountTree);
    printTree(CountTree, allCount);


    //
    freeMemory(CountTree, 0);
    freeMemory(Tree, 1);
    return 0;
}