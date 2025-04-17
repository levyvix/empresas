// #include <stdlib.h>
// #include <stdio.h>
// #include "hash.h"
// #include "binary_tree.h"
// #include <math.h>
// #include <string.h>

// typedef struct Jogador
// {
// 	char nickname[100];
// 	char nome[100];
// 	int vitorias;
// 	int partidas;
// 	double win_pct;
// } Jogador;

// int hash_str(HashTable *h, void *data)
// {
// 	char *str = (char *)data;

// 	long hash_val = 0;
// 	int base = 31;

// 	for (size_t i = 0; i < strlen(str); i++)
// 		hash_val = (base * hash_val + str[i]) % hash_table_size(h);

// 	return hash_val;
// }

// void print_hash_key(void *key)
// {
// 	printf("%s\n", (char *)key);
// }

// void print_jogador(void *jogador)
// {
// 	Jogador *j = (Jogador *)jogador;
// 	printf("%s %s %d %d %lf\n", j->nickname, j->nome, j->vitorias, j->partidas, j->win_pct);
// }

// void print_hash_value(void *val)
// {
// 	Jogador *j = (Jogador *)val;
// 	print_jogador(j);
// }

// void *get_key(void *obj)
// {
// 	Jogador *jogador = (Jogador *)obj;

// 	return &jogador->win_pct;
// }

// int cmp_str(void *a, void *b)
// {
// 	return strcmp((char *)a, (char *)b);
// }

// int cmp_chave(void *a, void *b)
// {
// 	Jogador *jogadorA = (Jogador *)a;
// 	Jogador *jogadorB = (Jogador *)b;

// 	double winA = jogadorA->win_pct;
// 	double winB = jogadorB->win_pct;

// 	if (winA == winB)
// 	{
// 		char *nicknameA = jogadorA->nickname;
// 		char *nicknameB = jogadorB->nickname;

// 		return strcmp(nicknameA, nicknameB);
// 	}
// 	else
// 	{
// 		return (winA > winB) ? 1 : -1;
// 	}
// }

// void val_destroy_fn(void *val)
// {
// 	Jogador *j = (Jogador *)val;
// 	if (j == NULL)
// 		return;

// 	free(j);
// }

// Jogador *jogador_construct(char *nickname, char *nome, int vitorias, int partidas, double win_pct)
// {
// 	Jogador *jogador = malloc(sizeof(Jogador));
// 	strcpy(jogador->nickname, nickname);
// 	strcpy(jogador->nome, nome);
// 	jogador->vitorias = vitorias;
// 	jogador->partidas = partidas;
// 	jogador->win_pct = win_pct;
// 	return jogador;
// }

// void jogador_destroy(void *jogador)
// {
// 	Jogador *j = (Jogador *)jogador;
// 	if (j == NULL)
// 	{
// 		return;
// 	}
// 	free(j);
// }

// int main()
// {
// 	char arquivo[100];
// 	HashTable *hash = hash_table_construct(23, hash_str, cmp_str);
// 	BinaryTree *tree = binary_tree_construct(cmp_chave, get_key, val_destroy_fn);

// 	scanf("%s", arquivo);

// 	FILE *arq = fopen(arquivo, "r");
// 	if (arq == NULL)
// 	{
// 		printf("Erro ao abrir o arquivo\n");
// 		return 1;
// 	}

// 	int i = 0, n = 1;
// 	fscanf(arq, "%d", &n);

// 	while (i < n)
// 	{
// 		char nickname[100];
// 		char nome[100];
// 		int vitorias;
// 		int partidas;
// 		fscanf(arq, "%s %s %d %d", nickname, nome, &vitorias, &partidas);

// 		Jogador *jogador = jogador_construct(nickname, nome, vitorias, partidas, vitorias / (double)partidas);

// 		Jogador *jogador_antigo = hash_table_set(hash, jogador->nickname, jogador);

// 		if (jogador_antigo != NULL)
// 		{
// 			free(jogador_antigo);
// 		}

// 		binary_tree_add(tree, jogador);
// 		i++;
// 	}
// 	// print_tree(tree);
// 	fclose(arq);

// 	int m;
// 	scanf("%d", &m);

// 	for (int i = 0; i < m; i++)
// 	{
// 		char operacao[32];
// 		scanf("%s", operacao);

// 		if (strcmp(operacao, "GET") == 0)
// 		{
// 			char nickname[100];
// 			scanf("%s", nickname);

// 			Jogador *jogador = hash_table_get(hash, nickname);

// 			if (jogador == NULL)
// 			{
// 				printf("GET: Empresa nao encontrada\n");
// 			}
// 			else
// 			{
// 				printf("%s %s %d %d\n", jogador->nickname, jogador->nome, jogador->vitorias, jogador->partidas);
// 			}
// 		}
// 		else if (strcmp(operacao, "RM") == 0)
// 		{
// 			char nickname[100];
// 			scanf("%s", nickname);

// 			Jogador *jogador = hash_table_pop(hash, nickname);

// 			if (jogador == NULL)
// 			{
// 				printf("RM: Jogador nao encontrado\n");
// 			}
// 			else
// 			{

