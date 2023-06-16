#include "mini_buffer.h"
#include "huffman_tree.h"
#include "priority_queue.h"
#include "printing_buffer.h"
#include <malloc.h>

static const struct Node EMPTY_NODE = {
        .byte = 0,
        .isLeftChild = 0,
        .frequency = 0,
        .leftChild = NULL,
        .rightChild = NULL,
        .parent = NULL
};

struct Node* getLeaf(const struct Tree* tree, const unsigned short index){
    unsigned short leavesMetCounter = 0;

    for (unsigned short i = 0; i < tree->nodesCount; i++){
        struct Node* curNode = &(tree->nodes[i]);

        if (IS_NODE_LEAF(curNode)){
            if (leavesMetCounter == index){
                return curNode;
            }

            leavesMetCounter++;
        }
    }
    return NULL;
}
struct Node createLeaf(const Byte byte, const unsigned int frequency){
    struct Node result = EMPTY_NODE;
    result.byte = byte;
    result.frequency = frequency;
    return result;
}
unsigned char getAncestorsCount(const struct Node* node){
    unsigned char result = 0;
    while (node->parent){
        result++;
        node = node->parent;
    }
    return result;
}
struct Node* addNode(struct Tree* tree, struct Node node){
    tree->nodes[tree->nodesCount] = node;
    struct Node* result = &tree->nodes[tree->nodesCount];
    tree->nodesCount++;
    return result;
}
struct Node* addEmptyNode(struct Tree* tree){
    return addNode(tree, EMPTY_NODE);
}
void connectNodes(struct Node* child, struct Node* parent, const bool isLeftChild){
    child->parent = parent;
    child->isLeftChild = isLeftChild;

    if (isLeftChild){
        parent->leftChild = child;
    }
    else{
        parent->rightChild = child;
    }
}
struct Node* uniteNodes(struct Tree* tree, struct Node* leftNode, struct Node* rightNode){
    struct Node newNode = EMPTY_NODE;
    newNode.frequency = leftNode->frequency + rightNode->frequency;
    struct Node* newNodePointer = addNode(tree, newNode);

    connectNodes(leftNode, newNodePointer, false);
    connectNodes(rightNode, newNodePointer, true);

    return newNodePointer;
}
struct Node* findParentWithoutRightChild(struct Node* node){
    node = node->parent;
    while (node && node->rightChild){
        node = node->parent;
    }
    return node;
}

Status pushMaxLeavesCount(const struct Tree* tree, struct PrintingBuffer* printingBuffer){
    return PushByte(printingBuffer, tree->maxLeavesCount - 1);
}
Status pushSubTree(const struct Node* subTreeRoot, struct PrintingBuffer* printingBuffer){
    if (!subTreeRoot){
        return SUCCESS;
    }

    if (!IS_NODE_LEAF(subTreeRoot)){
        HANDLE_OPERATION(PushBit(printingBuffer, 1))
        HANDLE_OPERATION(pushSubTree(subTreeRoot->leftChild, printingBuffer))
        HANDLE_OPERATION(pushSubTree(subTreeRoot->rightChild, printingBuffer))
        return SUCCESS;
    }

    HANDLE_OPERATION(PushBit(printingBuffer, 0))
    HANDLE_OPERATION(PushByte(printingBuffer, subTreeRoot->byte))

    return SUCCESS;
}

unsigned short inputMaxLeavesCount(FILE* in){
    const int symbol = fgetc(in);
    return symbol == EOF ? 0 : ((Byte) (symbol) + 1);
}

struct Tree* CreateEmptyTree(const unsigned short uniqueCharsCounter){
    struct Tree* tree = malloc(sizeof(struct Tree));
    if (!tree){
        return NULL;
    }

    const unsigned short maxNodesCount = uniqueCharsCounter * 2 - 1;
    tree->nodes = malloc(sizeof(struct Node) * maxNodesCount);

    if (!tree->nodes){
        DestroyTree(tree);
        return NULL;
    }

    tree->root = NULL;
    tree->nodesCount = 0;
    tree->maxLeavesCount = uniqueCharsCounter;

