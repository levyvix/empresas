
#ifndef _DATA_STRUCTURES_VECTOR_H_
#define _DATA_STRUCTURES_VECTOR_H_

typedef struct
{
    void **data;
    int size;
    int allocated;
} Vector;

Vector *vector_construct();

void vector_push_back(Vector *v, void* val);

void* vector_get(Vector *v, int i);
void vector_set(Vector *v, int i, void* val);
int vector_size(Vector *v);

int vector_find(Vector *v, void* val);

// Retorna o maior elemento do vector (assumindo que podemos comparar elementos usando o operador "<")
void* vector_max(Vector *v);

// Retorna o menor elemento do vector (assumindo que podemos comparar elementos usando o operador "<")
void* vector_min(Vector *v);

// Retorna o índice do maior elemento do vector (assumindo que podemos comparar elementos usando o operador "<")
int vector_argmax(Vector *v);

// Retorna o índice do menor elemento do vector (assumindo que podemos comparar elementos usando o operador "<")
int vector_argmin(Vector *v);

void* vector_pop_back(Vector *v);

void vector_destroy(Vector *v);

// vector sort
void vector_sort(Vector *v, int (*cmp)(void*, void*));
#endif
