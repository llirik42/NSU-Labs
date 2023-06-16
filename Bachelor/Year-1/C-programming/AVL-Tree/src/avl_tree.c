#include "avl_tree.h"
#include <malloc.h>
#include <stdio.h>

struct Node{
    int key;
    unsigned char height;
    struct Node* left;
    struct Node* right;
};
struct Tree{
    struct Node* root;
    struct Node* nodes;
    unsigned int nodesCount;
};

unsigned char max(const unsigned char a, const unsigned char b){
    return a > b  ? a : b;
}
unsigned char getHeightOfNode(const struct Node* node){
    return node ? node->height : 0;
}

void printNode(const struct Node* node){
    if (!node){
        return;
    }

    printNode(node->left);
    printf("%d ", node->key);
    printNode(node->right);
}
struct Node createNode(const int key){
    struct Node newNode = {.key = key, .height = 1, .left = NULL, .right = NULL};
    return newNode;
}

char getBalanceFactor(struct Node* node){
    if (!node) {
        return 0;
    } else {
        return (char) (getHeightOfNode(node->left) - getHeightOfNode(node->right));
    }
}
void adjustHeight(struct Node* node){
    if (!node){
        return;
    }

    node->height = 1 + max(getHeightOfNode(node->right), getHeightOfNode(node->left));
}
struct Node* rotateLeft(struct Node* node){
    if (!node || !node->right){
        return NULL;
    }

    struct Node* newSubTreeRoot = node->right;

    node->right = newSubTreeRoot->left;
    newSubTreeRoot->left = node;

    adjustHeight(node);
    adjustHeight(newSubTreeRoot);

    return newSubTreeRoot;
}
struct Node* rotateRight(struct Node* node){
    if (!node || !node->left){
        return NULL;
    }

    struct Node* newSubTreeRoot = node->left;

    node->left = newSubTreeRoot->right;
    newSubTreeRoot->right = node;

    adjustHeight(node);
    adjustHeight(newSubTreeRoot);

    return newSubTreeRoot;
}
struct Node* rotateRightLeft(struct Node* node){
    if (!node){
        return NULL;
    }

    node->right = rotateRight(node->right);

    return rotateLeft(node);
}
struct Node* rotateLeftRight(struct Node* node){
    if (!node){
        return NULL;
    }

    node->left = rotateLeft(node->left);

    return rotateRight(node);
}
void balanceSubTree(struct Node** root){
    const char thisBalance = getBalanceFactor(*root);

    if (thisBalance == -2){
        const char rightChildrenBalance = getBalanceFactor((*root)->right);
        *root = rightChildrenBalance < 0 ? rotateLeft(*root) : rotateRightLeft(*root);
        return;
    }
    if (thisBalance == 2){
        const char leftChildrenBalance = getBalanceFactor((*root)->left);
        *root = leftChildrenBalance > 0 ? rotateRight(*root) : rotateLeftRight(*root);
        return;
    }
}

void insert(struct Node** root, struct Node* node){
    if (!root || !node){
        return;
    }

    if(!(*root)){
        (*root) = node;
        return;
    }

    if (node->key < (*root)->key){
        insert(&(*root)->left, node);
    }
    else{
        insert(&(*root)->right, node);
    }

    adjustHeight(*root);

    balanceSubTree(root);
}

struct Tree* CreateTree(const unsigned int nodesCount){
    struct Tree* newTree = malloc(sizeof(struct Tree));
    if (!newTree){
        return NULL;
    }

    newTree->nodes = malloc(sizeof(struct Node) * nodesCount);
    if (!newTree->nodes){
        free(newTree);
        return NULL;
    }

    newTree->root = NULL;
    newTree->nodesCount = nodesCount;

    return newTree;
}
void DestroyTree(struct Tree* tree){
    if (!tree){
        return;
    }

    free(tree->nodes);
    free(tree);
}
bool InputTree(struct Tree* tree){
    if (!tree || !tree->nodes){
        return false;
    }

    for(unsigned int i = 0; i < tree->nodesCount; i++){
        int curKey = 0;
        if (scanf("%d", &curKey) != 1){
            return false;
        }
        tree->nodes[i] = createNode(curKey);
    }

    for (unsigned int i = 0; i < tree->nodesCount; i++){
        insert(&tree->root, &tree->nodes[i]);
    }


    return true;
}
void PrintTree(const struct Tree* tree){
    if (!tree || !tree->nodes){
        return;
    }

    printNode(tree->root);
}
unsigned char GetHeight(const struct Tree* tree){
    return getHeightOfNode(tree->root);
}
