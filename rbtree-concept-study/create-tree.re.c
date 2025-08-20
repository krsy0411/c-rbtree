#include <stdio.h>
#include <stdlib.h>

typedef enum { RBTREE_RED, RBTREE_BLACK } Color;
typedef struct RBNode 
{
    int data;
    Color color;
    struct RBNode *left, *right, *parent;
} RBNode;

typedef struct RBTree
{
    RBNode *root;
    RBNode *nil;
} RBTree;

RBTree* createRBTree()
{
    RBTree *tree = (RBTree *)malloc(sizeof(RBTree));

    tree->nil = (RBTree *)malloc(sizeof(RBTree));
    tree->nil->color = RBTREE_BLACK;
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