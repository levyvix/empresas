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

BinaryTree *binary_tree_construct(
	CmpFn cmp_fn, GTK get_key, ValDestroyFn val_destroy_fn)
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

void node_destroy(Node *node)
{
	free(node);
}

Node *_add_recursive(Node *node, void *value, CmpFn cmp_fn)
{
	if (node == NULL)
	{
		return node_construct(value, NULL, NULL);
	}

	int cmp = cmp_fn(value, node->value);
	if (cmp < 0)
	{
		node->left = _add_recursive(node->left, value, cmp_fn);
	}
	else if (cmp > 0)
	{
		node->right = _add_recursive(node->right, value, cmp_fn);
	}

	return node;
}

void binary_tree_add(BinaryTree *bt, void *value)
{
	bt->root = _add_recursive(bt->root, value, bt->cmp_fn);
}

void *_get_recursive(Node *node, void *key, CmpFn cmp_fn)
{
	if (node == NULL)
	{
		return NULL;
	}

	int cmp = cmp_fn(key, node->value);
	if (cmp == 0)
	{
		return node->value;
	}
	else if (cmp < 0)
	{
		return _get_recursive(node->left, key, cmp_fn);
	}
	else
	{
		return _get_recursive(node->right, key, cmp_fn);
	}

	return NULL;
}

void *binary_tree_get(BinaryTree *bt, void *key)
{
	return _get_recursive(bt->root, key, bt->cmp_fn);
}

void binary_tree_remove(BinaryTree *bt, void *key)
{
	Node *actual = bt->root;
	Node *parent = NULL;

	while (actual != NULL && bt->cmp_fn(key, actual->value) != 0)
	{
		parent = actual;
		if (bt->cmp_fn(key, actual->value) < 0)
		{
			actual = actual->left;
		}
		else
		{
			actual = actual->right;
		}
	}

	if (actual == NULL)
	{
		return;
	}

	if (actual->left == NULL && actual->right == NULL)
	{
		if (parent == NULL)
		{
			bt->root = NULL;
		}
		else if (parent->left == actual)
		{
			parent->left = NULL;
		}
		else
		{
			parent->right = NULL;
		}
		bt->val_destroy_fn(actual->value);
		free(actual);
	}

	else if (actual->left == NULL)
	{
		Node *child = actual->right;
		if (parent == NULL)
		{
			bt->root = child;
		}
		else if (parent->left == actual)
		{
			parent->left = child;
		}
		else
		{
			parent->right = child;
		}
		bt->val_destroy_fn(actual->value);

		free(actual);
	}

	else if (actual->right == NULL)
	{
		Node *child = actual->left;
		if (parent == NULL)
		{
			bt->root = child;
		}
		else if (parent->left == actual)
		{
			parent->left = child;
		}
		else
		{
			parent->right = child;
		}
		bt->val_destroy_fn(actual->value);

		free(actual);
	}

	else
	{

		Node *successor_parent = actual;
		Node *successor = actual->right;

		while (successor->left != NULL)
		{
			successor_parent = successor;
			successor = successor->left;
		}

		actual->value = successor->value;

		if (successor_parent->left == successor)
		{
			successor_parent->left = successor->right;
		}
		else
		{
			successor_parent->right = successor->right;
		}

		free(successor);
	}
}

void *binary_tree_pop_min(BinaryTree *bt)
{
	if (bt == NULL || bt->root == NULL)
		return NULL;

	Node *current = bt->root;

	while (current->left != NULL)
	{
		current = current->left;
	}

	return current->value;
}

void *binary_tree_pop_max(BinaryTree *bt)
{
	if (bt == NULL || bt->root == NULL)
		return NULL;

	Node *current = bt->root;

	while (current->right != NULL)
	{
		current = current->right;
	}

	return current->value;
}

void _inorder(Node *node, Vector *v)
{
	if (node == NULL)
	{
		return;
	}
	_inorder(node->left, v);
	vector_push_back(v, node->value);
	_inorder(node->right, v);
}

Vector *binary_tree_inorder_traversal_recursive(BinaryTree *bt)
{

	Vector *v1 = vector_construct();
	_inorder(bt->root, v1);

	return v1;
}

