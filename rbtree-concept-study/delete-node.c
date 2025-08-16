#include <stdio.h>
#include <stdlib.h>

typedef enum { RED, BLACK } Color;

typedef struct Node {
    int key;
    Color color;
    struct Node *left, *right, *parent;
} Node;

typedef struct {
    Node *root;
    Node *NIL; // 경계 노드(sentinel)
} RBTree;

// 트리에서 최솟값 찾기 : 우측 자식 서브트리에서의 최소값 찾는 용도
Node* rb_minimum(RBTree *tree, Node *x)
{
    while(x->left != NULL)
    {
        x = x->left;
    }

    return x;
}

// 트리에서 u 위치를 v로 교체하는 함수
void rb_transplant(RBTree *tree, Node *u, Node *v)
{
    if(u->parent == tree->NIL)
    {
        // u가 루트 노드라는 소리
        tree->root = v;
    }
    else if(u == u->parent->left)
    {
        // u가 부모의 왼쪽 자식노드인 경우
        u->parent->left = v;
    }
    else
    {
        // u가 부모의 오른쪽 노드인 경우
        u->parent->right = v;
    }

    v->parent = u->parent;
}

void rotate_left(RBTree *tree, Node *node)
{
    Node *temp = node->right;
    node->right = temp->left;

    if((temp->left) != (tree->NIL))
    {
        temp->left->parent = node;
    }

    temp->parent = node->parent;
    if(node->parent == tree->NIL)
    {
        tree->root = temp;
    }
    else if(node == node->parent->left)
    {
        node->parent->left = temp;
    }
    else
    {
        node->parent->right = temp;
    }

    temp->left = node;
    node->parent = temp;
}

void rotate_right(RBTree *tree, Node *node)
{
    Node *temp = node->left;

    node->left = temp->right;
    if(temp->right != tree->NIL)
    {
        temp->right->parent = node;
    }

    temp->parent = node->parent;
    if(node->parent == tree->NIL)
    {
        tree->root = temp;
    }
    else if(node == node->parent->left)
    {
        node->parent->left = temp;
    }
    else
    {
        node->parent->right = temp;
    }

    temp->right = node;
    node->parent = temp;
}

// tree : rbtree, z : 삭제하고자 하는 노드
void rb_delete(RBTree *tree, Node *z)
{
    Node *y = z; // 삭제될 노드
    Node *x; // 삭제될 노드를 대체할 노드(대체 노드)
    Color y_original_color = y->color;

    // 왼쪽 자식이 없는 경우
    if(z->left == tree->NIL)
    {
        x = z->right;
        rb_transplant(tree, z, z->right);
    }
    // 오른쪽 자식이 없는 경우
    else if(z->right == tree->NIL)
    {
        x = z->left;
        rb_transplant(tree, z, z->left);
    }
    // 자식 노드가 2개인 경우
    else
    {
        y = rb_minimum(tree, z->right); // 우측 서브트리의 최대값(후계자) 찾기
        y_original_color = y->color;

        x = y->right; // 대체 노드 : 우선 y의 우측 자식 노드로 설정

        if(y != z->right)
        {
            rb_transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        else
        {
            x->parent = y;
        }

        // 삭제 노드 위치에 후계자(y) 연결
        rb_transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    if(y_original_color == BLACK)
    {
        rb_delete_fixup(tree, x);
    }

    // 메모리 할당 해제
    free(z);
}

// tree : rbtree, x : 대체 노드
void rb_delete_fixup(RBTree *tree, Node *x)
{
    while(x != tree->root && x->color == BLACK)
    {
        // x가 왼쪽 자식인 경우
        if(x == x->parent->left)
        {
            Node *w = x->parent->right; // x의 형제 노드

            // 1) 형제가 RED
            if(w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(tree, x->parent);
                w = x->parent->right; // 부모를 기준으로 좌회전했으니, 부모의 우측 노드를 형제 노드로 변경
            }

            // 2) 형제가 BLACK이고 자식 둘 다 BLACK
            if(((w->left->color) == BLACK) && ((w->right->color) == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                // 형제 노드의 두 자식 노드 중 하나는 RED
                // 3) 형제가 BLACK, 왼쪽 자식 RED, 오른쪽 자식 BLACK
                if(w->right->color == BLACK)
                {
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(tree, w);
                    w = x->parent->right; // 우회전 한 이후 형제 노드를 갱신
                }

                // 4) 형제가 BLACK, 오른쪽 자식 RED
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(tree, x->parent);
                x = tree->root;
            }
        }
        else
        {
            // x가 오른쪽 자식인 경우
            Node *w = x->parent->left; // x의 형제 노드

            // 1. 형제 노드가 RED인 경우
            if(w->color == RED)
            {
                w->color = BLACK;
                x->parent->color = RED;
                rotate_left(tree, x->parent);
                w = x->parent->left;
            }

            // 2. 형제노드 & 형제 노드의 두 자식 노드가 BLACK인 경우
            if(((w->left->color) == BLACK) && ((w->right->color) == BLACK))
            {
                w->color = RED;
                x = x->parent;
            }
            else
            {
                // 3. 형제 노드의 두 자식 노드 중 오른쪽 노드가 RED인 경우
                if((w->left->color) == BLACK)
                {
                    // 국소적으로 left_rotate 필요
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(tree, w);
                    x = x->parent;
                }
                // 4. 형제 노드의 두 자식 노드 중 왼쪽 노드가 RED인 경우
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rotate_right(tree, x->parent);
                x = tree->root;
            }
        }
    }

    x->color = BLACK;
}