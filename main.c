// #include <stdlib.h>
// #include <stdio.h>
// #include "hash.h"
// #include "binary_tree.h"
// #include <string.h>

// #define EPSILON 0.0001

// typedef struct Empresa
// {
// 	char nome[100];
// 	char sigla[32];
// 	float valorUnitario;
// 	int totalAcoes;
// 	int acoesVendidas;
// } Empresa;

// int hash_str(HashTable *h, void *data)
// {
// 	char *str = (char *)data;

// 	long hash_val = 0;
// 	int base = 127;

// 	for (size_t i = 0; i < strlen(str); i++)
// 		hash_val = (base * hash_val + str[i]) % hash_table_size(h);

// 	return hash_val;
// }

// int cmp_str(void *a, void *b)
// {
// 	return strcmp((char *)a, (char *)b);
// }

// int cmp_empresa(void *a, void *b)
// {
// 	Empresa *empresaA = (Empresa *)a;
// 	Empresa *empresaB = (Empresa *)b;

// 	if ((empresaA->valorUnitario) < (empresaB->valorUnitario))
// 		return -1;
// 	if ((empresaA->valorUnitario) > (empresaB->valorUnitario))
// 		return 1;
// 	return strcmp(empresaA->sigla, empresaB->sigla);
// }

// int cmp_chave(void *a, void *b)
// {
// 	float chaveA = *(float *)a;
// 	float chaveB = *(float *)b;
// 	if (chaveA == chaveB)
// 		return 0;
// 	return (chaveA > chaveB) ? 1 : -1;
// }

// void key_destroy_fn(void *key)
// {
// 	free(key);
// }

// void val_destroy_fn(void *val)
// {
// 	Empresa *e = (Empresa *)val;
// 	if (e == NULL)
// 	{
// 		return;
// 	}

// 	free(e);
// }

// Empresa *empresa_construct(char *nome, char *sigla, float valorUnitario, int totalAcoes, int acoesVendidas)
// {
// 	Empresa *empresa = malloc(sizeof(Empresa));
// 	strcpy(empresa->nome, nome);
// 	strcpy(empresa->sigla, sigla);
// 	empresa->valorUnitario = valorUnitario;
// 	empresa->totalAcoes = totalAcoes;
// 	empresa->acoesVendidas = acoesVendidas;
// 	return empresa;
// }

// void empresa_destroy(void *empresa)
// {
// 	Empresa *e = (Empresa *)empresa;
// 	free(e);
// }

// void print_values(HashTable *hash, BinaryTree *tree)
// {
// 	Vector *v_hash = hash_to_vector(hash);
// 	printf("Hash: ");
// 	for (int i = 0; i < vector_size(v_hash); i++)
// 	{
// 		HashTableItem *item = (HashTableItem *)vector_get(v_hash, i);

// 		Empresa *empresa = (Empresa *)item->val;

// 		printf("%s %s %.2f %d %d\n", empresa->nome, empresa->sigla, empresa->valorUnitario, empresa->totalAcoes, empresa->acoesVendidas);
// 	}

// 	// TREE

// 	Vector *v = binary_tree_inorder_traversal_recursive(tree);
// 	printf("InOrder recursivo: ");
// 	for (int i = 0; i < vector_size(v); i++)
// 	{
// 		KeyValPair *kvp = vector_get(v, i);
// 		Empresa *empresa = (Empresa *)kvp->value;
// 		printf("%s %s %.2f %d %d\n", empresa->nome, empresa->sigla, empresa->valorUnitario, empresa->totalAcoes, empresa->acoesVendidas);
// 	}
// 	printf("\n");
// }

// int main()
// {
// 	char arquivo[100];
// 	HashTable *hash = hash_table_construct(500, hash_str, cmp_str);
// 	BinaryTree *tree = binary_tree_construct(cmp_chave, key_destroy_fn, val_destroy_fn);

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
// 		char nome[100];
// 		char sigla[32];
// 		float valorUnitario;
// 		int totalAcoes;
// 		int acoesVendidas;
// 		fscanf(arq, "%s %s %f %d %d", nome, sigla, &valorUnitario, &totalAcoes, &acoesVendidas);

// 		valorUnitario = valorUnitario * 100 / 100;

// 		Empresa *empresa = empresa_construct(nome, sigla, valorUnitario, totalAcoes, acoesVendidas);

