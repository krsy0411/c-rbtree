#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->left = NULL;
  p->nil->parent = NULL;
  p->nil->right = NULL;

  p->root = p->nil;
  return p;
}

void free_node(rbtree* t, node_t* node)
{
  if(node == t->nil)
  {
    return;
  }
  
  free_node(t, node->left);
  free_node(t, node->right);
  free(node);
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  node_t *node = t->root;

  free_node(t, node);
  free(t->nil);
  free(t);
}

void rotate_left(rbtree *t, node_t *x)
{
  node_t *y = x->right;

  // y의 좌측 노드와 x 연결관계 설정
  x->right = y->left;
  if(y->left != t->nil)
  {
    y->left->parent = x;
  }

  // x의 부모 노드와 y 연결관계 설정
  y->parent = x->parent;
  if(x->parent == t->nil)
  {
    // x가 루트 노드라는 소리
    t->root = y;
  }
  else if(x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  // x와 y 연결관계 설정
  y->left = x;
  x->parent = y;
}

void rotate_right(rbtree *t, node_t *x)
{
  node_t *y = x->left;

  // y의 우측 자식노드와 x 연결관계 설정
  x->left = y->right;
  if(y->right != t->nil)
  {
    y->right->parent = x;
  }

  // x의 부모노드와 y 연결관계 설정
  y->parent = x->parent;
  if(x->parent == t->nil)
  {
    t->root = y;
  }
  else if(x == (x->parent->left))
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  // x와 y 연결관계 설정
  y->right = x;
  x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *node)
{
  // insert때 fixup이 발생하는 경우 : "double red" 상황일 때
  while(node->parent != t->nil && (node->parent->color) == RBTREE_RED)
  {
    // 크게 부모노드가 그 부모의 왼쪽 자식인 경우 & 오른쪽 자식인 경우로 갈림
    if((node->parent) == (node->parent->parent->left))
    {
      // 우선 삼촌 노드 지정
      node_t *uncle_node = (node->parent->parent->right);

      // 1) 삼촌 노드 & 부모 노드 모두 RED
      if((uncle_node->color) == RBTREE_RED)
      {
        (node->parent->color) = RBTREE_BLACK;
        (node->parent->parent->color) = RBTREE_RED;
        (uncle_node->color) = RBTREE_BLACK;
        node = node->parent->parent; // 이후 발생할 double red 상황 대응을 위해 노드 위치 이동
      }
      else
      {
        // 삼촌 노드가 BLACK인 경우
        // 2) 현재 노드가 부모의 우측 자식노드라면
        if(node == (node->parent->right))
        {
          node = node->parent;
          rotate_left(t, node);
        }
        // 3) Double Red가 좌정렬된 경우
        (node->parent->color) = RBTREE_BLACK;
        (node->parent->parent->color) = RBTREE_RED;
        rotate_right(t, node->parent->parent);
      }
    }
    // 오른쪽 자식인 경우
    else
    {
      // 우선 삼촌 노드 지정
      node_t *uncle_node = (node->parent->parent->left);

      // 1) 삼촌 & 부모 노드 모두 빨강인 경우
      if((uncle_node->color) == RBTREE_RED)
      {
        uncle_node->color = RBTREE_BLACK;
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else
      {
        // 삼촌 노드가 검정인 경우
        // 2) 부모 노드의 왼쪽 노드에 해당하는 경우
        if(node == (node->parent->left))
        {
          node = node->parent;
          rotate_right(t, node);
        }
        
        // 3) Double Red가 우정렬된 경우
        (node->parent->color) = RBTREE_BLACK;
        (node->parent->parent->color) = RBTREE_RED;
        rotate_left(t, node->parent->parent);
      }
    }
  }

  // 루트 노드의 색은 항상 BLACK을 보장
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // 노드 생성
  node_t *node = (node_t *)calloc(1, sizeof(node_t));
  node->color = RBTREE_RED; // 처음 삽입시 색깔은 레드
  node->key = key;
  node->parent = node->left = node->right = t->nil;
  
  // 삽입할 위치 찾기(탐색)
  node_t *compare_node = t->root;
  node_t *parent_node = t->nil;
  
  while(compare_node != t->nil)
  {
    parent_node = compare_node;

    // 내려가면서 탐색
    if((node->key) < (compare_node->key))
    {
      compare_node = (compare_node->left);
    }
    else
    {
      compare_node = (compare_node->right);
    }
  }

  // 삽입 위치를 찾았으니, 링크
  node->parent = parent_node;
  if(parent_node == t->nil)
  {
    // 삽입할 위치가 루트 노드인 경우
    t->root = node;
  }
  else if((node->key) < (parent_node->key))
  {
    // 부모의 키값보다 작은 경우
    parent_node->left = node;
  }
  else
  {
    // 부모의 키값보다 큰 경우
    parent_node->right = node;
  }

  //fixup
  rbtree_insert_fixup(t, node);

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *current_node = t->root;

  // 최대한 내려가면서 키값 찾기
  while(current_node != t->nil)
  {
    if(current_node->key > key)
    {
      current_node = current_node->left;
    }
    else if(current_node->key < key)
    {
      current_node = current_node->right;
    }
    else
    {
      // 해당하는 키값을 찾은 경우
      return current_node;
    }
  }

  // 키를 찾지 못한 경우
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  if(t->root == t->nil)
  {
    return NULL;
  }

  node_t *current_node = t->root;
  while(current_node->left != t->nil)
  {
    current_node = current_node->left;
  }

  return current_node;
}

node_t *rbtree_max(const rbtree *t) {
  if(t->root == t->nil)
  {
    return NULL;
  }

  node_t *current_node = t->root;
  while(current_node->right != t->nil)
  {
    current_node = current_node->right;
  }

  return current_node;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_arraysqqq
  return 0;
}
