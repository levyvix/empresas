#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

typedef struct Node
{
	void *value;
	struct Node *left;
	struct Node *right;
} Node;

struct BinaryTree
{
	Node *root;
	CmpFn cmp_fn;
	GTK get_key;
	ValDestroyFn val_destroy_fn;
};

// Helper function to find the minimum value node in a subtree
static Node *find_min_node(Node *node)
{
	while (node->left != NULL)
		node = node->left;
	return node;
}

// Helper function to recursively destroy nodes
static void _destroy_node_recursive(Node *node, ValDestroyFn val_destroy_fn)
{
	if (node == NULL)
		return;

	_destroy_node_recursive(node->left, val_destroy_fn);
	_destroy_node_recursive(node->right, val_destroy_fn);

	if (val_destroy_fn != NULL)
		val_destroy_fn(node->value);
	free(node);
}

BinaryTree *binary_tree_construct(CmpFn cmp_fn, GTK get_key, ValDestroyFn val_destroy_fn)
{
	BinaryTree *bt = malloc(sizeof(BinaryTree));
	bt->root = NULL;
	bt->cmp_fn = cmp_fn;
	bt->get_key = get_key;
	bt->val_destroy_fn = val_destroy_fn;
	return bt;
}

Node *node_construct(void *value, Node *left, Node *right)
{
	Node *new_node = malloc(sizeof(Node));
	new_node->value = value;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

static Node *_add_recursive(Node *node, void *value, CmpFn cmp_fn)
{
	if (node == NULL)
		return node_construct(value, NULL, NULL);

	int cmp = cmp_fn(value, node->value);
	if (cmp < 0)
		node->left = _add_recursive(node->left, value, cmp_fn);
	else if (cmp > 0)
		node->right = _add_recursive(node->right, value, cmp_fn);

	return node;
}

void binary_tree_add(BinaryTree *bt, void *value)
{
	bt->root = _add_recursive(bt->root, value, bt->cmp_fn);
}

static Node *_remove_recursive(Node *node, void *key, CmpFn cmp_fn, void **removed_value)
{
	if (node == NULL)
		return NULL;

	int cmp = cmp_fn(key, node->value);
	
	if (cmp < 0)
		node->left = _remove_recursive(node->left, key, cmp_fn, removed_value);
	else if (cmp > 0)
		node->right = _remove_recursive(node->right, key, cmp_fn, removed_value);
	else
	{
		*removed_value = node->value;

		// Case 1: No children
		if (node->left == NULL && node->right == NULL)
		{
			free(node);
			return NULL;
		}
		// Case 2: One child
		else if (node->left == NULL)
		{
			Node *temp = node->right;
			free(node);
			return temp;
		}
		else if (node->right == NULL)
		{
			Node *temp = node->left;
			free(node);
			return temp;
		}
		// Case 3: Two children
		else
		{
			Node *successor = find_min_node(node->right);
			node->value = successor->value;
			node->right = _remove_recursive(node->right, successor->value, cmp_fn, &successor->value);
		}
	}
	return node;
}

void binary_tree_remove(BinaryTree *bt, void *key)
{
	void *removed_value = NULL;
	bt->root = _remove_recursive(bt->root, key, bt->cmp_fn, &removed_value);
}

void *binary_tree_pop_min(BinaryTree *bt)
{
	if (bt->root == NULL)
		return NULL;

	Node *parent = NULL;
	Node *current = bt->root;

	while (current->left != NULL)
	{
		parent = current;
		current = current->left;
	}

	void *min_value = current->value;

	if (parent == NULL)
		bt->root = current->right;
	else
		parent->left = current->right;

	free(current);
	return min_value;
}

void *binary_tree_pop_max(BinaryTree *bt)
{
	if (bt->root == NULL)
		return NULL;

	Node *parent = NULL;
	Node *current = bt->root;

	while (current->right != NULL)
	{
		parent = current;
		current = current->right;
	}

	void *max_value = current->value;

	if (parent == NULL)
		bt->root = current->left;
	else
		parent->right = current->left;

	free(current);
	return max_value;
}

static void _inorder_recursive(Node *node, Vector *v)
{
	if (node == NULL)
		return;

	_inorder_recursive(node->left, v);
	vector_push_back(v, node->value);
	_inorder_recursive(node->right, v);
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt)
{
	Vector *v = vector_construct();
	_inorder_recursive(bt->root, v);
	return v;
}

static void _interval_recursive(Node *node, void *min_key, void *max_key, Vector *result, CmpFn cmp_fn)
{
	if (node == NULL)
		return;

	int cmp_min = cmp_fn(node->value, min_key);
	int cmp_max = cmp_fn(node->value, max_key);

	if (cmp_min >= 0)
		_interval_recursive(node->left, min_key, max_key, result, cmp_fn);

	if (cmp_min >= 0 && cmp_max <= 0)
		vector_push_back(result, node->value);

	if (cmp_max <= 0)
		_interval_recursive(node->right, min_key, max_key, result, cmp_fn);
}

Vector *binary_tree_interval(BinaryTree *bt, void *min_key, void *max_key)
{
	Vector *result = vector_construct();
	_interval_recursive(bt->root, min_key, max_key, result, bt->cmp_fn);
	return result;
}

void binary_tree_destroy(BinaryTree *bt)
{
	_destroy_node_recursive(bt->root, bt->val_destroy_fn);
	free(bt);
}