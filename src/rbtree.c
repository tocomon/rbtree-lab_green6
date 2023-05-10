#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h> // malloc, calloc, free

rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *null_node = (node_t *)calloc(1, sizeof(node_t));
  t->root = null_node;
  t->nil = null_node;
  t->nil->color = RBTREE_BLACK;
  return t;
}

void delete_node(rbtree *t, node_t *p){
  if (p != t->nil) { // NULL이 아닌 경우
    delete_node(t, p->left); // 왼쪽 서브트리 순회
    delete_node(t, p->right); // 오른쪽 서브트리 순회
    free(p); // 노드 메모리 해제
  }
}

void delete_rbtree(rbtree *t) {
  delete_node(t, t->root); // 노드 순회 및 메모리 해제
  free(t->nil); // sentinel 메모리 해제
  free(t); // rbtree 구조체 메모리 해제
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;
  x->right = y->left;
  if(y->left != t->nil){
    y->left->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  } 
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else{
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x){
  node_t *y = x->left;
  x->left = y->right;
  if(y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;
  if(x->parent == t->nil){
    t->root = y;
  }
  else if(x == x->parent->right){
    x->parent->right = y;
  }
  else{
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void insert_fixup(rbtree *t, node_t * z){
  node_t * y = NULL;
  while(z->parent->color == RBTREE_RED){
    if(z->parent == z->parent->parent->left){
      y = z->parent->parent->right;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if(z == z->parent->right){
          z = z->parent;
          left_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate(t,z->parent->parent);
      }
    }
    else{
      y = z->parent->parent->left;
      if(y->color == RBTREE_RED){
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else {
        if(z == z->parent->left){
          z = z->parent;
          right_rotate(t,z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t,z->parent->parent);
      }     
    }
  }
  t->root->color = RBTREE_BLACK;
}
  
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *x = t->root;
  node_t *y = t->nil;
  node_t *z = (node_t*)calloc(1, sizeof(node_t));
  z->key = key;
  while(x != t->nil){
    y = x;
    if(key < x->key){
      x = x->left;
    }
    else{
      x = x->right;
    }
  }
  z->parent = y;
  if(y == t->nil){
    t->root = z;
  }
  else if(z->key < y->key){
    y->left = z;
  }
  else{
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;
  insert_fixup(t, z);
  return t->root;
}

node_t *search_node(const rbtree *t, node_t* root, const key_t key){
  if(root == t->nil){
    return NULL;
  }else if(root->key == key){
    return root;
  }else if (root->key >key){
    return search_node(t, root->left, key);
  }else{
    return search_node(t, root->right, key);
  }
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  return search_node(t, t->root, key);
}

node_t *tree_min(rbtree *t, node_t *x) {
  // TODO: implement find
  while (x->left != t->nil){
    x= x->left;
  }
  return x;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *p = t->root;
  // for (p = t->root; p->left != NULL; p = p->left);
  while (p->left != t->nil){
    p = p->left;
  }
  return p;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *p = t->root;
  // for (p = t->root; p->right != NULL; p = p->right);
  while (p->right != t->nil){
    p = p->right;
  }
  return p;
}

void transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil){
    t->root = v;
  }
  else if(u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  v->parent = u->parent;
}

void rbtree_delete_fixup(rbtree *t, node_t* x){
  node_t *w = NULL;
  while(x != t->root && x->color == RBTREE_BLACK){
    if(x == x->parent->left){
      w = x->parent->right;

      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate(t, x->parent);
        w = x->parent->right;
      }
      if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        
        if (w->right->color == RBTREE_BLACK){
          // w->left->color == RBTREE_BLACK;
          // w->color == RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);
        x = t->root;
      }
    }else{
      w = x->parent->left;
      if (w->color == RBTREE_RED){
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK){
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        if (w->left->color == RBTREE_BLACK){
          // w->right->color == RBTREE_BLACK;
          // w->color == RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  if (p == NULL){
    return 0;
  }
  node_t *x = NULL;

  node_t *y = p;
  color_t y_original_color = y->color;
  if(p->left == t->nil){
    x = p->right;
    transplant(t,p,p->right);
  }
  else if(p->right == t->nil){
    x = p->left;
    transplant(t,p,p->left);
  }
  else{
    y = tree_min(t, p->right);
    y_original_color = y->color;
    x = y->right;
    if(y->parent == p){
      x->parent = y;
    }
    else{
      transplant(t,y,y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    transplant(t,p,y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if(y_original_color == RBTREE_BLACK){
    rbtree_delete_fixup(t,x);
  }

  free(p);
  return 0;
}


void indorder_to_array(const rbtree *t, node_t *root, int *pidx, key_t *arr){
  if (root != t->nil){
    indorder_to_array(t, root->left, pidx, arr);
    arr[(*pidx)++] = root->key;
    indorder_to_array(t, root->right, pidx, arr);
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int idx = 0;
  int *pidx = &idx;
  indorder_to_array(t, t->root, pidx, arr);
  return 0;
}