// 				Jogador *chave = jogador_construct(jogador->nickname, "A", 0, 0, jogador->win_pct);
// 				binary_tree_remove(tree, chave);
// 				free(chave);
// 			}
// 		}
// 		else if (strcmp(operacao, "INTERVAL") == 0)
// 		{
// 			// print_tree(tree);
// 			double min, max;
// 			scanf("%lf", &min);
// 			scanf("%lf", &max);

// 			Jogador *chave_min = jogador_construct("A", "A", 0, 0, min); // quando tem 100% de win_rate, qualquer nome sempre vai ser maior

// 			Jogador *chave_max = jogador_construct("zzzzzzzzzzzzzzzzzzzzz", "A", 0, 0, max); // quando tem 100% de win_rate, qualquer nome sempre vai ser menor

// 			Vector *interval = binary_tree_interval(tree, chave_min, chave_max);

// 			free(chave_min);
// 			free(chave_max);

// 			for (int i = 0; i < vector_size(interval); i++)
// 			{
// 				Jogador *jogador = vector_get(interval, i);
// 				printf("%s\n", jogador->nickname);
// 			}

// 			vector_destroy(interval);
// 		}
// 		else if (strcmp(operacao, "MIN") == 0)
// 		{
// 			Jogador *jogador = binary_tree_pop_min(tree);
// 			printf("%s\n", jogador->nickname);
// 		}
// 		else if (strcmp(operacao, "MAX") == 0)
// 		{
// 			// print_tree(tree);
// 			Jogador *jogador = binary_tree_pop_max(tree);
// 			printf("%s\n", jogador->nickname);
// 		}
// 		else if (strcmp(operacao, "SORTED") == 0)
// 		{
// 			Vector *v = binary_tree_inorder_traversal_recursive(tree);
// 			for (int i = 0; i < vector_size(v); i++)
// 			{
// 				Jogador *jogador = vector_get(v, i);
// 				printf("%s %.2f\n", jogador->nickname, jogador->win_pct);
// 			}
// 			vector_destroy(v);
// 		}
// 		else if (strcmp(operacao, "VICTORIES") == 0)
// 		{
// 			char nickname[100];
// 			int qtd;
// 			scanf("%s %d", nickname, &qtd);

// 			Jogador *jogador = hash_table_get(hash, nickname);

// 			if (jogador == NULL)
// 			{
// 				printf("VICTORIES: Jogador %s nao encontrado\n", nickname);
// 			}
// 			else
// 			{
// 				Jogador *novo_jogador = jogador_construct(
// 					jogador->nickname,
// 					jogador->nome,
// 					jogador->vitorias + qtd,
// 					jogador->partidas + qtd,
// 					(jogador->vitorias + qtd) / (double)(jogador->partidas + qtd));

// 				hash_table_set(hash, nickname, novo_jogador);

// 				Jogador *chave = jogador_construct(nickname, "A", 0, 0, jogador->win_pct);
// 				binary_tree_remove(tree, chave);
// 				free(chave);

// 				binary_tree_add(tree, novo_jogador);
// 			}
// 		}
// 		else if (strcmp(operacao, "DEFEATS") == 0)
// 		{
// 			char nickname[100];
// 			int qtd;
// 			scanf("%s %d", nickname, &qtd);

// 			Jogador *jogador = hash_table_get(hash, nickname);

// 			if (jogador == NULL)
// 			{
// 				printf("DEFEATS: Jogador %s nao encontrado\n", nickname);
// 			}
// 			else
// 			{
// 				Jogador *novo_jogador = jogador_construct(
// 					nickname,
// 					jogador->nome,
// 					jogador->vitorias,
// 					jogador->partidas + qtd,
// 					(jogador->vitorias) / (double)(jogador->partidas + qtd));

// 				hash_table_set(hash, nickname, novo_jogador);

// 				Jogador *chave = jogador_construct(nickname, "A", 0, 0, jogador->win_pct);
// 				binary_tree_remove(tree, chave);
// 				free(chave);

// 				binary_tree_add(tree, novo_jogador);
// 			}
// 		}
// 		else if (strcmp(operacao, "MATCH") == 0)
// 		{
// 			// print_tree(tree);
// 			// hash_table_print(hash, print_hash_key, print_hash_value);

// 			char nickname[100];
// 			scanf("%s", nickname);

// 			Jogador *jogador = hash_table_get(hash, nickname);

// 			Jogador *chave = jogador_construct(jogador->nickname, "A", 0, 0, jogador->win_pct);

// 			Jogador *jogador_proximo = binary_tree_find_nearest(tree, chave);
// 			free(chave);

// 			if (jogador_proximo == NULL)
// 			{
// 				printf("MATCH: Jogador nao encontrado\n");
// 			}
// 			else
// 			{
// 				printf("%s\n", jogador_proximo->nickname);
// 			}
// 		}
// 	}

// 	// print_tree(tree);

// 	// print_values(hash, tree);

// 	hash_table_destroy(hash);
// 	binary_tree_destroy(tree);

// 	return 0;
// }