// 		Empresa *empresa_antiga = hash_table_set(hash, empresa->sigla, empresa);
// 		if (empresa_antiga != NULL)
// 		{
// 			free(empresa_antiga);
// 		}
// 		float *valorKey = malloc(sizeof(float));
// 		*valorKey = empresa->valorUnitario;
// 		binary_tree_add(tree, valorKey, empresa);
// 		i++;
// 	}

// 	fclose(arq);

// 	int m;
// 	scanf("%d", &m);

// 	for (int i = 0; i < m; i++)
// 	{
// 		char operacao[32];
// 		scanf("%s", operacao);

// 		if (strcmp(operacao, "UPDATE") == 0)
// 		{
// 			char sigla[32];
// 			float valor;
// 			scanf("%s", sigla);
// 			scanf("%f", &valor);

// 			Empresa *empresa = hash_table_get(hash, sigla);

// 			if (empresa == NULL)
// 			{
// 				printf("UPDATE: Empresa nao encontrada\n");
// 			}
// 			else
// 			{
// 				Empresa *nova_empresa_ht = empresa_construct(empresa->nome, empresa->sigla, valor, empresa->totalAcoes, empresa->acoesVendidas);
// 				float valorAntigo = empresa->valorUnitario;
// 				hash_table_set(hash, sigla, nova_empresa_ht);

// 				binary_tree_remove(tree, &valorAntigo);
// 				float *novoValorKey = malloc(sizeof(float));
// 				*novoValorKey = valor;
// 				binary_tree_add(tree, novoValorKey, nova_empresa_ht);
// 			}

// 			// empresa_destroy(empresa_antiga); // da problema no resultado
// 		}
// 		else if (strcmp(operacao, "GET") == 0)
// 		{
// 			char sigla[32];
// 			scanf("%s", sigla);

// 			Empresa *empresa = hash_table_get(hash, sigla);

// 			if (empresa == NULL)
// 			{
// 				printf("GET: Empresa nao encontrada\n");
// 			}
// 			else
// 			{
// 				printf("%s %s %.2f %d %d\n", empresa->nome, empresa->sigla, empresa->valorUnitario, empresa->totalAcoes, empresa->acoesVendidas);
// 			}
// 		}
// 		else if (strcmp(operacao, "RM") == 0)
// 		{
// 			char sigla[32];
// 			scanf("%s", sigla);

// 			Empresa *empresa = hash_table_pop(hash, sigla);

// 			if (empresa == NULL)
// 			{
// 				printf("RM: Empresa nao encontrada\n");
// 			}
// 			else
// 			{
// 				float *valor_key = malloc(sizeof(float));
// 				*valor_key = empresa->valorUnitario;
// 				binary_tree_remove(tree, valor_key);
// 				free(valor_key);
// 			}
// 		}
// 		else if (strcmp(operacao, "INTERVAL") == 0)
// 		{
// 			float min, max;
// 			scanf("%f", &min);
// 			scanf("%f", &max);

// 			Vector *interval = binary_tree_interval(tree, &min, &max);

// 			for (int i = 0; i < vector_size(interval); i++)
// 			{
// 				Empresa *empresa = vector_get(interval, i);
// 				printf("%s\n", empresa->sigla);
// 			}

// 			vector_destroy(interval);
// 		}
// 		else if (strcmp(operacao, "MIN") == 0)
// 		{
// 			KeyValPair *kvp_min = binary_tree_pop_min(tree);
// 			Empresa *empresa = (Empresa *)kvp_min->value;
// 			printf("%s\n", empresa->sigla);
// 		}
// 		else if (strcmp(operacao, "MAX") == 0)
// 		{
// 			KeyValPair *kvp_max = binary_tree_pop_max(tree);
// 			Empresa *empresa = (Empresa *)kvp_max->value;
// 			printf("%s\n", empresa->sigla);
// 		}
// 		else if (strcmp(operacao, "SORTED") == 0)
// 		{
// 			Vector *v = binary_tree_inorder_traversal_recursive(tree);
// 			for (int i = 0; i < vector_size(v); i++)
// 			{
// 				KeyValPair *kvp = vector_get(v, i);
// 				Empresa *empresa = (Empresa *)kvp->value;
// 				printf("%s %.2f\n", empresa->sigla, empresa->valorUnitario);
// 			}
// 			vector_destroy(v);
// 		}
// 	}

// 	// print_tree(tree);

// 	// print_values(hash, tree);

// 	hash_table_destroy(hash);
// 	binary_tree_destroy(tree);

// 	return 0;
// }
