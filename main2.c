#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include "binary_tree.h"
#include <math.h>
#include <string.h>

#define EPSILON 0.0001

typedef struct Jogador
{
	char nickname[100];
	char nome[100];
	int vitorias;
	int partidas;
} Jogador;

int hash_str(HashTable *h, void *data)
{
	char *str = (char *)data;

	long hash_val = 0;
	int base = 31;

	for (size_t i = 0; i < strlen(str); i++)
		hash_val = (base * hash_val + str[i]) % hash_table_size(h);

	return hash_val;
}

int cmp_str(void *a, void *b)
{
	return strcmp((char *)a, (char *)b);
}

int cmp_chave(void *a, void *b)
{
	float chaveA = *(float *)a;
	float chaveB = *(float *)b;
	if (chaveA == chaveB)
		return 0;
	return (chaveA > chaveB) ? 1 : -1;
}

void key_destroy_fn(void *key)
{
	free(key);
}

void val_destroy_fn(void *val)
{
	Jogador *j = (Jogador *)val;
	if (j == NULL)
	{
		return;
	}

	free(j);
}

Jogador *jogador_construct(char *nickname, char *nome, int vitorias, int partidas)
{
	Jogador *jogador = malloc(sizeof(Jogador));
	strcpy(jogador->nickname, nickname);
	strcpy(jogador->nome, nome);
	jogador->vitorias = vitorias;
	jogador->partidas = partidas;
	return jogador;
}

void jogador_destroy(void *jogador)
{
	Jogador *j = (Jogador *)jogador;
	if (j == NULL)
	{
		return;
	}
	free(j);
}

