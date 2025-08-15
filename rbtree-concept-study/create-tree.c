#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct RBNode {
    int data;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct RBTree {
    RBNode *root;
    RBNode *nil; // 경계 노드 : sentinel (항상 BLACK)
} RBTree;

RBTree* createRBTree()
{
    RBTree *tree = (RBTree *)malloc(sizeof(RBTree));
    tree->nil = (RBTree *)malloc(sizeof(RBNode));

    tree->nil->color = BLACK;
    tree->nil->left = NULL;
    tree->nil->right = NULL;
    tree->nil->parent = NULL;
    tree->root = tree->nil; // 루트 노드도 처음엔 NIL

    return tree;
}

RBNode* createNode(RBTree *tree, int data, Color color)
{
    RBNode *node = (RBNode *)malloc(sizeof(RBNode));
    node->data = data;
    node->color = color;

    node->left = tree->nil;
    node->right = tree->nil;
    node->parent = tree->nil;

    return node;
}