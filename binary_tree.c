#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"

typedef struct Node
{
	KeyValPair *kvp;
	struct Node *left;
	struct Node *right;
} Node;

struct BinaryTree
{
	Node *root;
	CmpFn cmp_fn;
	KeyDestroyFn key_destroy_fn;
	ValDestroyFn val_destroy_fn;
};

void key_val_pair_destroy(KeyValPair *kvp)
{
	free(kvp);
}

BinaryTree *binary_tree_construct(
	CmpFn cmp_fn, KeyDestroyFn key_destroy_fn,
	ValDestroyFn val_destroy_fn)
{
	BinaryTree *bt = malloc(sizeof(BinaryTree));
	bt->root = NULL;
	bt->cmp_fn = cmp_fn;
	bt->key_destroy_fn = key_destroy_fn;
	bt->val_destroy_fn = val_destroy_fn;
	return bt;
}

Node *node_construct(KeyValPair *kvp, Node *left, Node *right)
{
	Node *new_node = malloc(sizeof(Node));
	new_node->kvp = kvp;
	new_node->left = left;
	new_node->right = right;
	return new_node;
}

void node_destroy(Node *node)
{
	free(node);
}

Node *_add_recursive(Node *node, KeyValPair *kvp, CmpFn cmp_fn)
{
	if (node == NULL)
	{
		return node_construct(kvp, NULL, NULL);
	}

	int cmp = cmp_fn(kvp->key, node->kvp->key);
	if (cmp < 0)
	{
		node->left = _add_recursive(node->left, kvp, cmp_fn);
	}
	else if (cmp > 0)
	{
		node->right = _add_recursive(node->right, kvp, cmp_fn);
	}

	return node;
}

KeyValPair *key_val_pair_construct(void *key, void *value)
{
	KeyValPair *kv = malloc(sizeof(KeyValPair));
	kv->key = key;
	kv->value = value;

	return kv;
}

void binary_tree_add(BinaryTree *bt, void *key, void *value)
{
	Node *new_node = node_construct(key_val_pair_construct(key, value), NULL, NULL);

	if (bt->root == NULL)
		bt->root = new_node;
	else
	{
		Node *node = bt->root;

		while (node != NULL)
		{
			if (bt->cmp_fn(key, node->kvp->key) == 0)
			{
				node_destroy(new_node);
				return;
			}
			else if (bt->cmp_fn(key, node->kvp->key) < 0)
			{
				if (node->left == NULL)
				{
					node->left = new_node;
					return;
				}
				else
					node = node->left;
			}
			else
			{
				if (node->right == NULL)
				{
					node->right = new_node;
					return;
				}
				else
					node = node->right;
			}
		}
	}
}

void *_get_recursive(Node *node, void *key, CmpFn cmp_fn)
{
	if (node == NULL)
	{
		return NULL;
	}

	int cmp = cmp_fn(key, node->kvp->key);
	if (cmp == 0)
	{
		return node->kvp->value;
	}
	else if (cmp < 0)
	{
		return _get_recursive(node->left, key, cmp_fn);
	}
	else
	{
		return _get_recursive(node->right, key, cmp_fn);
	}

	// return NULL;
}

void *binary_tree_get(BinaryTree *bt, void *key)
{
	if (bt == NULL || bt->root == NULL || key == NULL)
	{
		// Verifica se a árvore ou a chave são válidas
		return NULL;
	}
	return _get_recursive(bt->root, key, bt->cmp_fn);
}

