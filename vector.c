#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

// Cria um vetor vazio
Vector *vector_construct()
{
	Vector *v = (Vector *)calloc(1, sizeof(Vector));
	v->data = NULL;
	v->size = 0;
	v->allocated = 0;
	return v;
}

// Adiciona um elemento ao final do vetor
void vector_push_back(Vector *v, void *val)
{
	// Se o vetor estiver cheio, realoca mais memória
	if (v->size == v->allocated)
	{
		v->allocated = (v->allocated == 0) ? 1 : v->allocated * 2;
		v->data = realloc(v->data, v->allocated * sizeof(void *));
	}

	// Adiciona o novo elemento
	v->data[v->size] = val;
	v->size++;
}

// Retorna o elemento na posição i
void *vector_get(Vector *v, int i)
{
	if (i < 0 || i >= v->size)
	{
		printf("Erro: Índice fora dos limites do vetor.\n");
		exit(1);
	}
	return v->data[i];
}

// Define o valor na posição i
void vector_set(Vector *v, int i, void *val)
{
	if (i < 0 || i >= v->size)
	{
		printf("Erro: Índice fora dos limites do vetor.\n");
		exit(1);
	}
	v->data[i] = val;
}

// Retorna o tamanho do vetor
int vector_size(Vector *v)
{
	return v->size;
}

// Encontra o índice de um valor no vetor (retorna -1 se não encontrar)
int vector_find(Vector *v, void *val)
{
	for (int i = 0; i < v->size; i++)
	{
		if (v->data[i] == val)
		{
			return i;
		}
	}
	return -1;
}

// Retorna o maior elemento do vetor (assumindo que podemos comparar elementos usando o operador "<")
void *vector_max(Vector *v)
{
	if (v->size == 0)
	{
		printf("Erro: Vetor vazio.\n");
		exit(1);
	}

	void *max = v->data[0];
	for (int i = 1; i < v->size; i++)
	{
		if (v->data[i] > max)
		{
			max = v->data[i];
		}
	}
	return max;
}

// Retorna o menor elemento do vetor (assumindo que podemos comparar elementos usando o operador "<")
void *vector_min(Vector *v)
{
	if (v->size == 0)
	{
		printf("Erro: Vetor vazio.\n");
		exit(1);
	}

	void *min = v->data[0];
	for (int i = 1; i < v->size; i++)
	{
		if (v->data[i] < min)
		{
			min = v->data[i];
		}
	}
	return min;
}

// Retorna o índice do maior elemento do vetor (assumindo que podemos comparar elementos usando o operador "<")
int vector_argmax(Vector *v)
{
	if (v->size == 0)
	{
		printf("Erro: Vetor vazio.\n");
		exit(1);
	}

	int argmax = 0;
	for (int i = 1; i < v->size; i++)
	{
		if (v->data[i] > v->data[argmax])
		{
			argmax = i;
		}
	}
	return argmax;
}

// Retorna o índice do menor elemento do vetor (assumindo que podemos comparar elementos usando o operador "<")
int vector_argmin(Vector *v)
{
	if (v->size == 0)
	{
		printf("Erro: Vetor vazio.\n");
		exit(1);
	}

	int argmin = 0;
	for (int i = 1; i < v->size; i++)
	{
		if (v->data[i] < v->data[argmin])
		{
			argmin = i;
		}
	}
	return argmin;
}

void *vector_pop_back(Vector *v)
{
	if (v->size == 0)
	{
		printf("Erro: Vetor vazio.\n");
		exit(1);
	}
	void *val = v->data[v->size - 1];
	v->size--;
	return val;
}

void vector_sort(Vector *v, int (*cmp)(void *, void *))
{
	for (int i = 0; i < v->size - 1; i++)
	{
		for (int j = i + 1; j < v->size; j++)
		{

			if (cmp(v->data[i], v->data[j]) > 0)
			{
				void *aux = v->data[i];
				v->data[i] = v->data[j];
				v->data[j] = aux;
			}
		}
	}
}
// Libera a memória alocada para o vetor
void vector_destroy(Vector *v)
{
	free(v->data);
	free(v);
}