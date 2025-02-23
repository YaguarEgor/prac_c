#include <stdio.h>
#include <stdlib.h>

typedef struct dictElement dictElement;

struct dictElement{
    int key;
    dictElement* right;
    dictElement* left;
} ;


dictElement* createTree(int key) {
    dictElement* newNode = (dictElement*)malloc(sizeof(dictElement));
    newNode->key = key;
    newNode->right = newNode->left = NULL;
    return newNode;
}

dictElement* insertNode(dictElement* oldTree, int newKey) {
    if (oldTree == NULL) {
        return createTree(newKey);
    }
    if (newKey > oldTree -> key) {
        oldTree->right = insertNode(oldTree->right, newKey);
    }
    else{
        oldTree->left = insertNode(oldTree->left, newKey);
    }
    return oldTree;
}

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


///////////////////////////

//#8

void printLevel(dictElement* t, int level) {
    if (t == NULL) {
        return;
    } else {
        printLevel(t->left, level - 1); 
        if (level == 0) {
            printf("%d ", t->key);  
        }
        printLevel(t->right, level - 1);  
    }
}

void printTreeByLevels(dictElement* tree) {
    int height = heightOfTree(tree);
    for (int i = 0; i < height; i++) {
        printLevel(tree, i);
        printf("\n");
    }
}


///////////////////

//#9
int ifEqual(dictElement* tree1, dictElement* tree2) {
    if (tree1 == NULL || tree2 == NULL) {
        return tree1 == NULL && tree2 == NULL;
    }
    return (tree1->key == tree2->key) && ifEqual(tree1->right, tree2->right) && ifEqual(tree1->left, tree2->left);
}


/////////////////////////////////

int main() {
    dictElement* tree = NULL;
    tree = insertNode(tree, 5);
    tree = insertNode(tree, 7);
    tree = insertNode(tree, 3);
    tree = insertNode(tree, 6);
    tree = insertNode(tree, 4);
    tree = insertNode(tree, 2);
    //printTreeByLevels(tree);

    dictElement* tree2 = NULL;
    tree2 = insertNode(tree2, 5);
    tree2 = insertNode(tree2, 7);
    tree2 = insertNode(tree2, 3);
    tree2 = insertNode(tree2, 6);
    tree2 = insertNode(tree2, 4);
    tree2 = insertNode(tree2, 2);

    ifEqual(tree, tree2) ? printf("Equal\n") : printf("Not equal\n");

    return 0;
}