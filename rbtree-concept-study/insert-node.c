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

/* 회전 연산 */
void rotate_left(RBTree *tree, RBNode *n)
{
    RBNode *y = n->right;
    n->right = y->left;

    if(y->left != tree->nil)
    {
        y->left->parent = n;
    }

    y->parent = n->parent;
    if(n->parent == tree->nil)
    {
        tree->root = y;
    }
    else if(n == n->parent->left)
    {
        n->parent->left = n;
    }
    else
    {
        n->parent->right = n;
    }

    y->left = n;
    n->parent = y;
}

void rotate_right(RBTree *tree, RBNode *n)
{
    RBNode *y = n->left;
    n->left = y->right;

    if(y->right != tree->nil)
    {
        y->right->parent = n;
    }

    y->parent = n->parent;
    if(n->parent == tree->nil)
    {
        tree->root = y;
    }
    else if(n == n->parent->left)
    {
        n->parent->left = y;
    }
    else
    {
        n->parent->right = y;
    }

    y->right = n;
    n->parent = y;
}

void rb_insert(RBTree *tree, int key)
{
    RBNode *newNode = createNode(tree, key, RED);
    RBNode *y = tree->nil; // y는 새 노드의 최종 부모
    RBNode *x = tree->root; // 탐색

    while (x != tree->nil)
    {
        y = x;

        if((newNode->data) < (x->data))
        {
            x = x->left;
        }
        else
        {
            x = x->right;
        }
    }

    newNode->parent = y;
    if(y == tree->nil)
    {
        tree->root = newNode;
    }
    else if(newNode->data < y->data)
    {
        y->left = newNode;
    }
    else
    {
        y->right = newNode;
    }

    // RB 속성 복구
    rb_insert_fixup(tree, newNode);
}

void rb_insert_fixup(RBTree *tree, RBNode *z)
{
    // 삽입 이후 Double Red가 유지된다면 계속 while문 진행
    while((z->parent->color) == RED)
    {
        // z의 부모가 왼쪽 자식인 경우
        if(z->parent == z->parent->parent->left)
        {
            RBNode *uncle = z->parent->parent->right; // 삼촌 노드

            // 1) 부모와 삼촌 모두 빨강 노드
            if(uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->right)
                {
                    z = z->parent;
                    rotate_left(tree, z);
                }
                
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(tree, z->parent->parent);
            }
        }
        else
        {
            // 대칭 케이스 : 삽입노드의 부모 노드가 부모의 우측 자식인 경우
            RBNode *uncle = z->parent->parent->left;

            // 1) 부모와 삼촌 모두 빨강 노드
            if(uncle->color == RED)
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if(z == z->parent->left)
                {
                    z = z->parent;
                    rotate_left(tree, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(tree, z->parent->parent);
            }
        }
    }

    // 트리의 루트 컬러는 항상 검정 유지
    tree->root->color = BLACK;
}