void _preorder(Node *node, Vector *v)
{
	if (node == NULL)
	{
		return;
	}
	vector_push_back(v, node->value);
	_preorder(node->left, v);
	_preorder(node->right, v);
}

Vector *binary_tree_preorder_traversal_recursive(BinaryTree *bt)
{

	Vector *v2 = vector_construct();
	_preorder(bt->root, v2);

	return v2;
}

void _postorder(Node *node, Vector *v)
{
	if (node == NULL)
	{
		return;
	}
	_postorder(node->left, v);
	_postorder(node->right, v);
	vector_push_back(v, node->value);
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt)
{
	Vector *v3 = vector_construct();
	_postorder(bt->root, v3);

	return v3;
}

void _search_recursive(Node *node, void *min_key, void *max_key, Vector *v, CmpFn cmp_fn)
{

	if (node == NULL)
	{
		return;
	}

	_search_recursive(node->left, min_key, max_key, v, cmp_fn);
	if (cmp_fn(node->value, min_key) >= 0 && cmp_fn(node->value, max_key) <= 0)
	{
		vector_push_back(v, node->value);
	}
	_search_recursive(node->right, min_key, max_key, v, cmp_fn);
}

Vector *binary_tree_interval(BinaryTree *bt, void *min_key, void *max_key)
{

	Vector *v = vector_construct();
	Node *node = bt->root;

	_search_recursive(node, min_key, max_key, v, bt->cmp_fn);
	return v;
}

void _destroy_node_recursive(Node *node, ValDestroyFn val_destroy_fn)
{
	if (node == NULL)
		return;

	_destroy_node_recursive(node->left, val_destroy_fn);
	_destroy_node_recursive(node->right, val_destroy_fn);
	val_destroy_fn(node->value);
	node_destroy(node);
}

double _fabs(double x)
{

	return x > 0 ? x : -x;
}

void _busca_proximo(Node *node, void *key, Node **closest, double *min_distance, CmpFn cmp_fn, GTK get_key)
{
	if (node == NULL)
		return;

	int cmp = cmp_fn(key, node->value);
	if (cmp == 0)
	{
		_busca_proximo(node->left, key, closest, min_distance, cmp_fn, get_key);
		_busca_proximo(node->right, key, closest, min_distance, cmp_fn, get_key);
		return;
	}

	double current_distance = _fabs(*(double *)get_key(key) - *(double *)get_key(node->value));

	if (current_distance < *min_distance)
	{
		*min_distance = current_distance;
		*closest = node;
	}

	if (current_distance == *min_distance)
	{
		// comparaco entre closest e actual
		Node *node_closest = *closest;
		cmp = cmp_fn(node_closest->value, node->value);

		// se o actual for mais proximo do que o closest
		if (cmp > 0)
			*closest = node;
	}

	_busca_proximo(node->left, key, closest, min_distance, cmp_fn, get_key);
	_busca_proximo(node->right, key, closest, min_distance, cmp_fn, get_key);
}
void *binary_tree_find_nearest(BinaryTree *bt, void *key)
{
	if (bt == NULL || bt->root == NULL || key == NULL)
		return NULL;

	double min_distance = 999999;
	Node *closest = NULL;

	_busca_proximo(bt->root, key, &closest, &min_distance, bt->cmp_fn, bt->get_key);

	return (closest != NULL) ? closest->value : NULL;
}

/// PRINT TREE
void _print_tree(Node *node, int level, void *(*get_key)(void *))
{
	if (node == NULL)
		return;

	_print_tree(node->right, level + 1, get_key);

	for (int i = 0; i < level; i++)
		printf("    ");

	printf("%lf (%s)\n", *(double *)get_key(node->value), (char *)node->value);

	_print_tree(node->left, level + 1, get_key);
}

void print_tree(BinaryTree *bt)
{
	_print_tree(bt->root, 0, bt->get_key);
}

/// PRINT TREE

void binary_tree_destroy(BinaryTree *bt)
{
	_destroy_node_recursive(bt->root, bt->val_destroy_fn);
	free(bt);
}