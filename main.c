#include <stdlib.h>
#include <stdio.h>
#include "hash.h"
#include "binary_tree.h"
#include <string.h>

typedef struct Empresa
{
	char nome[100];
	char sigla[32];
	double valorUnitario;
	int totalAcoes;
	int acoesVendidas;
} Empresa;

int hash_str(HashTable *h, void *data)
{
	char *str = (char *)data;

	long hash_val = 0;
	int base = 31;

	for (size_t i = 0; i < strlen(str); i++)
		hash_val = (base * hash_val + str[i]) % hash_table_size(h);

	return hash_val;
}

void print_hash_key(void *key)
{
	printf("%s\n", (char *)key);
}

void print_empresa(void *empresa)
{
	Empresa *em = (Empresa *)empresa;
	printf("%s %s %lf %d %d\n", em->nome, em->sigla, em->valorUnitario, em->totalAcoes, em->acoesVendidas);
}

void print_hash_value(void *val)
{
	Empresa *e = (Empresa *)val;
	print_empresa(e);
}

void *get_key(void *obj)
{
	Empresa *empresa = (Empresa *)obj;

	return &empresa->valorUnitario;
}

int cmp_str(void *a, void *b)
{
	return strcmp((char *)a, (char *)b);
}

int cmp_chave(void *a, void *b)
{
	Empresa *empresaA = (Empresa *)a;
	Empresa *empresaB = (Empresa *)b;

	double valorUnitarioA = empresaA->valorUnitario;
	double valorUnitarioB = empresaB->valorUnitario;

	if (valorUnitarioA == valorUnitarioB)
	{
		char *siglaA = empresaA->sigla;
		char *siglaB = empresaB->sigla;

		return strcmp(siglaA, siglaB);
	}
	else
	{
		return (valorUnitarioA > valorUnitarioB) ? 1 : -1;
	}
}

void val_destroy_fn(void *val)
{
	Empresa *e = (Empresa *)val;
	if (e == NULL)
	{
		return;
	}

	free(e);
}

Empresa *empresa_construct(char *nome, char *sigla, double valorUnitario, int totalAcoes, int acoesVendidas)
{
	Empresa *empresa = malloc(sizeof(Empresa));
	strcpy(empresa->nome, nome);
	strcpy(empresa->sigla, sigla);
	empresa->valorUnitario = valorUnitario;
	empresa->totalAcoes = totalAcoes;
	empresa->acoesVendidas = acoesVendidas;
	return empresa;
}

void empresa_destroy(void *empresa)
{
	Empresa *e = (Empresa *)empresa;
	if (e == NULL)
	{
		return;
	}
	free(e);
}

int main()
{
	char arquivo[100];
	HashTable *hash = hash_table_construct(23, hash_str, cmp_str);
	BinaryTree *tree = binary_tree_construct(cmp_chave, get_key, val_destroy_fn);

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
		char nome[100];
		char sigla[32];
		double valorUnitario;
		int totalAcoes;
		int acoesVendidas;
		fscanf(arq, "%s %s %lf %d %d", nome, sigla, &valorUnitario, &totalAcoes, &acoesVendidas);

		Empresa *empresa = empresa_construct(nome, sigla, valorUnitario, totalAcoes, acoesVendidas);

		Empresa *empresa_antiga = hash_table_set(hash, empresa->sigla, empresa);
		if (empresa_antiga != NULL)
		{
			free(empresa_antiga);
		}
		binary_tree_add(tree, empresa);
		i++;
	}

	fclose(arq);

	int m;
	scanf("%d", &m);

	for (int i = 0; i < m; i++)
	{
		char operacao[32];
		scanf("%s", operacao);

		if (strcmp(operacao, "UPDATE") == 0)
		{
			char sigla[32];
			double valor;
			scanf("%s", sigla);
			scanf("%lf", &valor);

			Empresa *empresa = hash_table_get(hash, sigla);

			if (empresa == NULL)
			{
				printf("UPDATE: Empresa nao encontrada\n");
			}
			else
			{
				Empresa *nova_empresa = empresa_construct(empresa->nome, empresa->sigla, valor, empresa->totalAcoes, empresa->acoesVendidas);
				hash_table_set(hash, sigla, nova_empresa);

				Empresa *chave = empresa_construct(
					empresa->nome,
					empresa->sigla,
					empresa->valorUnitario,
					empresa->totalAcoes,
					empresa->acoesVendidas);

				binary_tree_remove(tree, chave);
				free(chave);

				binary_tree_add(tree, nova_empresa);
			}
		}
		else if (strcmp(operacao, "GET") == 0)
		{
			// print_tree(tree);
			char sigla[32];
			scanf("%s", sigla);

			Empresa *empresa = hash_table_get(hash, sigla);

			if (empresa == NULL)
			{
				printf("GET: Empresa nao encontrada\n");
			}
			else
			{
				printf("%s %s %.2lf %d %d\n", empresa->nome, empresa->sigla, empresa->valorUnitario, empresa->totalAcoes, empresa->acoesVendidas);
			}
		}
		else if (strcmp(operacao, "RM") == 0)
		{
			char sigla[32];
			scanf("%s", sigla);

			Empresa *empresa = hash_table_pop(hash, sigla);

			if (empresa == NULL)
			{
				printf("RM: Empresa nao encontrada\n");
			}
			else
			{

				Empresa *chave = empresa_construct("A", empresa->sigla, empresa->valorUnitario, empresa->totalAcoes, empresa->acoesVendidas);
				binary_tree_remove(tree, chave);
				free(chave);
			}
		}
		else if (strcmp(operacao, "INTERVAL") == 0)
		{
			// print_tree(tree);
			double min, max;
			scanf("%lf", &min);
			scanf("%lf", &max);

			Empresa *chave_min = empresa_construct("A", "A", min, 0, 0); // qualquer nome sempre vai ser maior

			Empresa *chave_max = empresa_construct("A", "zzzzzzzzzzzzzzzzzzzzz", max, 0, 0); // qualquer nome sempre vai ser menor

			Vector *interval = binary_tree_interval(tree, chave_min, chave_max);

			free(chave_min);
			free(chave_max);

			for (int i = 0; i < vector_size(interval); i++)
			{
				Empresa *empresa = vector_get(interval, i);
				printf("%s\n", empresa->sigla);
			}

			vector_destroy(interval);
		}
		else if (strcmp(operacao, "MIN") == 0)
		{
			Empresa *empresa = binary_tree_pop_min(tree);
			printf("%s\n", empresa->sigla);
		}
		else if (strcmp(operacao, "MAX") == 0)
		{
			Empresa *empresa = binary_tree_pop_max(tree);
			printf("%s\n", empresa->sigla);
		}
		else if (strcmp(operacao, "SORTED") == 0)
		{
			Vector *v = binary_tree_inorder_traversal_recursive(tree);
			for (int i = 0; i < vector_size(v); i++)
			{
				Empresa *empresa = vector_get(v, i);
				printf("%s %.2lf\n", empresa->sigla, empresa->valorUnitario);
			}
			vector_destroy(v);
		}
	}

	// print_tree(tree);

	// print_values(hash, tree);

	hash_table_destroy(hash);
	binary_tree_destroy(tree);

	return 0;
}
