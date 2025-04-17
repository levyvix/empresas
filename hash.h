
#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include "vector.h"

typedef struct HashTable HashTable;

typedef int (*HashFunction)(HashTable *h, void *);
typedef int (*CmpFunction)(void *k1, void *k2);

typedef struct
{
	void *key;
	void *val;
} HashTableItem;

// constroi a hash
HashTable *hash_table_construct(int table_size, HashFunction hash_fn, CmpFunction cmp_fn);

// funcao para insercao/atualizacao de pares chave-valor em O(1).
// Se a chave ja existir, atualiza o valor e retorna o valor antigo para permitir desalocacao.
void *hash_table_set(HashTable *h, void *key, void *val);

// retorna o valor associado com a chave key ou NULL se ela nao existir em O(1).
void *hash_table_get(HashTable *h, void *key);

// remove o par chave-valor e retorna o valor ou NULL se nao existir tal chave em O(1).
void *hash_table_pop(HashTable *h, void *key);

// numero de buckets
int hash_table_size(HashTable *h);

// numero de elementos inseridos
int hash_table_num_elems(HashTable *h);

// libera o espaco alocado para a tabela hash
void hash_table_destroy(HashTable *h);

void hash_table_print(HashTable *h, void (*print_key)(void *), void (*print_value)(void *));

Vector *hash_to_vector(HashTable *h);

#endif