
#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "vector.h"

typedef int (*CmpFn)(void *, void *);
typedef void (*ValDestroyFn)(void *);
typedef void *(*GTK)(void *);

typedef struct BinaryTree BinaryTree;

BinaryTree *binary_tree_construct(
	CmpFn cmp_fn, GTK get_key,
	ValDestroyFn val_destroy_fn);
void binary_tree_add(BinaryTree *bt, void *value);
void binary_tree_remove(BinaryTree *bt, void *key);

void *binary_tree_pop_min(BinaryTree *bt);
void *binary_tree_pop_max(BinaryTree *bt);
void *binary_tree_find_nearest(BinaryTree *bt, void *key);
Vector *binary_tree_interval(BinaryTree *bt, void *min_key, void *max_key);
void *binary_tree_get(BinaryTree *bt, void *key);
void binary_tree_destroy(BinaryTree *bt);
void print_tree(BinaryTree *bt);

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt);
Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt);
Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt);

#endif