    return tree;
}
Status InputTree(FILE* in, struct Tree** tree){
    unsigned short maxLeavesCount = inputMaxLeavesCount(in);
    if (maxLeavesCount == 0){
        return INCORRECT_COMPRESSION;
    }

    (*tree) = CreateEmptyTree(maxLeavesCount);
    if (!(*tree)){
        return NO_MEMORY;
    }
    (*tree)->root = addEmptyNode(*tree);

    if (maxLeavesCount == 1){
        const int leafSymbol = fgetc(in);
        if (leafSymbol == EOF){
            DestroyTree(*tree);
            return INCORRECT_COMPRESSION;
        }
        (*tree)->root->byte = leafSymbol;
        return SUCCESS;
    }

    struct MiniBuffer miniBuffer;
    InitMiniBuffer(&miniBuffer, in);
    if (!InputMiniBuffer(&miniBuffer)){
        return INCORRECT_COMPRESSION;
    }

    struct Node* curNode = addEmptyNode(*tree);

    connectNodes(curNode, (*tree)->root, true);

    unsigned short leavesMetCount = 0;
    while (leavesMetCount < maxLeavesCount){
        const char bit = ExtractBit(&miniBuffer);

        if (bit == -1){
            DestroyTree(*tree);
            return INCORRECT_COMPRESSION;
        }

        if (bit){
            struct Node* newNode = addNode(*tree, EMPTY_NODE);
            connectNodes(newNode, curNode, true);
            curNode = newNode;
        }
        else{
            int curSymbol = ExtractSymbol(&miniBuffer);

            if (curSymbol == EOF){
                DestroyTree(*tree);
                return INCORRECT_COMPRESSION;
            }

            curNode->byte = (Byte) curSymbol;
            curNode = findParentWithoutRightChild(curNode);
            if (curNode){
                struct Node* newNode = addNode(*tree, EMPTY_NODE);
                connectNodes(newNode, curNode, false);
                curNode = newNode;
            }

            leavesMetCount++;
        }
    }

    return SUCCESS;
}
Status FillTreeByFrequencies(struct Tree* tree, const unsigned long long* frequencies){
    struct Queue* queue = CreateQueue(tree->maxLeavesCount);
    if (!queue){
        return NO_MEMORY;
    }

    for (unsigned short i = 0; i < ALPHABET_SIZE; i++){
        const unsigned int curFrequency = frequencies[i];
        if (curFrequency){
            struct Node* curNode = addNode(tree, createLeaf(i, curFrequency));
            Insert(queue, curNode);
        }
    }

    for (unsigned short i = 0; i < tree->maxLeavesCount - 1; i++){
        struct Node* node1 = ExtractMinimum(queue);
        struct Node* node2 = ExtractMinimum(queue);

        struct Node* newNode = uniteNodes(tree, node1, node2);
        Insert(queue, newNode);
    }

    tree->root = &(tree->nodes[tree->nodesCount - 1]);

    DestroyQueue(queue);

    return SUCCESS;
}
void FillCodes(const struct Tree* tree, Code codes[]){
    if (tree->nodesCount == 1){
        SET_BIT_CODE(codes[tree->root->byte], 0, 0)
        codes[tree->root->byte].bitsCount = 1;
        return;
    }



    for (unsigned short leafIndex = 0; leafIndex < tree->maxLeavesCount; leafIndex++){
        struct Node* curNode = getLeaf(tree, leafIndex);
        const Byte curByte = curNode->byte;
        const unsigned char curAncestorsCount = getAncestorsCount(curNode);
        while (curNode->parent){
            SET_BIT_CODE(
                    codes[curByte],
                    curAncestorsCount - codes[curByte].bitsCount - 1,
                    !(curNode->isLeftChild))
            codes[curByte].bitsCount++;
            curNode = curNode->parent;
        }
    }
}
Status PrintTree(const struct Tree* tree, FILE* out){
    if (tree->nodesCount == 1){
        return fprintf(out, "%c%c", 0, tree->root->byte) == 2 ? SUCCESS : EXECUTION_ERROR;
    }

    struct PrintingBuffer printingBuffer;
    InitPrintingBuffer(&printingBuffer, out);

    HANDLE_OPERATION(pushMaxLeavesCount(tree, &printingBuffer))
    HANDLE_OPERATION(pushSubTree(tree->root->leftChild, &printingBuffer))
    HANDLE_OPERATION(pushSubTree(tree->root->rightChild, &printingBuffer))

    return DumpPrintingBuffer(&printingBuffer);
}
void DestroyTree(struct Tree* tree){
    if (!tree){
        return;
    }

    free(tree->nodes);
    free(tree);
}