void binary_tree_remove(BinaryTree *bt, void *key)
{
	Node *actual = bt->root;
	Node *parent = NULL;

	// 1. Encontrar o nó a ser removido e seu pai
	while (actual != NULL && bt->cmp_fn(key, actual->kvp->key) != 0)
	{
		parent = actual;
		if (bt->cmp_fn(key, actual->kvp->key) < 0)
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
		return; // Nó não encontrado
	}

	// 2. Caso 1: Nó é uma folha (sem filhos)
	if (actual->left == NULL && actual->right == NULL)
	{
		if (parent == NULL)
		{
			bt->root = NULL; // O nó era a raiz
		}
		else if (parent->left == actual)
		{
			parent->left = NULL;
		}
		else
		{
			parent->right = NULL;
		}

		// Liberar a memória do nó e do KeyValPair
		bt->key_destroy_fn(actual->kvp->key);
		key_val_pair_destroy(actual->kvp);
		free(actual);
	}
	// 3. Caso 2: Nó tem apenas um filho à direita
	else if (actual->left == NULL)
	{
		Node *child = actual->right;
		if (parent == NULL)
		{
			bt->root = child; // O nó era a raiz
		}
		else if (parent->left == actual)
		{
			parent->left = child;
		}
		else
		{
			parent->right = child;
		}

		// Liberar a memória do nó e do KeyValPair
		bt->key_destroy_fn(actual->kvp->key);
		key_val_pair_destroy(actual->kvp);
		free(actual);
	}
	// 4. Caso 3: Nó tem apenas um filho à esquerda
	else if (actual->right == NULL)
	{
		Node *child = actual->left;
		if (parent == NULL)
		{
			bt->root = child; // O nó era a raiz
		}
		else if (parent->left == actual)
		{
			parent->left = child;
		}
		else
		{
			parent->right = child;
		}

		// Liberar a memória do nó e do KeyValPair
		bt->key_destroy_fn(actual->kvp->key);
		key_val_pair_destroy(actual->kvp);
		free(actual);
	}
	// 5. Caso 4: Nó tem dois filhos
	else
	{
		// Encontrar o sucessor in-order (menor nó da subárvore direita)
		Node *successor_parent = actual;
		Node *successor = actual->right;

		while (successor->left != NULL)
		{
			successor_parent = successor;
			successor = successor->left;
		}

		// Substituir a chave e valor do nó a ser removido
		KeyValPair *old_kvp = actual->kvp;
		actual->kvp = successor->kvp;

		// Remover o sucessor (que terá no máximo um filho à direita)
		if (successor_parent->left == successor)
		{
			successor_parent->left = successor->right;
		}
		else
		{
			successor_parent->right = successor->right;
		}

		// Liberar a memória do KeyValPair antigo
		bt->key_destroy_fn(old_kvp->key);
		key_val_pair_destroy(old_kvp);

		// Liberar o nó sucessor
		free(successor);
	}
}

KeyValPair *binary_tree_pop_min(BinaryTree *bt)
{
	if (bt == NULL || bt->root == NULL)
		return NULL;

	Node *current = bt->root;

	// Find the leftmost node (min)
	while (current->left != NULL)
	{
		current = current->left;
	}

	KeyValPair *kvp = current->kvp;

	// // Adjust parent links
	// if (parent == NULL)
	// {
	// 	// The root is the minimum node
	// 	bt->root = current->right;
	// }
	// else
	// {
	// 	parent->left = current->right;
	// }

	// free(current); // Free the node but not the KeyValPair
	return kvp;
}

KeyValPair *binary_tree_pop_max(BinaryTree *bt)
{
	if (bt == NULL || bt->root == NULL)
		return NULL;

	Node *current = bt->root;

	// Find the rightmost node (max)
	while (current->right != NULL)
	{
		current = current->right;
	}

	KeyValPair *kvp = current->kvp;

	// // Adjust parent links
	// if (parent == NULL)
	// {
	// 	// The root is the maximum node
	// 	bt->root = current->left;
	// }
	// else
	// {
	// 	parent->right = current->left;
	// }

	// free(current); // Free the node but not the KeyValPair
	return kvp;
}

void _inorder(Node *node, Vector *v)
{
	if (node == NULL)
	{
		return;
	}
	_inorder(node->left, v);
	vector_push_back(v, node->kvp);
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
	vector_push_back(v, node->kvp);
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
	vector_push_back(v, node->kvp);
}

Vector *binary_tree_postorder_traversal_recursive(BinaryTree *bt)
{
	Vector *v3 = vector_construct();
	_postorder(bt->root, v3);

	return v3;
}

static void _node_destroy(Node *node, KeyDestroyFn key_destroy_fn, ValDestroyFn val_destroy_fn)
{
	if (node == NULL)
	{
		return;
	}
	_node_destroy(node->left, key_destroy_fn, val_destroy_fn);
	_node_destroy(node->right, key_destroy_fn, val_destroy_fn);

	// Destruir o KeyValPair do nó
	key_destroy_fn(node->kvp->key);
	val_destroy_fn(node->kvp->value);
	free(node->kvp);

	// Liberar o nó em si
	free(node);
}

Node *_search_recursive(Node *node, void *min_key, void *max_key, Vector *v, CmpFn cmp_fn)
{

	if (node == NULL)
	{
		return NULL;
	}

	_search_recursive(node->left, min_key, max_key, v, cmp_fn);
	if (cmp_fn(node->kvp->key, min_key) >= 0 && cmp_fn(node->kvp->key, max_key) <= 0)
	{
		vector_push_back(v, node->kvp->value);
	}
	_search_recursive(node->right, min_key, max_key, v, cmp_fn);

	return NULL;
}

Vector *binary_tree_interval(BinaryTree *bt, void *min_key, void *max_key)
{

	Vector *v = vector_construct();
	Node *node = bt->root;

	_search_recursive(node, min_key, max_key, v, bt->cmp_fn);
	return v;
}

void _destroy_node_recursive(Node *node)
{
	if (node == NULL)
		return;

	_destroy_node_recursive(node->left);
	_destroy_node_recursive(node->right);
	node_destroy(node);
}

void binary_tree_destroy(BinaryTree *bt)
{
	_destroy_node_recursive(bt->root);
	free(bt);
}