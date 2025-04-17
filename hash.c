#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

typedef struct HashNode
{
	HashTableItem item;
	struct HashNode *next;
} HashNode;

struct HashTable
{
	HashNode **buckets;
	int table_size;
	int num_elems;
	HashFunction hash_fn;
	CmpFunction cmp_fn;
};

HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn)
{
	HashTable *hash_tbl = calloc(1, sizeof(HashTable));

	hash_tbl->table_size = table_size;
	hash_tbl->num_elems = 0;
	hash_tbl->hash_fn = hash_fn;
	hash_tbl->cmp_fn = cmp_fn;
	hash_tbl->buckets = calloc(table_size, sizeof(HashNode *));

	return hash_tbl;
}

void *hash_table_set(HashTable *h, void *key, void *val)
{
	int index = h->hash_fn(h, key) % h->table_size;

	HashNode *node = h->buckets[index];

	while (node != NULL)
	{
		if (h->cmp_fn(node->item.key, key) == 0)
		{
			void *old_val = node->item.val;
			node->item.val = val;
			return old_val;
		}
		node = node->next;
	}

	HashNode *new_node = calloc(1, sizeof(HashNode));
	if (new_node == NULL)
	{
		fprintf(stderr, "Erro ao alocar memória para HashNode\n");
		exit(EXIT_FAILURE);
	}

	new_node->item.key = key;
	new_node->item.val = val;
	new_node->next = h->buckets[index];
	h->buckets[index] = new_node;

	h->num_elems++;
	return NULL;
}

void *hash_table_get(HashTable *h, void *key)
{
	int index = h->hash_fn(h, key) % h->table_size;

	HashNode *node = h->buckets[index];

	while (node != NULL)
	{
		if (h->cmp_fn(node->item.key, key) == 0)
		{
			return node->item.val;
		}
		node = node->next;
	}

	return NULL;
}

int hash_table_size(HashTable *h)
{

	return h->table_size;
}

void *hash_table_pop(HashTable *h, void *key)
{
	int index = h->hash_fn(h, key) % h->table_size;

	HashNode *node = h->buckets[index];
	HashNode *prev = NULL;

	while (node != NULL)
	{
		if (h->cmp_fn(node->item.key, key) == 0)
		{
			void *val = node->item.val;

			if (prev == NULL)
			{
				h->buckets[index] = node->next;
			}
			else
			{
				prev->next = node->next;
			}

			free(node);
			h->num_elems--;
			return val;
		}

		prev = node;
		node = node->next;
	}

	return NULL; // Chave não encontrada
}

void hash_table_print(HashTable *h, void (*print_key)(void *), void (*print_value)(void *))
{
	if (h == NULL || print_key == NULL || print_value == NULL)
	{
		fprintf(stderr, "Erro: Parâmetros inválidos para hash_table_print.\n");
		return;
	}

	printf("Tabela Hash (Tamanho: %d, Elementos: %d):\n", h->table_size, h->num_elems);

	for (int i = 0; i < h->table_size; i++)
	{
		HashNode *node = h->buckets[i];

		if (node != NULL)
		{
			printf("Bucket %d:\n", i);
			while (node != NULL)
			{
				printf("  Chave: ");
				print_key(node->item.key);
				printf(", Valor: ");
				print_value(node->item.val);
				printf("\n");

				node = node->next;
			}
		}
	}
}

void hash_table_destroy(HashTable *h)
{
	for (int i = 0; i < h->table_size; i++)
	{
		HashNode *node = h->buckets[i];
		while (node != NULL)
		{
			HashNode *next = node->next;
			free(node);
			node = next;
		}
	}

	free(h->buckets);
	free(h);
}