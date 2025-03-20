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

typedef struct HashTableIterator
{
	HashTable *hash_table;	// Tabela hash a ser iterada
	int bucket_index;		// Índice do bucket atual
	HashNode *current_node; // Nó atual na lista encadeada
	int current_element;	// Elemento atual (contador)
} HashTableIterator;

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

// Inserção na tabela hash
void *hash_table_set(HashTable *h, void *key, void *val)
{
	int index = h->hash_fn(h, key) % h->table_size;

	HashNode *node = h->buckets[index];

	while (node != NULL)
	{
		if (h->cmp_fn(node->item.key, key) == 0)
		{
			// Chave encontrada, atualizar o valor
			void *old_val = node->item.val;
			node->item.val = val;
			return old_val; // Retorna o valor antigo
		}
		node = node->next;
	}

	// Chave não encontrada, criar um novo nó
	HashNode *new_node = calloc(1, sizeof(HashNode));
	if (new_node == NULL)
	{
		fprintf(stderr, "Erro ao alocar memória para HashNode\n");
		exit(EXIT_FAILURE);
	}

	new_node->item.key = key;
	new_node->item.val = val;
	new_node->next = h->buckets[index]; // Insere no início da lista
	h->buckets[index] = new_node;

	h->num_elems++;
	return NULL; // Retorna NULL porque não havia valor antigo
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

	return NULL; // Chave não encontrada
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
				// O nó a ser removido é o primeiro da lista
				h->buckets[index] = node->next;
			}
			else
			{
				// O nó a ser removido está no meio ou no final da lista
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

HashTableIterator *hash_table_iterator(HashTable *h)
{
	HashTableIterator *iterator = calloc(1, sizeof(HashTableIterator));
	iterator->hash_table = h;
	iterator->bucket_index = 0;
	iterator->current_node = h->buckets[0];
	iterator->current_element = 0;
	return iterator;
}

int hash_table_iterator_is_over(HashTableIterator *it)
{
	return it->current_element >= it->hash_table->num_elems;
}

HashTableItem *hash_table_iterator_next(HashTableIterator *it)
{
	// Se o nó atual for nulo, procura o próximo bucket não nulo
	while (it->current_node == NULL)
	{
		it->bucket_index++;
		if (it->bucket_index >= it->hash_table->table_size)
		{
			// Não há mais buckets para iterar
			return NULL;
		}
		it->current_node = it->hash_table->buckets[it->bucket_index];
	}

	// Retorna um ponteiro para o item atual
	HashTableItem *item = &(it->current_node->item);

	// Avança para o próximo nó na lista encadeada
	it->current_node = it->current_node->next;

	// Incrementa o contador de elementos
	it->current_element++;

	return item;
}

void hash_table_iterator_destroy(HashTableIterator *it)
{
	free(it);
}

Vector *hash_to_vector(HashTable *h)
{
	Vector *v = vector_construct();
	for (int i = 0; i < h->table_size; i++)
	{
		HashNode *node = h->buckets[i];
		while (node != NULL)
		{
			vector_push_back(v, &(node->item));
			node = node->next;
		}
	}
	return v;
}

void hash_table_destroy(HashTable *h)
{
	for (int i = 0; i < h->table_size; i++)
	{
		HashNode *node = h->buckets[i];
		while (node != NULL)
		{
			HashNode *next = node->next; // Salva o próximo nó antes de liberar o atual
			free(node);
			node = next;
		}
	}

	free(h->buckets);
	free(h);
}