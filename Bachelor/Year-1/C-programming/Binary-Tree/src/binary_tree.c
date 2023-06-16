#include <stdio.h>
#include <stdlib.h>
#include "binary_tree.h"

int max(int a, int b) {
    return a > b ? a : b;
}

size_t getHeight(const struct Tree* tree) {
    // Высота дерева
    
    if (tree == NULL){
        return 0;
    }

    return 1 + max(getHeight(tree->left), getHeight(tree->right));
}

struct Tree* createSubTree(const int data){
    //Создание поддерева
    
    struct Tree* result = (struct Tree*) malloc(sizeof(struct Tree));

    if (result == NULL){
        printf("No memory error\n");
        return NULL;
    }

    result->data = data;
    result->left = NULL;
    result->right = NULL;

    return result;
}

void addSubTree(struct Tree** tree, struct Tree* subTree){
    // Добавление поддерева
    
    if (tree == NULL){
        printf("NULL error\n");
        return;
    }

    if ((*tree) == NULL){
        (*tree) = subTree;
        return;
    }

    if (subTree->data < (*tree)->data){
        if ((*tree)->left == NULL){
            (*tree)->left = subTree;
            return;
        }
        addSubTree(&(*tree)->left, subTree);
        return;
    }
    if (subTree->data > (*tree)->data){
        if ((*tree)->right == NULL){
            (*tree)->right = subTree;
            return;
        }
        addSubTree(&(*tree)->right, subTree);
        return;
    }
}

bool add(struct Tree** tree, const int data) {
    // Добавление значения в дерево
    // false - добавить не получилось
    // true - добавление прошло успешно
    
    struct Tree* subTree = createSubTree(data);

    if (subTree == NULL){
        return false;
    }

    addSubTree(tree, subTree);
    return true;
}

struct Tree* find(struct Tree* root, const int data) {
    // найти элемент со значением "data"
    
    if (root == NULL){
        return NULL;
    }

    if (root->data == data){
        return root;
    }

    if (data < root->data){
        return find(root->left, data);
    }
    return find(root->right, data);
}

size_t countNodes(const struct Tree* root) {
    // Количество узлов в дереве
    
    if (root == NULL){
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

void destroyTree(struct Tree* root) {
    // Уничтожение дерева
    
    if (root == NULL){
        return;
    }

    destroyTree(root->right);
    destroyTree(root->left);

    free(root);
}