int main()
{
	char arquivo[100];
	HashTable *hash = hash_table_construct(23, hash_str, cmp_str);
	BinaryTree *tree = binary_tree_construct(cmp_chave, key_destroy_fn, val_destroy_fn);

	scanf("%s", arquivo);

	FILE *arq = fopen(arquivo, "r");
	if (arq == NULL)
	{
		printf("Erro ao abrir o arquivo\n");
		return 1;
	}

	int i = 0, n = 1;
	fscanf(arq, "%d", &n);

	while (i < n)
	{
		char nickname[100];
		char nome[100];
		int vitorias;
		int partidas;
		fscanf(arq, "%s %s %d %d", nickname, nome, &vitorias, &partidas);

		Jogador *jogador = jogador_construct(nickname, nome, vitorias, partidas);

		Jogador *jogador_antigo = hash_table_set(hash, jogador->nickname, jogador);
		if (jogador_antigo != NULL)
		{
			free(jogador_antigo);
		}
		float *valorKey = malloc(sizeof(float));
		*valorKey = round(((float)jogador->vitorias / (float)jogador->partidas) * 100) / 100;
		binary_tree_add(tree, valorKey, jogador);
		i++;
	}

	fclose(arq);

	int m;
	scanf("%d", &m);

	for (int i = 0; i < m; i++)
	{
		char operacao[32];
		scanf("%s", operacao);
		// TODO: implementar MATCH e knn
		if (strcmp(operacao, "GET") == 0)
		{
			char nickname[100];
			scanf("%s", nickname);

			Jogador *jogador = hash_table_get(hash, nickname);

			if (jogador == NULL)
			{
				printf("GET: Empresa nao encontrada\n");
			}
			else
			{
				printf("%s %s %d %d\n", jogador->nickname, jogador->nome, jogador->vitorias, jogador->partidas);
			}
		}
		else if (strcmp(operacao, "RM") == 0)
		{
			char nickname[100];
			scanf("%s", nickname);

			Jogador *jogador = hash_table_pop(hash, nickname);

			if (jogador == NULL)
			{
				printf("RM: Empresa nao encontrada\n");
			}
			else
			{
				float *valor_key = malloc(sizeof(float));
				*valor_key = round(((float)jogador->vitorias / (float)jogador->partidas) * 100) / 100;
				binary_tree_remove(tree, valor_key);
				free(valor_key);
			}
		}
		else if (strcmp(operacao, "INTERVAL") == 0)
		{
			float min, max;
			scanf("%f", &min);
			scanf("%f", &max);

			Vector *interval = binary_tree_interval(tree, &min, &max);

			for (int i = 0; i < vector_size(interval); i++)
			{
				Jogador *jogador = vector_get(interval, i);
				printf("%s\n", jogador->nickname);
			}

			vector_destroy(interval);
		}
		else if (strcmp(operacao, "MIN") == 0)
		{
			KeyValPair *kvp_min = binary_tree_pop_min(tree);
			Jogador *jogador = (Jogador *)kvp_min->value;
			printf("%s\n", jogador->nickname);
		}
		else if (strcmp(operacao, "MAX") == 0)
		{
			KeyValPair *kvp_max = binary_tree_pop_max(tree);
			Jogador *jogador = (Jogador *)kvp_max->value;
			printf("%s\n", jogador->nickname);
		}
		else if (strcmp(operacao, "SORTED") == 0)
		{
			Vector *v = binary_tree_inorder_traversal_recursive(tree);
			for (int i = 0; i < vector_size(v); i++)
			{
				KeyValPair *kvp = vector_get(v, i);
				Jogador *jogador = (Jogador *)kvp->value;
				float percentual = *(float *)kvp->key;
				printf("%s %.2f\n", jogador->nickname, percentual);
			}
			vector_destroy(v);
		}
		else if (strcmp(operacao, "VICTORIES") == 0)
		{

			// o número de vitórias e de partidas deve ser
			// incrementado de qtd para o jogador em ambas as estruturas. Na árvore, o
			// percentual de vitórias anterior deve ser usado para remover os dados do jogador e
			// um novo registro deve ser inserido com as quantidades atualizadas.

			char nickname[100];
			scanf("%s", nickname);

			Jogador *jogador = hash_table_get(hash, nickname);

			if (jogador == NULL)
			{
				printf("VICTORIES: Jogador nao encontrado\n");
			}
			else
			{

				float *valor_key = malloc(sizeof(float));
				*valor_key = round(((float)jogador->vitorias / (float)jogador->partidas) * 100) / 100;
				binary_tree_remove(tree, valor_key);
				free(valor_key);

				jogador->partidas++;
				jogador->vitorias++;

				Jogador *novo_jogador = jogador_construct(jogador->nickname, jogador->nome, jogador->vitorias, jogador->partidas);

				float *novo_valor_key = malloc(sizeof(float));
				*novo_valor_key = round(((float)novo_jogador->vitorias / (float)novo_jogador->partidas) * 100) / 100;
				binary_tree_add(tree, novo_valor_key, novo_jogador);
			}
		}
		else if (strcmp(operacao, "DEFEATS") == 0)
		{
			char nickname[100];
			scanf("%s", nickname);

			Jogador *jogador = hash_table_get(hash, nickname);

			if (jogador == NULL)
			{
				printf("DEFEATS: Jogador nao encontrado\n");
			}
			else
			{

				float *valor_key = malloc(sizeof(float));
				*valor_key = round(((float)jogador->vitorias / (float)jogador->partidas) * 100) / 100;
				binary_tree_remove(tree, valor_key);
				free(valor_key);

				jogador->partidas++;
				// jogador->vitorias++;

				Jogador *novo_jogador = jogador_construct(jogador->nickname, jogador->nome, jogador->vitorias, jogador->partidas);

				float *novo_valor_key = malloc(sizeof(float));
				*novo_valor_key = round(((float)novo_jogador->vitorias / (float)novo_jogador->partidas) * 100) / 100;
				binary_tree_add(tree, novo_valor_key, novo_jogador);
			}
		}
	}

	// print_tree(tree);

	// print_values(hash, tree);

	hash_table_destroy(hash);
	binary_tree_destroy(tree);

	return 0;
}
