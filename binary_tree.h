
#ifndef _BINARY_TREE_H_
#define _BINARY_TREE_H_

#include "vector.h"

typedef struct
{
	void *key;
	void *value;
} KeyValPair;

KeyValPair *key_val_pair_construct(void *key, void *val);
void key_val_pair_destroy(KeyValPair *kvp);

typedef int (*CmpFn)(void *, void *);
typedef void (*KeyDestroyFn)(void *);
typedef void (*ValDestroyFn)(void *);

typedef struct BinaryTree BinaryTree;

BinaryTree *binary_tree_construct(
	CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,
	ValDestroyFn val_destroy_fn);
void binary_tree_add(BinaryTree *bt, void *key, void *value);
// void binary_tree_add_recursive(BinaryTree *bt, void *key, void *value);
// int binary_tree_empty(BinaryTree *bt);
void binary_tree_remove(BinaryTree *bt, void *key);
KeyValPair *binary_tree_min(BinaryTree *bt);
KeyValPair *binary_tree_max(BinaryTree *bt);
KeyValPair *binary_tree_pop_min(BinaryTree *bt);
KeyValPair *binary_tree_pop_max(BinaryTree *bt);
Vector *binary_tree_interval(BinaryTree *bt, void *min_key, void *max_key);
void *binary_tree_get(BinaryTree *bt, void *key);
void binary_tree_destroy(BinaryTree *bt);

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt);
Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt);
Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt);

#endif